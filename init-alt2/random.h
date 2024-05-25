#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <iostream>

#include <random>
#include <chrono>
#include <utility>

class TimeSeedSeq {
   std::default_random_engine rng;

public:
   TimeSeedSeq() :
      rng(std::chrono::system_clock::now().time_since_epoch().count())
   { rng.discard(1); }

   unsigned next_seed() { return rng(); }
};

extern TimeSeedSeq seeder;

template <template <class> class DistributionType, typename OutputType>
class Random {
protected:
  std::default_random_engine rng;
  DistributionType<OutputType> distribution;

public:
  template <typename ...Args>
  Random(Args&& ...args)
  : rng(seeder.next_seed()),
    distribution(std::forward<Args>(args)...) { }

  OutputType operator()() { return distribution(rng); }
};

using RandomUniformInt =
          Random<std::uniform_int_distribution, int>;
using RandomUniformReal =
          Random<std::uniform_real_distribution, double>;
using RandomPoisson =
          Random<std::poisson_distribution, int>;
using RandomNormal =
          Random<std::normal_distribution, double>;

class RandomTruncatedNormal : protected RandomNormal {
protected:
  double mean;
public:
  RandomTruncatedNormal(double mean_, double standard_deviation) :
      RandomNormal(0.0, standard_deviation),
      mean(mean_) { }

  int operator()() { return (int)(mean + (int)distribution(rng) + 0.5); }
};

inline double random_normal(double mean, double standard_deviation) {
  RandomNormal normal(mean, standard_deviation);
  return normal();
}

inline int random_poisson(double mean) {
  RandomPoisson poisson(mean);
  return poisson();
}

#endif // RANDOM_H_INCLUDED
