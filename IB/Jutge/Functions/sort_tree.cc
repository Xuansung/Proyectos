#include <vector>
#include <algorithm>

void sort3(int& a, int& b, int& c) {
  std::vector<int> Vector;
    Vector.push_back(a);
    Vector.push_back(b);
    Vector.push_back(c);
    std::sort (Vector.begin(), Vector.end());
    a = Vector[0];
    b = Vector[1];
    c = Vector[2];
}