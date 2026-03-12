#include <iostream>
#include <string>

void ProductDigits(std::string& number) {
    bool first_iteration = true;
    while (first_iteration || std::stoi(number) > 9) {
        int product = 1;
        for (char digit : number) {
            product *= digit - '0';
        }
        std::cout << "The product of the digits of " << number << " is " << product << "." << std::endl;
        number = std::to_string(product);
        first_iteration = false;
    }
    std::cout << "----------" << std::endl;
}

int main() {
    std::string number;
    while (std::cin >> number) {
        ProductDigits(number);
    }
    return 0;
}
