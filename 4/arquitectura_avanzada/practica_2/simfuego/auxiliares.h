#ifndef AUXILIARES_H
#define AUXILIARES_H
enum Terreno { AGUA, ROCA, HIERBA, MATORRAL, BOSQUE };
enum Estado { INERTE, INTACTA, ARDIENDO, QUEMADA };

typedef struct {
    int columnas, filas, n;
    double velocidad, grados;
    unsigned char *terreno, *foco;
    double *humedad, *altitud;
} Escenario;

typedef struct {
    unsigned char *estado;
    double *biomasa;
} Rejilla;

typedef struct {
    int cuentas[4], nuevas;
    double biomasa_total, intensidad_total, intensidad_maxima;
} Estadisticas;


#include <stdio.h>

typedef struct { FILE *evolucion, *estadisticas; } Salidas;
#ifdef __cplusplus
extern "C" {
#endif
double uniforme(void);
void error(const char *mensaje);
int entero(const char *texto, int minimo, int maximo);
double real(const char *texto);
Escenario asignar_escenario(int columnas, int filas);
Rejilla asignar_rejilla(int n);
void liberar_escenario(Escenario e);
void liberar_rejilla(Rejilla r);
Escenario leer_escenario(const char *ruta);
void escribir_escenario(FILE *f, const Escenario *e);
Salidas abrir_salidas(const char *directorio);
void escribir_paso(Salidas s, int paso, int n, Rejilla r, Estadisticas resumen);
void cerrar_salidas(Salidas s);
#ifdef __cplusplus
}
#endif
#endif
