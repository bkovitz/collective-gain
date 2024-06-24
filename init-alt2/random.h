#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

namespace randoms {
  extern unsigned int seed;
  extern bool need_seed;
  extern std::mt19937 rng;
  extern void reseed();
  extern void reseed(unsigned int);

  template <template <class> class DistributionType, typename OutputType>
  class Random {
  protected:
    DistributionType<OutputType> distribution;

  public:
    template <typename ...Args>
    Random(Args&& ...args) : distribution(std::forward<Args>(args)...) { }

    OutputType operator()() { return distribution(randoms::rng); }
  };
}

using RandomUniformInt =
          randoms::Random<std::uniform_int_distribution, int>;
using RandomUniformReal =
          randoms::Random<std::uniform_real_distribution, double>;
using RandomPoisson =
          randoms::Random<std::poisson_distribution, int>;
using RandomNormal =
          randoms::Random<std::normal_distribution, double>;

inline double random_normal(double mean, double standard_deviation) {
  static RandomNormal normal(mean, standard_deviation);
  return normal();
}

inline int random_poisson(double mean) {
  static RandomPoisson poisson(mean);
  return poisson();
}

#endif // RANDOM_H_INCLUDED
