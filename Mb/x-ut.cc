/*
 * x-ut.cc -- Experiment to try out Boost unit testing
 *
 * Lesson learned: need -lboost_unit_test_framework in compiler command line.
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>

int add(int i, int j) {
  return i + j;
}

BOOST_AUTO_TEST_CASE(universeInOrder) {
  BOOST_CHECK(add(2, 2) == 5);
}
