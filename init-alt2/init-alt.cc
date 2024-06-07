/*
 * init-alt.cc -- "Givers & Takers" evolutionary simulation
 *
 * Tests whether a gene that affects the probabilities of developing into
 * two different phenotypes, a "taker" that reproduces and a "giver" that
 * doesn't reproduce but contributes all its resources into unconditional aid
 * to nearby organisms, can evolve in a way that favors altruism without
 * first needing to evolve a gene for kinship selection--hence the name
 * "initial altruism" (altruism evolving before anything else).
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE InitAlt
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>

using std::cout;
using std::endl;

typedef unsigned int uint;

double
distance_from_origin(double dx, double dy) {
  return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

/*
 * Torus2D: like a 2-dimensional array, except coordinates wrap around.
 */

template <typename T>
class Torus2D {
  const uint xsize, ysize;
  T* array;
public:
  Torus2D(uint x, uint y) : xsize(x), ysize(y), array(new T[xsize * ysize]) { }
  ~Torus2D() { delete[] array; }
  T& operator()(int x, int y) {
    uint wrapped_x, wrapped_y;
    if (x >= 0)
      wrapped_x = x % xsize;
    else {
      wrapped_x = xsize - abs(x) % xsize;
    }
    if (y >= 0)
      wrapped_y = y % ysize;
    else {
      wrapped_y = ysize - abs(y) % ysize;
    }
    return array[wrapped_x * ysize + wrapped_y];
  }

  void set_all(T v) {
    for (int x = 0; x < xsize; x++)
      for (int y = 0; y < ysize; y++)
        (*this)(x, y) = v;
  }

  void print() {
    cout << std::setprecision(3) << std::setw(4) << std::fixed;
    for (int y = 0; y < ysize; y++) {
      for (int x = 0; x < xsize; x++)
        cout << (*this)(x, y) << ' ';
      cout << endl;
    }
  }

};

template <typename T>
class Array2D {
  const int xsize, ysize;
  T* array;
public:
  Array2D(int x, int y) : xsize(x), ysize(y), array(new T[xsize * ysize]) { }
  ~Array2D() { delete[] array; }
  T& operator()(int x, int y) { return array[x * xsize + y]; }

  void print() {
    cout << std::setprecision(3) << std::setw(4) << std::fixed;
    for (int y = 0; y < ysize; y++) {
      for (int x = 0; x < xsize; x++)
        cout << (*this)(x, y) << ' ';
      cout << endl;
    }
  }

  void scale_to(double desired_sum) {
    double scale_factor = desired_sum / sum();
    for (int x = 0; x < xsize; x++)
      for (int y = 0; y < ysize; y++)
        (*this)(x, y) *= scale_factor;
  }
  
  double sum() {
    double result = 0.0;
    for (int x = 0; x < xsize; x++)
      for (int y = 0; y < ysize; y++)
        result += (*this)(x, y);
    return result;
  }
};

class RadialDecayTable {
  const int radius;
  Array2D<double> table; // table(dx, dy) = amount after decay at dx, dy
public:
  RadialDecayTable(uint r, double sum)
  :
    radius(r),
    table(radius * 2 + 1, radius * 2 + 1)
  {
    for (int dx = -radius; dx <= radius; dx++)
      for (int dy = -radius; dy <= radius; dy++)
        table(dx + radius, dy + radius) =
            std::max(radius * radius - distance_from_origin(dx, dy), 0.0);
    table.scale_to(sum);
    //table.print();
  }

  void apply(Torus2D<double>& torus, int x, int y) {
    for (int dx = -radius; dx <= radius; dx++)
      for (int dy = -radius; dy <= radius; dy++) {
        torus(x + dx, y + dy) += table(dx + radius, dy + radius);
        // cout << x + dx << ", " << y + dy << ": " << table(dx + radius, dy + radius) << endl;
      }
  }
};

/*
int main(int argc, char *argv[]) {
}
*/

BOOST_AUTO_TEST_CASE(Torus2D_test) {
  Torus2D<int> t(3, 3);

  t(0, 0) = 22;
  BOOST_CHECK_EQUAL(t(0, 0), 22);
  BOOST_CHECK_EQUAL(t(3, 3), 22);  // test wrap-around

  t.set_all(1);
  BOOST_CHECK_EQUAL(t(0, 0), 1);
  BOOST_CHECK_EQUAL(t(2, 1), 1);

  Torus2D<double> u(10, 10);
  u.set_all(1.0);
  u(0, -1) += 0.038;
  BOOST_CHECK_EQUAL(u(0, 9), 1.038);
}

BOOST_AUTO_TEST_CASE(Array2D_test) {
  Array2D<double> a(2, 2);

  a(0, 0) = 1.0; a(1, 0) = 2.0;
  a(0, 1) = 3.0; a(1, 1) = 4.0;

  BOOST_CHECK_EQUAL(a(0, 1), 3.0);
  a.scale_to(1.0);
  BOOST_CHECK_CLOSE(a(0, 1), 0.3, 0.0001);
  BOOST_CHECK_CLOSE(a.sum(), 1.0, 0.0001);
}

BOOST_AUTO_TEST_CASE(RadialDecayTable_test) {
  Torus2D<double> t(10, 10);
  t.set_all(1);

  RadialDecayTable radial(2, 1.0);
  radial.apply(t, 0, 1);
  BOOST_CHECK_CLOSE(t(0, 1), 1.075, 0.1);
  BOOST_CHECK_CLOSE(t(0, 9), 1.038, 0.1);
}
