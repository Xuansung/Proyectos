#include <iostream>

void cross(int n, char c) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == n / 2 || j == n / 2) {
                std::cout << c; // Print the character in the cross
            } else {
                std::cout << " "; // Print space elsewhere
            }
        }
        std::cout << std::endl; // Move to the next line
    }
}
