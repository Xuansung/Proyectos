/*Funcionalidad: Programa principal para bignumber
 * Fecha: 09/03/2025
 * Autor: Xuan Sun
*/

// Librerías
#include <fstream>
#include <sstream>
#include <stack>

// Cabeceras de los tipos BigNumber
#include "BigNumber.h"
#include "BigUnsigned.h"
#include "BigInteger.h"
#include "BigRational.h"

// Usamos un vector de pares para almacenar las etiquetas y sus punteros:
template <unsigned char Base>
using BoardEntry = std::pair<std::string, BigNumber<Base>*>;
template <unsigned char Base>
using Board = std::vector<BoardEntry<Base>>;

// setValue: Inserta o actualiza en el Board la (etiqueta, puntero).
//Si la etiqueta ya existía, libera el viejo puntero antes de sobreescribirlo.
template <unsigned char Base>
void setValue(Board<Base>& board, const std::string& etiqueta, BigNumber<Base>* bignumber) {
  for (auto& [etiq, oldBignumber] : board) {
    if (etiq == etiqueta) {
      delete oldBignumber;    // liberar el viejo
      oldBignumber = bignumber;     // sobreescribir
      return;
    }
  }
  // Si no se encontró la etiqueta, la añadimos
  board.emplace_back(etiqueta, bignumber);
}

// getValue: Devuelve el puntero al BigNumber asociado a la etiqueta.
//Lanza excepción si no existe.
template <unsigned char Base>
BigNumber<Base>* getValue(const Board<Base>& board, const std::string& etiqueta) {
  for (auto& [etiq, bignumber] : board) {
    if (etiq == etiqueta) {
      return bignumber;
    }
  }
  throw std::runtime_error("Etiqueta no encontrada: " + etiqueta);
}

// existsValue: Indica si la etiqueta está en el Board.
template <unsigned char Base>
bool existsValue(const Board<Base>& board, const std::string& etiqueta) {
  for (auto& [etiq, bignumber] : board) {
    if (etiq == etiqueta) return true;
  }
  return false;
}

//printBoard: Recorre el vector e imprime cada (etiqueta, valorConSufijo).
template <unsigned char Base>
void printBoard(const Board<Base>& board, std::ostream& out) {
  for (auto& [etiq, bignumber] : board) {
    out << etiq << " = ";
    // Detectamos el tipo real usando dynamic_cast
    if (dynamic_cast<BigRational<Base>*>(bignumber)) {
      out << *bignumber << "r\n";
    } else if (dynamic_cast<BigInteger<Base>*>(bignumber)) {
      out << *bignumber << "i\n";
    } else if (dynamic_cast<BigUnsigned<Base>*>(bignumber)) {
      out << *bignumber << "u\n";
    } else {
      out << *bignumber << "\n"; // Caso raro, sin sufijo
    }
  }
}
// Poder aplicar los operadores en la Notación Polaca Inversa
template<unsigned char Base>
void applyOperator(std::stack<BigNumber<Base>*>& pila, const std::string& operation) {
  if (pila.size() < 2) {
    throw std::runtime_error("Expresión RPN inválida (faltan operandos).");
  }
  // Pop (right, left) en ese orden
  BigNumber<Base>* right = pila.top(); 
    pila.pop();
  BigNumber<Base>* left  = pila.top(); 
    pila.pop();

  BigNumber<Base>* result = nullptr;
  if (operation == "+") 
    result = &left->add(*right);
  else if (operation == "-") 
    result = &left->subtract(*right);
  else if (operation == "*") 
    result = &left->multiply(*right);
  else if (operation == "/") 
    result = &left->divide(*right);
  else throw std::runtime_error("Operador desconocido: " + operation);
  // La operación polimórfica crea un objeto nuevo y lo retorna
  pila.push(result);
}
/**
 * EvaluateRPN: Evalúa una expresión en notación polaca inversa (RPN),
 * sustituyendo cada etiqueta por su puntero en el Board, y aplicando +, -, *, /.
 * Devuelve un puntero nuevo al resultado.
 */
template <unsigned char Base>
BigNumber<Base>* EvaluateRPN(const std::string& expresion, const Board<Base>& board) {
  std::stringstream ss(expresion);
  std::stack<BigNumber<Base>*> pila;
  std::string simbolo;

  // Separa la expresion y procesa operandos u operadores
  while (ss >> simbolo) {
    if (simbolo == "+" || simbolo == "-" || simbolo == "*" || simbolo == "/") {
      applyOperator<Base>(pila, simbolo);
    } else {
      // Debería ser una etiqueta
      if (!existsValue<Base>(board, simbolo)) {
        throw std::runtime_error("Etiqueta no encontrada en RPN: " + simbolo);
      }
      pila.push(getValue<Base>(board, simbolo));
    }
  }

  if (pila.size() != 1) {
    throw std::runtime_error("Expresión RPN inválida (pila final != 1).");
  }
  return pila.top(); // Puntero resultante
}

/**
 * runCalculator: 
 *  Lee línea a línea tras la base, interpretando:
 *   - "etiqueta = númeroConSufijo"
 *   - "etiqueta ? expresiónRPN"
 *  y guarda resultados en un Board (vector de pairs).
 */
template <unsigned char Base>
void runCalculator(std::istream& in, std::ostream& out) {
  // Consumir lo que quede tras leer la base
  std::string dummy;
  std::getline(in,dummy);
  Board<Base> board;
  std::string line;
  // Leer línea a línea
  while (std::getline(in, line)) {
    if (line.empty()) continue;
    std::stringstream ss(line);
    std::string etiqueta, symbol;
    ss >> etiqueta >> symbol; // "N1" "=", "N2" "=", "E1" "?" etc.
    if (symbol == "=") {
      // "etiqueta = númeroConSufijo"
      std::string numStr;
      std::getline(ss, numStr);
      if (!numStr.empty() && numStr[0] == ' ') {
        numStr.erase(numStr.begin());
      }
      try {
        BigNumber<Base>* bignumber = BigNumber<Base>::create(numStr.c_str());
        setValue(board, etiqueta, bignumber);
      } catch (...) {
        // Si hay fallo, guardamos 0
        setValue(board, etiqueta, new BigUnsigned<Base>(0u));
      }
    }
    else if (symbol == "?") {
      // "etiqueta ? exprRPN"
      std::string expresion;
      std::getline(ss, expresion);
      if (!expresion.empty() && expresion[0] == ' ') {
        expresion.erase(expresion.begin());
      }
      try {
        BigNumber<Base>* result = EvaluateRPN<Base>(expresion, board);
        setValue(board, etiqueta, result);
      } catch (...) {
        setValue(board, etiqueta, new BigUnsigned<Base>(0u));
      }
    }
    else {
      // Línea con formato no reconocido, se ignora
    }
  }
  // Imprimir la base y luego cada etiqueta con su valor
  out << "Base = " << (int)Base << "\n";
  printBoard(board, out);

  // Limpiar memoria reservada
  for (auto& entry : board) {
    delete entry.second;
  }
}

/**
 * Función principal: 
 *  - Lee argv[1] y argv[2] para ficheros de entrada/salida,
 *  - Lee la base, llama a runCalculator<base>(),
 *  - Finaliza.
 */
int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Uso: " << argv[0] << " <fichero_entrada> <fichero_salida>\n";
    return 1;
  }

  std::ifstream fichero_in(argv[1]);
  if (!fichero_in) {
    std::cerr << "Error abriendo " << argv[1] << " para lectura.\n";
    return 1;
  }

  std::ofstream fichero_out(argv[2]);
  if (!fichero_out) {
    std::cerr << "Error abriendo " << argv[2] << " para escritura.\n";
    return 1;
  }

  int baseInt;
  fichero_in >> baseInt;
  switch (baseInt) {
    case 2:  runCalculator<2>(fichero_in, fichero_out);  break;
    case 8:  runCalculator<8>(fichero_in, fichero_out);  break;
    case 10: runCalculator<10>(fichero_in, fichero_out); break;
    case 16: runCalculator<16>(fichero_in, fichero_out); break;
    default:
      std::cerr << "Base no soportada (2, 8, 10 o 16).\n";
      return 1;
  }
  return 0;
}
