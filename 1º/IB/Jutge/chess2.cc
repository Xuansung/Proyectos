#include <iostream>
#include <vector>
#include <string>
using namespace std;

int total_white_coins() {
    // Read the dimensions of the chessboard
    int r, c;
    cin >> r >> c;

    // Initialize the total sum for white squares
    int total_coins = 0;

    // Process each row
    for (int i = 0; i < r; i++) {
        string row;
        cin >> row;
        for (int j = 0; j < c; j++) {
            // A square is white if the sum of its indices is even
            if ((i + j) % 2 == 0) {
                total_coins += row[j] - '0'; // Convert char to int
            }
        }
    }

    // Return the result
    return total_coins;
}

int main() {
    cout << total_white_coins() << endl;
    return 0;
}
