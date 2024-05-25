/*
 * x-getopt.cc -- Experiment to try out getopt in C++
 */

#include <unistd.h>
#include <iostream>
#include <getopt.h>
#include <stdlib.h>

using std::cout, std::endl;

extern char* optarg;  // for getopt()

int main(int argc, char *argv[]) {
  while (true) {
    static struct option options[] = {
      {"gain", required_argument, 0, 'g'}
    };

    int c = getopt_long(argc, argv, "g:", options, NULL);
    if (c == -1)
      break;

    switch (c) {
    case 'g':
      double d = atof(optarg);
      cout << "gain: " << d << endl;
    }
  }
}
