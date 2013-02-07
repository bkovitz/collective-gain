#include <algorithm>
#include <unordered_map>
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "random.h"

using std::cout;
using std::endl;
using std::unordered_map;
using std::map;
using std::vector;

typedef unsigned int uint;
typedef uint id;

double
distance_from_origin(double dx, double dy) {
  return sqrt(pow(dx, 2) + pow(dy, 2));
}

struct Coord {
  int x, y;

  bool operator==(const Coord& that) const {
    return x == that.x && y == that.y;
  }
};

namespace std {
  template <> struct hash<Coord> {
    size_t operator()(const Coord& c) const {
      return hash<int>()(c.x << 8 + c.y);
    }
  };
}

template <typename T>
class Array2D {
  uint xsize, ysize;
  T* array;
public:
  Array2D(uint x, uint y) : xsize(x), ysize(y), array(new T[x * ysize]) { }
  ~Array2D() { delete[] array; }
  T& operator()(uint x, uint y) { return array[x * ysize + y]; }

  bool in_bounds(int x, int y) {
    return x >= 0 && x < xsize && y >= 0 && y < ysize;
  }

  void zeros() {
    for (uint x = 0; x < xsize; x++)
      for (uint y = 0; y < ysize; y++)
        (*this)(x, y) = 0;
  }

  void ones() {
    for (uint x = 0; x < xsize; x++)
      for (uint y = 0; y < ysize; y++)
        (*this)(x, y) = 1;
  }

  void print() {
    for (uint y = 0; y < ysize; y++) {
      for (uint x = 0; x < xsize; x++) {
        cout << std::setprecision(2) << std::setw(4) << std::fixed
             << (*this)(x, y) << ' ';
      }
      cout << endl;
    }
  }

  void print_pct() {
    for (uint y = 0; y < ysize; y++) {
      for (uint x = 0; x < xsize; x++) {
        cout << std::setprecision(0) << std::setw(4) << std::fixed
             << (*this)(x, y) * 100.0 << ' ';
      }
      cout << endl;
    }
  }
};

struct Organism {
  double giver_probability;
  double sunlight_absorbed;
  bool is_giver;
  int x, y;

  Organism(double g, int x_, int y_) : giver_probability(g), x(x_), y(y_) { }
  Organism(const Organism& that) :
    giver_probability(that.giver_probability),
    sunlight_absorbed(that.sunlight_absorbed),
    is_giver(that.is_giver),
    x(that.x),
    y(that.y) { }
  Organism& operator=(const Organism& that) {
    giver_probability = that.giver_probability;
    sunlight_absorbed = that.sunlight_absorbed;
    is_giver = that.is_giver;
    x = that.x;
    y = that.y;
  }
};

class RadialDecayTable {
  const int radius;
  const double r2;  // radius squared
  mutable Array2D<double> table; // table[dx][dy] = amount after decay at dx,dy
  double sum_;

public:
  RadialDecayTable(uint r)
  :
    radius(r),
    r2(radius * radius),
    table(radius * 2 + 1, radius * 2 + 1),
    sum_(0.0)
  {
    for (int dx = -radius; dx <= radius; dx++) {
      for (int dy = -radius; dy <= radius; dy++) {
        /*
        double value = sqrt(1.0 - pow(distance_from_origin(dx, dy), 2)
                                  /
                                  r2
                           );
        */
        double value = radius - distance_from_origin(dx, dy);
        if (value > 0.0) {
          table(dx + radius, dy + radius) = value;
          sum_ += value;
        } else
          table(dx + radius, dy + radius) = 0.0;
      }
    }
  }

  double amount(int dx, int dy) const {
    if (distance_from_origin(dx, dy) > radius)
      return 0.0;
    else
      return table(dx + radius, dy + radius);
  }

  void apply_to(
    Array2D<double>& array,
    int x_center,
    int y_center,
    double factor = 1.0
  ) const {
    for (int dx = -radius; dx <= radius; dx++) {
      for (int dy = -radius; dy <= radius; dy++) {
        int x = x_center + dx;
        int y = y_center + dy;
        if (array.in_bounds(x, y)) {
          double delta = factor * table(dx + radius, dy + radius);
          array(x, y) += delta;
        }
      }
    }
  }

  double sum() { return sum_; }
};

class Simulation {
  const int world_x;
  const int world_y;

  const int num_initial_organisms = 20;
  const double predation_factor = 0.90;
  const double protection_factor = 1.0;
  const double fecundity = 1.0;
  const double acorn_distance_factor = 0.2;
  const int absorption_radius = 4;
  const int protection_radius = 4;

  const RadialDecayTable radial_absorption;
  const RadialDecayTable radial_protection;

  Array2D<double> total_attempted_absorption;
  Array2D<double> total_protection;
  unordered_map<Coord, Organism> organisms;
  unordered_map<Coord, Organism> offspring;

  int num_givers = 0;

  id next_id;
  RandomUniformInt random_x;
  RandomUniformInt random_y;
  RandomUniformReal unit_interval_random = RandomUniformReal(0.0, 1.0);
  RandomUniformReal random_angle = RandomUniformReal(0.0, 2.0 * M_PI);
  RandomNormal random_mutation_delta = RandomNormal(0.0, 0.01);

public:
  Simulation(uint x, uint y)
  :
    world_x(x),
    world_y(y),
    radial_absorption(absorption_radius),
    radial_protection(protection_radius),
    next_id(0),
    total_attempted_absorption(world_x, world_y),
    total_protection(world_x, world_y),
    random_x(0, world_x - 1),
    random_y(0, world_y - 1)
  { }

  void run(unsigned int num_generations) {
    make_initial_organisms();
    for (int gen = 0; gen < num_generations; gen++) {
      cout << "generation " << gen << endl;
      run_one_generation();
      cout << endl;
      if (organisms.size() == 0)
        break;
    }
  }

  void run_one_generation() {
    print();

    total_protection.ones();
    total_attempted_absorption.zeros();

    add_up_attempted_absorption();
    figure_sunlight_before_predation();
    print_sunlight_absorbed();

    decide_who_gives();
    cout << "# of givers: " << num_givers
         << '(' << std::setprecision(1)
         << 100.0 * (double)num_givers / organisms.size()
         << "%)" << endl;
    distribute_protection();
    print_protection();
    perform_predation();
    print_sunlight_absorbed();

    make_offspring();
    std::swap(organisms, offspring);
    offspring.clear();
  }

  void make_initial_organisms() {
    for (int i = 0; i < num_initial_organisms; i++) {
      Coord coord = find_random_empty_pixel();
      organisms.emplace(coord, Organism(0.0, coord.x, coord.y));
    }
  }

  Coord find_random_empty_pixel() {
    Coord c;
    do {
      c.x = random_x();
      c.y = random_y();
    } while (organisms.count(c) != 0);
    return c;
  }

  void add_up_attempted_absorption() {
    for (auto it = organisms.begin(); it != organisms.end(); ++it) {
      Organism& o = it->second;
      radial_absorption.apply_to(total_attempted_absorption, o.x, o.y);
    }
  }

  void figure_sunlight_before_predation() {
    for (auto it = organisms.begin(); it != organisms.end(); ++it) {
      Organism& o = it->second;
      o.sunlight_absorbed = 0.0;
      for (int dx = -absorption_radius; dx <= absorption_radius; dx++) {
        for (int dy = -absorption_radius; dy <= absorption_radius; dy++) {
          int x = o.x + dx;
          int y = o.y + dy;
          if (total_attempted_absorption.in_bounds(x, y)) {
            double tot = total_attempted_absorption(x, y);
            if (tot <= 1.0)
              o.sunlight_absorbed += radial_absorption.amount(dx, dy);
            else
              o.sunlight_absorbed += radial_absorption.amount(dx, dy) / tot;
          }
        }
      }
    }
  }

  void decide_who_gives() {
    num_givers = 0;
    for (auto it = organisms.begin(); it != organisms.end(); ++it) {
      Organism& o = it->second;
      o.is_giver = unit_interval_random() < o.giver_probability;
      if (o.is_giver)
        num_givers++;
    }
  }

  void distribute_protection() {
    for (auto it = organisms.begin(); it != organisms.end(); ++it) {
      Organism& o = it->second;
      if (o.is_giver)
        radial_protection.apply_to(
            total_protection, o.x, o.y, o.sunlight_absorbed * protection_factor
        );
    }
  }

  void perform_predation() {
    for (auto it = organisms.begin(); it != organisms.end(); ++it) {
      Organism& o = it->second;
      double predation_loss = pow(predation_factor, total_protection(o.x, o.y));
      o.sunlight_absorbed *= 1.0 - predation_loss;
    }
  }

  void make_offspring() {
    offspring.clear();
    for (auto it = organisms.begin(); it != organisms.end(); ++it) {
      Organism& o = it->second;
      if (!o.is_giver) {
        double num_children = random_poisson(o.sunlight_absorbed * fecundity);
        /*
        cout << '(' << o.x << ", " << o.y << ')'
             << " num_children: " << num_children 
             << " g: " << o.giver_probability
             << " sunlight_absorbed: " << o.sunlight_absorbed
             << endl;
        */
        for (int i = 0, children_produced = 0;
             i < 3 * num_children && children_produced < num_children;
             i++) {
          double distance_from_parent = fabs(random_normal(
              0.0, o.sunlight_absorbed * acorn_distance_factor
          ));
          double angle = random_angle();
          Coord c;
          //cout << "  distance_from_parent: " << distance_from_parent << endl;
          c.x = o.x + distance_from_parent * cos(angle);
          c.y = o.y + distance_from_parent * sin(angle);
          if (in_bounds(c.x, c.y) && offspring.count(c) == 0) {
            offspring.emplace(
                c,
                Organism(mutation_delta(o.giver_probability), c.x, c.y)
            );
            children_produced++;
            /*
            cout << "  child at "
                 << '(' << c.x << ", " << c.y << ')'
                 << " g: " << std::setprecision(2)
                 <<           offspring.at(c).giver_probability
                 << endl;
            */
          }
        }
      }
    }
  }

  bool in_bounds(int x, int y) {
    return x >= 0 && x < world_x && y >= 0 && y < world_y;
  }

  double mutation_delta(double g) {
    double result = g + random_mutation_delta();
    if (result < 0.0)
      return 0.0;
    else if (result > 1.0)
      return 1.0;
    else
      return result;
  }

  void print() {
    cout << "# of organisms: " << organisms.size() << endl;
    cout << "average g: " << std::setprecision(2) << average_g() << endl;

    Array2D<double> world(world_x, world_y);
    world.zeros();
    for (auto it = organisms.begin(); it != organisms.end(); ++it) {
      Organism& o = it->second;
      world(o.x, o.y) = o.giver_probability;
    }
    for (int y = 0; y < world_y; y++) {
      for (int x = 0; x < world_x; x++) {
        Coord c;
        c.x = x;
        c.y = y;
        if (organisms.count(c) == 0)
          cout << "   - ";
        else {
          const Organism& o = organisms.at(c);
          cout << std::setprecision(0) << std::fixed << std::setw(4)
               << o.giver_probability * 100.0 << ' ';
        }
      }
      cout << endl;
    }
    cout << endl;
  }

  double average_g() {
    double total_g = 0;
    for (auto it = organisms.begin(); it != organisms.end(); ++it) {
      Organism& o = it->second;
      total_g += o.giver_probability;
    }
    return 100.0 * total_g / organisms.size();
  }

  void print_sunlight_absorbed() {
    cout << "absorption" << endl;
    Array2D<double> world(world_x, world_y);
    world.zeros();
    for (auto it = organisms.begin(); it != organisms.end(); ++it) {
      Organism& o = it->second;
      world(o.x, o.y) = o.sunlight_absorbed;
    }
    world.print();
    cout << endl;
  }

  void print_protection() {
    cout << "protection" << endl;
    total_protection.print();
    cout << endl;
  }

  void print_g_distribution() {
    cout << "g distribution" << endl;
    map<int, int> gcounts;

    for (auto it = organisms.begin(); it != organisms.end(); ++it) {
      Organism& o = it->second;
      int g = (int)(o.giver_probability * 100.0 + 0.5);
      if (gcounts.count(g) == 0)
        gcounts[g] = 1;
      else
        gcounts[g]++;
    }

    for (auto it = gcounts.begin(); it != gcounts.end(); ++it) {
      int g = it->first;
      int count = it->second;
      cout << g << ": " << count << endl;
    }
  }
};

int
main() {
  Simulation sim(200, 200);
  sim.run(5);
  sim.print();
  sim.print_g_distribution();
}

