#include <iostream>

using std::ostream;
using std::cout;
using std::endl;

struct Parameters {
  int i = 100;
  double d = 200.0;

  Parameters(int argc, char** argv) {
    while (*argv) {
      
    }
  }
};

ostream& operator<<(ostream& os, const Parameters& p) {
  os << "i = " << p.i << endl;
  os << "d = " << p.d << endl;
  return os;
}

void
run(const Parameters& p) {
  cout << p << endl;
}

int
main(int argc, char** argv) {
  Parameters p;

  for (p.i = 1; p.i <= 3; p.i++)
    for (p.d = -1.0; p.d <= 1.0; p.d += 0.5)
      cout << p << endl;
}
