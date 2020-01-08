#include <iostream>
#include <string>
using namespace std;
extern "C" {
int cm(int n) {
  int cm = 0;
  int i;
  for (i = 1; i <= n; ++i) {
    cm += i;
  }
  return (cm);
}
}