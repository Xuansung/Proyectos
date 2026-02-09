#include <iostream>
#include <vector>
#include <string>
#include <conio.h>  // Para _getch() y _kbhit()
#include <windows.h> // Para Sleep() y manejo del cursor

using namespace std;

// Definición de constantes y direcciones
const int ANCHO = 20;
const int ALTO = 20; // Ajustado al tamaño del mapa
bool gameOver = false;
int score = 0;

// Coordenadas del jugador y enemigos
int x, y; // Pacman
int fantasmasX[3], fantasmasY[3]; // 3 Fantasmas

// Mapa del juego
// '#' = Pared, '.' = Comida, ' ' = Vacío, 'O' = Pacman, '&' = Fantasma
vector<string> mapa = {
    "###################",
    "#........#........#",
    "#.##.###.#.###.##.#",
    "#.................#",
    "#.##.#.#####.#.##.#",
    "#....#...#...#....#",
    "####.### # ###.####",
    "   #.#       #.#   ",
    "####.# ## ## #.####",
    ".......#   #.......",
    "####.# ##### #.####",
    "   #.#       #.#   ",
    "####.# ##### #.####",
    "#........#........#",
    "#.##.###.#.###.##.#",
    "#..#.....O.....#..#",
    "##.#.#.#####.#.#.##",
    "#....#.......#....#",
    "###################"
};

// Función para mover el cursor de la consola (evita el parpadeo de system("cls"))
void gotoxy(int x, int y) {
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}

// Ocultar el cursor parpadeante de la consola
void ocultarCursor() {
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 2;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}

void setup() {
    ocultarCursor();
    // Buscar la posición inicial del Pacman 'O'
    for (int i = 0; i < mapa.size(); i++) {
        for (int j = 0; j < mapa[i].size(); j++) {
            if (mapa[i][j] == 'O') {
                x = j;
                y = i;
                mapa[i][j] = ' '; // Borramos la O del mapa estático
            }
        }
    }

    // Posiciones iniciales de los fantasmas (centro del mapa aprox)
    fantasmasX[0] = 9; fantasmasY[0] = 8;
    fantasmasX[1] = 9; fantasmasY[1] = 9;
    fantasmasX[2] = 9; fantasmasY[2] = 10;
}

void dibujar() {
    // Solo dibujamos "sobre" lo que ya existe para eficiencia, pero aquí redibujamos todo
    // Usamos gotoxy(0,0) para sobrescribir en lugar de limpiar pantalla
    gotoxy(0, 0); 
    
    for (int i = 0; i < mapa.size(); i++) {
        for (int j = 0; j < mapa[i].size(); j++) {
            bool dibujado = false;

            // Dibujar Pacman
            if (j == x && i == y) {
                cout << "C"; // Personaje Pacman
                dibujado = true;
            }
            else {
                // Dibujar Fantasmas
                for (int k = 0; k < 3; k++) {
                    if (j == fantasmasX[k] && i == fantasmasY[k]) {
                        cout << "&"; // Fantasma
                        dibujado = true;
                        break;
                    }
                }
            }

            // Si no es ni pacman ni fantasma, dibujar el mapa
            if (!dibujado) {
                cout << mapa[i][j];
            }
        }
        cout << endl;
    }
    cout << "Puntaje: " << score << endl;
    cout << "Controles: W, A, S, D para mover. X para salir." << endl;
}

void input() {
    if (_kbhit()) { // Si se presiona una tecla
        char tecla = _getch(); // Obtener tecla
        int nextX = x;
        int nextY = y;

        switch (tecla) {
            case 'w': nextY--; break;
            case 's': nextY++; break;
            case 'a': nextX--; break;
            case 'd': nextX++; break;
            case 'x': gameOver = true; break;
        }

        // Colisiones con paredes
        if (mapa[nextY][nextX] != '#') {
            x = nextX;
            y = nextY;
            
            // Comer puntos
            if (mapa[y][x] == '.') {
                score += 10;
                mapa[y][x] = ' '; // Dejar vacío
            }
        }
    }
}

void logica() {
    // Lógica de los fantasmas (IA muy simple: movimiento aleatorio)
    for (int k = 0; k < 3; k++) {
        int dir = rand() % 4; // 0-3
        int fX = fantasmasX[k];
        int fY = fantasmasY[k];

        if (dir == 0) fY--; // Arriba
        else if (dir == 1) fY++; // Abajo
        else if (dir == 2) fX--; // Izquierda
        else if (dir == 3) fX++; // Derecha

        // Si no choca con pared, se mueve
        if (mapa[fY][fX] != '#') {
            fantasmasX[k] = fX;
            fantasmasY[k] = fY;
        }

        // Colisión Fantasma con Pacman
        if (fantasmasX[k] == x && fantasmasY[k] == y) {
            gameOver = true;
        }
    }
    
    // Victoria (si no quedan puntos)
    // Para simplificar, omitimos el chequeo exhaustivo de victoria en cada frame
}

int main() {
    setup();
    
    while (!gameOver) {
        dibujar();
        input();
        logica();
        Sleep(100); // Controla la velocidad del juego (ms)
    }

    gotoxy(0, 22);
    cout << "GAME OVER! Tu puntaje final fue: " << score << endl;
    system("pause");
    return 0;
}