#include <vector>

double scalar_product(const std::vector<double>& u, const std::vector<double>& v) {
  double product = 0.0;
  for (int i = 0; i < u.size() - 1; i++) {
    product += u[i] * v[i];
  }
  return product;
}