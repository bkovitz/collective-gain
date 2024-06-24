#include <random>
#include "random.h"

namespace randoms
{
  unsigned int seed;  // global random-number seed
  bool need_seed = true;  // does 'seed' need to be initialized?
  std::random_device random_device;
  std::mt19937 rng;   // global random-number sequence

  void reseed()
  {
    seed = random_device();
    rng.seed(seed);
    need_seed = false;
  }

  void reseed(unsigned int _seed)
  {
    seed = _seed;
    rng.seed(seed);
    need_seed = false;
  }
}
