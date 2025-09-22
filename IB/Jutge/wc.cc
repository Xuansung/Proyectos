#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    // Verificar si se proporciona el nombre del archivo
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]); // Abrir el archivo en modo lectura

    if (!file) {
        std::cerr << "Error: No se pudo abrir el archivo " << argv[1] << std::endl;
        return 1;
    }

    int num_lineas = 0, num_palabras = 0, num_caracteres = 0;
    std::string linea, palabra;

    // Contar líneas, palabras y caracteres
    while (std::getline(file, linea)) {
        num_lineas++;                      // Contar línea
        num_caracteres += linea.size() + 1; // Contar caracteres (+1 para '\n')
        
    }

    file.close(); // Cerrar el archivo

    // Mostrar el resultado (similar a `wc`)
    std::cout << num_lineas << " " << num_palabras << " " << num_caracteres << " " << argv[1] << std::endl;

    return 0;
}
