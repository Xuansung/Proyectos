#include <iostream>
#include <vector>
#include <algorithm> // Para usar std::max

using namespace std;

int mochila_1_vector(int W, const vector<int>& pesos, const vector<int>& valores, int n) {
    // Creamos un vector de tamaño W + 1 inicializado a 0
    vector<int> dp(W + 1, 0);
    
    // Iteramos sobre cada elemento
    for (int i = 0; i < n; i++) {
        // Iteramos sobre los pesos de ATRÁS hacia ADELANTE
        // w >= pesos[i] asegura que no intentemos meter un objeto si su peso es mayor a la capacidad w
        for (int w = W; w >= pesos[i]; w--) {
            dp[w] = max(dp[w], dp[w - pesos[i]] + valores[i]);
        }
    }
    
    // El resultado final estará en la última posición
    return dp[W];
}

int main() {
    vector<int> valores = {60, 100, 120};
    vector<int> pesos = {10, 20, 30};
    int W = 50;
    int n = valores.size();
    int resultado = mochila_1_vector(W, pesos, valores, n);
    cout << "El valor maximo que se puede llevar es: " << resultado << endl;

    return 0;
}
