#include <vector>
#include <algorithm>
bool exists(int x, const std::vector<int>& v) {
  for ( int i = 0; i < v.size(); i++) {
    if ( v[i] == x ) {
      return true;
    }
  }
  return false;
}