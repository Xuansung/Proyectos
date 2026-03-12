#include <iostream>
#include <string>

void printRhombus(int n) {
    // Upper part of the rhombus
    for (int i = 1; i <= n; ++i) {
        std::string spaces(n - i, ' ');
        std::string stars(2 * i - 1, '*');
        std::cout << spaces << stars << std::endl;
    }

    // Lower part of the rhombus
    for (int i = n - 1; i >= 1; --i) {
        std::string spaces(n - i, ' ');
        std::string stars(2 * i - 1, '*');
        std::cout << spaces << stars << std::endl;
    }
}

int main() {
    int n;
    std::cin >> n;

    if (n >= 1) {
        printRhombus(n);
    } else {
        std::cout << "Input must be a natural number (n >= 1)." << std::endl;
    }

    return 0;
}
