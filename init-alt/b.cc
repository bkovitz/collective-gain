#include <iostream>
#include <random>
#include <chrono>
#include <functional>

using std::cout;
using std::endl;

class TimeSeedSeq {
   std::default_random_engine rng;

public:
   TimeSeedSeq() :
      rng(std::chrono::system_clock::now().time_since_epoch().count())
   { rng.discard(1); }

   unsigned next_seed() { return rng(); }
};

class RandomUniformInt {
   static TimeSeedSeq seeder;
   std::default_random_engine rng;
   std::uniform_int_distribution<int> distribution;

public:
   RandomUniformInt(int lb, int ub) :
      //rng(time_seed()),
      rng(seeder.next_seed()),
      distribution(lb, ub)
   {
      //distribution(rng); // throw away first value, because it's predictable
   }

   int operator()() { return distribution(rng); }

protected:
   unsigned time_seed() {
      return std::chrono::system_clock::now().time_since_epoch().count() >> 1;
   }
};

TimeSeedSeq
RandomUniformInt::seeder;

int main() {
   //std::default_random_engine rng;
   //std::uniform_int_distribution<int> random_x(0, 20);
   //auto random_x = std::bind(std::uniform_int_distribution<int>(0, 20), rng);
   RandomUniformInt random_x(0, 20);
   RandomUniformInt random_y(0, 20);

   for (int i = 0; i < 10; i++) {
      cout << random_x() << ' ' << random_y() << "  ";
   }
   cout << endl;

   TimeSeedSeq seeder;
   for (int i = 0; i < 10; i++)
      cout << seeder.next_seed() << ' ';
   cout << endl;
}
