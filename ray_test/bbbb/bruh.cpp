

#include <iostream>

using namespace std;

class num {
public:
  static int bruh;
  int value;
  num operator+(const num &) const;
  num() {
    value = 1;
    bruh = 15;
  };
  num(int v) { value = v; };
  static void print() { cout << 45 << endl; };
};

num num::operator+(const num &number) const {
  return (num(value + number.value));
}

int main() {
  num bruh;

  cout << bruh.value << endl;

  num bruh2;

  cout << bruh2.value << endl;

  num bruh3(bruh2 + bruh);

  cout << bruh3.value << endl;
  num::print();
  bruh.print();
}
