/*Programa que dado un número ingresado por teclado retorne sdu raíz cuadrada y el cuadrado.*/
#include <iostream>
#include <string>
#include <windows.h>
#include <locale.h>
#include <cmath>
using namespace std;
int main() {
  float num, raiz, cua;
  string seguir = "S";
  setlocale(LC_ALL, "es_ES.UTF-8");
  while (seguir == "S" || seguir == "s") {
    system("cls");
    cout << "\nPrograma que calcula la raíz y el cuadrado de un número!";
    cout << "\n\t------------------------------------------------------\n\n";
    cout << "\n\tDigite un numero:\t";
    cin >> num;
    system("color 0");
    if (num < 0 )
      cout << "\\tError: El numero no tiene raiz real\n";
    else
      raiz = sqrt(num);
    cua = pow(num,2);
    cout << "\n\n          Imprimir datos y resultados";
    cout << "\n\t------------------------------------------------------\n\n";
    cout << "\n El numero digitado es: \t\t" << num << endl << endl;
    if (num >= 0)
      cout << "\n La raiz cuadrada es: \t\t" << "+/- " << raiz << endl << endl;
    cout << "\n\t El cuadrado es: \t"<< cua << endl << endl;
    cout << "\n\t------------------------------------------------------\n\n\t";
    cout << "\n\t Desea seguir procesando S o N\t";
    cin >> seguir;
    while( seguir != "S" && seguir != "s" && seguir != "N" && seguir != "n") {
      cout << "\nError. Digite S o N";
      Sleep(500); cin >> seguir;
    }
  }
  cout << endl << endl;
  system("pause");
  return 0;
}