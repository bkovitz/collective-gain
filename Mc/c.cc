#include "random.h"
#include <cmath>
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;

constexpr double pi() { return std::atan(1)*4; }

int main() {
  RandomUniformInt coin_flip(0, 1);
  RandomUniformReal unit_interval_random(0.0, 1.0);
  RandomPoisson poisson(2.34);
  RandomNormal normal(0.0, 0.02);
  RandomTruncatedNormal truncated_normal(0.0, 2.0);

  for (int i = 0; i < 20; i++)
    cout << coin_flip() << ' ';
  cout << endl;

  for (int i = 0; i < 20; i++)
    cout << unit_interval_random() << ' ';
  cout << endl;

  for (int i = 0; i < 20; i++)
    cout << poisson() << ' ';
  cout << endl;

  for (int i = 0; i < 20; i++)
    cout << normal() << ' ';
  cout << endl;

  for (int i = 0; i < 20; i++)
    cout << random_normal(10, 1) << ' ';
  cout << endl;

  for (int i = 0; i < 20; i++)
    cout << truncated_normal() << ' ';
  cout << endl;
}

