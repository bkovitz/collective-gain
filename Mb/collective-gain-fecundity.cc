/*
 * collective-gain-fecundity.cc -- Givers & Takers simulation
 *
 * Very simple: givers add to number of offspring of neighboring takers.
 */

//#define BOOST_TEST_DYN_LINK  // enable or compile with -D for unit tests
#define BOOST_TEST_MODULE CollectiveGain
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <map>
#include <ranges>
#include <functional>
#include <getopt.h>

#include "random.h"

using std::cout;
using std::cerr;
using std::endl;

RandomUniformReal unit_interval_random = RandomUniformReal(0.0, 1.0);
//std::uniform_real_distribution<double> unit_interval_random(0.0, 1.0);
RandomUniformReal random_angle = RandomUniformReal(0.0, 2.0 * M_PI);

double initial_g = 0.01;  // g of all organisms in zeroth generation
int num_gens = 10000;       // number of generations to run
double base_fecundity = 1.0;
int world_xsize = 20;
int world_ysize = 20;
double acorn_stdev = 2.0;  // std deviation of child's distance from parent

struct Coord;

std::ostream& operator<<(std::ostream& os, const Coord& c);

struct Coord {
  const int x, y; 

  bool operator==(const Coord& that) const {
    return x == that.x && y == that.y;
  }

  /*
  bool operator<(const Coord& that) const {
    bool result;
    if (y < that.y)
      result = true;
    else
      result = x < that.x;
    //cout << "Coord " << *this << ' ' << that << ' ' << result << endl;
    return result;
  };
  */

  static Coord random(int max_x, int max_y) {
    return Coord{RandomUniformInt(0, max_x)(), RandomUniformInt(0, max_y)()};
  }

  std::vector<Coord> delta_square(int max_delta) const {
    std::vector<Coord> result{};

    for (int i = -max_delta; i <= max_delta; i++)
      for (int j = -max_delta; j <= max_delta; j++)
        result.push_back(Coord{i, j});
    return result;
  }
};

std::ostream& operator<<(std::ostream& os, const Coord& c) {
  return os << '(' << c.x << ", " << c.y << ')';
};

namespace std {
  template <> struct hash<Coord> {
    size_t operator()(const Coord& c) const {
      return hash<int>()((c.x << 16) + c.y);
    }
  };
}

struct Organism {
  const double g;    // probability of becoming a giver
  double fecundity;  // expected number of children
  bool is_giver = false;

  Organism(double _g) : g(_g), fecundity(base_fecundity) { }

  // returns true if this Organism became a giver
  bool develop_into_giver_or_taker() {
    if (unit_interval_random() < g)
    //if (unit_interval_random(randoms::rng) < g)
      is_giver = true;
    return is_giver;
  }
};

std::ostream& operator<<(std::ostream& os, const Organism& o) {
  if (o.is_giver)
    os << " (g)";
  else
    os << std::fixed << std::setw(4) << std::setprecision(0) << o.g * 100;
  return os;
}

class World {
public:
  const int xsize, ysize;
  std::unordered_map<Coord, Organism> organisms;       // current population
  std::unordered_map<Coord, Organism> next_organisms;  // next generation's population

  const int giving_radius = 4;
  int children_attempted = 0;
  int children_produced = 0;
  int num_givers = 0;
  
  RandomNormal random_mutation_delta = RandomNormal(0.0, 0.01);

  World(int x, int y) : xsize(x), ysize(y), organisms(), next_organisms() { }
  World(int x, int y, std::unordered_map<Coord, Organism>&& orgs)
  : xsize(x), ysize(y), organisms(std::move(orgs)), next_organisms() { }

  static World make_world(int x, int y, int num_initial_organisms = 20) {
//    auto place_random_initial_organism = [&]() {
//      cout << x << y << endl;
//    };
//    place_random_initial_organism();
    std::unordered_map<Coord, Organism> m{};
    for (int i = 0; i < 3 * num_initial_organisms; i++) {
      Coord c = Coord::random(x - 1, y - 1);
      //cout << "making at " << c << endl;
      //m.try_emplace(c, Organism(initial_g));
      m.try_emplace(c, initial_g);
      if (m.size() >= num_initial_organisms)
        break;
    }
      
    //cout << "World made. size=" << m.size() << endl;
    return World(x, y, std::move(m));
  }

  void run_one_generation() {
    children_attempted = 0;
    children_produced = 0;
    num_givers = 0;
    develop_into_givers();
    cout << "organisms:" << endl;
    print_as_grid(organisms);
    givers_spread_fecundity();
    print_fecundity();
    takers_reproduce();
    std::swap(organisms, next_organisms);
    print();
  }

  void develop_into_givers() {
    for (Organism& o : std::views::values(organisms)) {
      o.develop_into_giver_or_taker();
      if (o.is_giver)
        num_givers++;
    }
  }

  void givers_spread_fecundity1() {
    for (auto& pair : organisms) {
      const Coord& c = pair.first;
      Organism& o = pair.second;
      if (o.is_giver) {
        for (Coord& d : c.delta_square(giving_radius)) {
          //cout << "d = " << d << endl;
          Coord recipient_coord = add_coords(c, d);
          if (organisms.count(recipient_coord) > 0) {
            organisms.at(recipient_coord).fecundity += 1.0;
            cout << recipient_coord << ' ' << organisms.at(recipient_coord).fecundity << endl;
          }
        }
        cout << "giver at " << c << " fecundity = " << o.fecundity << endl;
      }
    }
  }

  void givers_spread_fecundity() {
    for (auto& pair : givers()) {
      const Coord& c = pair.get().first;
      //Organism& o = pair.second;
      for (Coord& d : c.delta_square(giving_radius)) {
        //cout << "d = " << d << endl;
        Coord recipient_coord = add_coords(c, d);
        if (organisms.count(recipient_coord) > 0) {
          organisms.at(recipient_coord).fecundity += 1.0;
          //cout << recipient_coord << ' ' << organisms.at(recipient_coord).fecundity << endl;
        }
      }
    }
    for (auto& pair : givers())
      pair.get().second.fecundity = 0.0;
  }

  std::vector<std::reference_wrapper<std::pair<const Coord, Organism>>>
  givers() {
    std::vector<std::reference_wrapper<std::pair<const Coord, Organism>>>
    result{};

    for (auto& pair : organisms)
      if (pair.second.is_giver)
        result.push_back(std::ref(pair));
    return result;
  }

  void takers_reproduce() {
    next_organisms.clear();
    for (auto& pair : organisms) {
      const Coord& coord = pair.first;
      Organism& o = pair.second;
      if (!o.is_giver)   // if taker
        reproduce(o, coord);
    }
  }

  void reproduce(const Organism& o, const Coord& coord) {
    //double num_children = random_poisson(o.fecundity);
    double num_children = o.fecundity;
    int children_actually_produced = 0;
//    cout << "Organism at " << coord << ": num_children=" << num_children
//         << endl;
    for (int i = 0; i < 200; i++) {
      if (children_actually_produced >= num_children)
        break;
      double distance_from_parent = fabs(random_normal(0.0, 2));
          // avg distance = 2 cells
      double angle = random_angle();
      Coord child_coord = normalize_coord(Coord{
        (int)std::round(coord.x + distance_from_parent * cos(angle)),
        (int)std::round(coord.y + distance_from_parent * sin(angle))
      });
      if (next_organisms.count(child_coord) == 0) {
//         cout << "  " << child_coord << " .count="
//              << next_organisms.count(child_coord) << endl;
        //const auto& pair = next_organisms.emplace(
        next_organisms.emplace(
          child_coord,
          //Organism{mutated(o.g)}
          mutated(o.g)
        );
        children_actually_produced++;
//        cout << "  made: " << child_coord << ' ' << pair.second
//             << " .count="
//             << next_organisms.count(child_coord) << ' '
//             << next_organisms.count(Coord{child_coord.x, child_coord.y})
//             << endl;
      }
    }
//    cout << "  children_actually_produced=" << children_actually_produced
//         << endl;
    children_attempted += num_children;
    children_produced += children_actually_produced;
  }

  double mutated(double g) {
    double result = g + random_mutation_delta();
    if (result < 0.0)
      return 0.0;
    else if (result > 1.0)
      return 1.0;
    else
      return result;
  }

  Coord normalize_coord(const Coord& c) {
    int x = c.x;
    if (x >= 0)
      x = x % xsize;
    else
      x = (xsize - abs(x) % xsize) % xsize;
    int y = c.y;
    if (y >= 0)
      y = y % ysize;
    else
      y = (ysize - abs(y) % ysize) % ysize;
    //cout << "normalize_coord(" << c << ") = " << Coord{x, y} << endl;
    return Coord{x, y};
  }

  Coord add_coords(const Coord& c, const Coord& delta) {
    // TODO OAOO normalize_coord
    int x = c.x + delta.x;
    int y = c.y + delta.y;
    Coord result = normalize_coord(Coord{x, y});
    //cout << "add_coords(" << c << ", " << delta << ") = " << result << endl;
    return result;
  }

  void print() {
    cout << "next organisms:\n";
    print_as_grid(organisms);
    print_data();
    //cout << endl;
    //print_as_grid(next_organisms);
    //cout << next_organisms.size() << endl;
    
  }

  void print_data() {
    cout << "data, " << organisms.size() <<
                ", " << std::fixed << std::setprecision(2) << average_g() <<
                ", " << num_givers <<
                ", " << children_attempted <<
                ", " << children_produced << endl;
    if (organisms.size() > xsize * ysize) {
      for (const auto& pair : organisms)
        cout << "  " << pair.first << ' ' << pair.second << ' '
             << (pair.second.is_giver ? "(g)" : "") << endl;
    }
  }

  double average_g() {
    double total_g = 0.0;
    for (const auto& o : std::views::values(organisms))
      total_g += o.g;
    return total_g / static_cast<double>(organisms.size());
  }

  void print_as_grid(const std::unordered_map<Coord, Organism>& organisms) {
    for (int i = 0; i < xsize; i++) {
      for (int j = 0; j < ysize; j++) {
        Coord c{i, j};
        if (organisms.count(c) == 0)
          cout << "   - ";
        else
          cout << organisms.at(c) << ' ';
      }
      cout << endl;
    }
  }

  void print_fecundity() {
    cout << "fecundities:" << endl;
    //cout << "num_givers: " << num_givers << endl;
    for (int i = 0; i < xsize; i++) {
      for (int j = 0; j < ysize; j++) {
        Coord c{i, j};
        if (organisms.count(c) == 0)
          cout << "   - ";
        else if (organisms.at(c).is_giver)
          cout << " (g) ";
        else
          cout << std::fixed << std::setw(4) << std::setprecision(0)
               << organisms.at(c).fecundity << ' ';
      }
      cout << endl;
    }
    cout << organisms.size() << endl;
  }
};

//std::ostream& operator<<(std::ostream& os, const World& world) {
//  for (int i = 0; i < world.xsize; i++) {
//    for (int j = 0; j < world.ysize; j++)
//      cout << world(i, j) << ' ';
//    cout << endl;
//  }
//}

/*
BOOST_AUTO_TEST_CASE(Coord_test) {
  Coord c{2, 3}, d{2, 4};

  //cout << c << endl;
  cout << Coord{6, 4} << endl;
  BOOST_CHECK(true);
  //bool a = Coord{2, 3} < Coord{2, 4};
  BOOST_CHECK_LT(c, d);
}
*/

void short_test() {
  std::unordered_map<Coord, std::string> m{};
  World w = World::make_world(20, 20);
  cout << Coord::random(20, 20) << ' ' << Coord::random(20, 20) << endl;
  w.run_one_generation();
  //w.print();
  cout << endl;
}

void set_up_experiment1() {
  cout << "experiment1" << endl;
  base_fecundity = 1.0;
  world_xsize = 20;
  world_ysize = 20;
  num_gens = 400;
  initial_g = 0.01;
}

void set_up_experiment2() {
  cout << "experiment2" << endl;
  base_fecundity = 0.0;
  world_xsize = 20;
  world_ysize = 20;
  num_gens = 4000;
  initial_g = 0.5;
}

void run_experiment() {
  World w = World::make_world(world_xsize, world_ysize);
  cout << "gen 0" << endl;
  w.print();
  for (int gen = 1; gen < num_gens + 1; gen++) {
    cout << endl << "gen" << gen << endl;
    w.run_one_generation();
    //w.print();
  }
}

void print_command_line_options()
{
  static std::string lc = " \\\n";  // line continuation
  cout << "--initial_g=" << initial_g << lc;
  cout << "--num_gens=" << num_gens << lc;
  cout << "--xsize=" << world_xsize << lc;
  cout << "--ysize=" << world_ysize << lc;
  cout << "--base_fecundity=" << base_fecundity << lc;
  cout << "--seed=" << randoms::seed << lc;
  cout << "--acorn_stdev=" << acorn_stdev << lc;
}

/*
 * Sets up global variables to match command-line options. Returns 0 if
 * successful, or an error code to return to the shell
 *
 * Initialized the random-number seed.
 */

int parse_command_line_options(int argc, char** argv)
{
  int option_index = 0;
  static struct option long_options[] = {
    { "initial_g", required_argument, 0, 0 },
    { "num_gens", required_argument, 0, 0},
    { "xsize", required_argument, 0, 0},
    { "ysize", required_argument, 0, 0},
    { "base_fecundity", required_argument, 0, 0},
    { "seed", required_argument, 0, 0},
    { "acorn_stdev", required_argument, 0, 0},
    { NULL, 0, 0, 0 }
  };

  for ( ; ; ) {
    int c = getopt_long(argc, argv, "", long_options, &option_index);
    if (c == -1)
      break;
    if (c == 0) {
      switch (option_index) {
      case 0:
        initial_g = atof(optarg);
        break;
      case 1:
        num_gens = atoi(optarg);
        break;
      case 2:
        world_xsize = atoi(optarg);
        break;
      case 3:
        world_ysize = atoi(optarg);
        break;
      case 4:
        base_fecundity = atof(optarg);
        break;
      case 5:
        randoms::reseed(atoi(optarg));
        break;
      case 6:
        acorn_stdev = atof(optarg);
        break;
      default:
        cerr << "Internal error\n";
        return 2;
      }
    } else {
      return 1; // unrecognized option
    }
  }
  if (optind < argc) {
    cerr << "unknown args --\n";
    while (optind < argc)
      cerr << "  " << argv[optind++] << endl;
    return 1;
  }
  if (randoms::need_seed)
    randoms::reseed();
  return 0;
}

int main(int argc, char** argv) {
  cout << std::boolalpha;
  int err = parse_command_line_options(argc, argv);
  if (err != 0)
    exit(err);
  print_command_line_options();

  // a way to see if the random-number seed stuff is working
  for (int i = 0; i < 5; i++) {
    cout << unit_interval_random() << endl;
  }

  /*
  set_up_experiment1();
  */
  run_experiment();
  return 0;
}
