#include <iostream>
#include <iomanip>

void ArmonicNumber(const int n, const int m) {
    double resultado = 0.0;
    for (int i = m + 1; i <= n; i++) {
        resultado += 1.0 / i;
    }
    std::cout << std::fixed << std::setprecision(10) << resultado << std::endl;
}

int main() {
    int n, m;
    while (std::cin >> n >> m) {
        if (n >= m) {
            ArmonicNumber(n, m);
        }
    }
    return 0;
}
