#include <iostream>

using std::cout;
using std::endl;

struct Coord {
   int x, y;
};

int main() {
   Coord c{2, 3};

   cout << c.x << ' ' << c.y << endl;
}
