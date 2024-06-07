/*
 * collective-gain.cc -- Givers & Takers simulation
 *
 * Very simple: givers add to number of offspring of neighboring takers.
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CollectiveGain
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <map>
#include <ranges>
#include <functional>

#include "random.h"

using std::cout;
using std::endl;

RandomUniformReal unit_interval_random = RandomUniformReal(0.0, 1.0);

struct Coord {
  const int x, y; 

  bool operator==(const Coord& that) const {
    return x == that.x && y == that.y;
  }
  bool operator<(const Coord& that) const {
    if (y < that.y)
      return true;
    else
      return x < that.x;
  };

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

struct Organism {
  const float g;    // probability of becoming a giver
  float fecundity;  // expected number of children
  bool is_giver = false;

  Organism(float _g) : g(_g), fecundity(1.0) { }

  // returns true if this Organism became a giver
  bool develop_into_giver_or_taker() {
    if (unit_interval_random() < g)
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
  std::map<Coord, Organism> organisms;

  const int giving_radius = 4;

  World(int x, int y) : xsize(x), ysize(y), organisms() { }
  World(int x, int y, std::map<Coord, Organism>&& orgs)
  : xsize(x), ysize(y), organisms(std::move(orgs)) { }

  static World make_world(int x, int y, int num_initial_organisms = 20) {
    auto place_random_initial_organism = [&]() {
      cout << x << y << endl;
    };
    place_random_initial_organism();
    std::map<Coord, Organism> m{};
    for (int i = 0; i < 2 * num_initial_organisms; i++) {
      m.try_emplace(Coord::random(x, y), Organism(0.2));
      if (m.size() >= num_initial_organisms)
        break;
    }
      
    return World(x, y, std::move(m));
  }

  void run_one_generation() {
    develop_into_givers();
    givers_spread_fecundity();
    //takers_reproduce();
    print();
  }

  void develop_into_givers() {
    for (Organism& o : std::views::values(organisms))
      o.develop_into_giver_or_taker();
  }

  void givers_spread_fecundity1() {
    for (auto& pair : organisms) {
      const Coord& c = pair.first;
      Organism& o = pair.second;
      if (o.is_giver) {
        for (Coord& d : c.delta_square(giving_radius)) {
          cout << "d = " << d << endl;
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
        cout << "d = " << d << endl;
        Coord recipient_coord = add_coords(c, d);
        if (organisms.count(recipient_coord) > 0) {
          organisms.at(recipient_coord).fecundity += 1.0;
          cout << recipient_coord << ' ' << organisms.at(recipient_coord).fecundity << endl;
        }
      }
    }
    for (auto& pair : givers()) {
      pair.get().second.fecundity = 0.0;
      cout << "giver at " << pair.get().first << " fecundity = " << pair.get().second.fecundity << endl;
    }
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

  /*
  std::vector<std::pair<Coord, Organism>&>
  givers() {
    std::vector<std::pair<Coord, Organism>&> result{}
    for (auto& pair : organisms)
      if (pair.second.is_giver)
        result.push_back(pair);
    return result;
  }
  */

  Coord add_coords(const Coord& c, const Coord& delta) {
    // NEXT Copy torus wrap-=around code from init-alt2.cc
    int x = c.x + delta.x;
    if (x >= 0)
      x = x % xsize;
    else
      x = xsize - abs(x) % xsize;
    int y = c.y + delta.y;
    if (y >= 0)
      y = y % ysize;
    else
      y = ysize - abs(y) % ysize;
    return Coord{x, y};
  }

  void print() {
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
    cout << organisms.size() << endl;
  }

  void print_fecundity() {
    for (int i = 0; i < xsize; i++) {
      for (int j = 0; j < ysize; j++) {
        Coord c{i, j};
        if (organisms.count(c) == 0)
          cout << "   - ";
        else
          cout << std::fixed << std::setw(4) << organisms.at(c).fecundity << ' ';
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

BOOST_AUTO_TEST_CASE(Coord_test) {
  Coord c{2, 3}, d{2, 4};

  //cout << c << endl;
  cout << Coord{6, 4} << endl;
  BOOST_CHECK(true);
  //bool a = Coord{2, 3} < Coord{2, 4};
  BOOST_CHECK_LT(c, d);


  std::map<Coord, std::string> m{};
  World w = World::make_world(20, 20);
  cout << Coord::random(20, 20) << ' ' << Coord::random(20, 20) << endl;
  w.run_one_generation();
  //w.print();
  cout << endl;
  w.print_fecundity();
}