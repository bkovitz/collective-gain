#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class Switch : public string { };
class Value : public string { };

istream& operator>>(istream& is, Switch& swtch) {
  int c;

  while ((c = is.get()) != EOF) {
    if (c != '-' && c != ' ') {
      swtch.push_back(c);
      break;
    }
  }
  
  while ((c = is.get()) != EOF) {
    if (c == '=')
      break;
    else
      swtch.push_back(c);
  }

  return is;
}

istream& operator>>(istream& is, Value& value) {
  int c;

  while ((c = is.get()) != EOF)
    value.push_back(c);
  
  return is;
}

int main() {
  Switch swtch;
  Value value;

  stringstream ss("--switch=value");
  ss >> swtch >> value;
  cout << swtch << endl << value << endl;
}
