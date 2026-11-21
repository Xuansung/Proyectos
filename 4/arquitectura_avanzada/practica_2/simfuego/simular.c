#include "auxiliares.h"
#include <math.h>
#include <stdlib.h>

static const double biomasa_inicial[] = {0, 0, .35, .65, 1};
static const double factor_ignicion[] = {0, 0, 1.40, 1.15, .90};
static const double factor_combustion[] = {0, 0, 1.40, 1, .70};

static double limitar_factor(double valor) {
    return fmin(1.5, fmax(.5, valor));
}

void inicializar(const Escenario *e, Rejilla actual) {
    for (int i = 0; i < e->n; ++i) {
        actual.biomasa[i] = biomasa_inicial[e->terreno[i]];
        actual.estado[i] = INTACTA;
        if (actual.biomasa[i] == 0) actual.estado[i] = INERTE;
        if (e->foco[i]) actual.estado[i] = ARDIENDO;
    }
}

void calcular_viento(const Escenario *e, double viento[6]) {
    for (int k = 0; k < 6; ++k) {
        double angulo = (60*k - e->grados) * acos(-1.) / 180;
        viento[k] = limitar_factor(1 + .05*e->velocidad*cos(angulo));
    }
}

static double influencia_vecinal(const Escenario *e, Rejilla actual,
                                 const double viento[6], int i) {
    int columna = i % e->columnas, fila = i / e->columnas;
    int impar = fila % 2;
    // Del destino a la fuente: E, NE, NO, O, SO, SE.
    int dx[6] = {1, impar, impar-1, -1, impar-1, impar};
    const int dy[6] = {0, 1, 1, 0, -1, -1};
    double suma = 0;
    for (int k = 0; k < 6; ++k) {
        int x = columna + dx[k], y = fila + dy[k];
        if (x < 0 || x >= e->columnas || y < 0 || y >= e->filas) continue;
        int j = y*e->columnas + x;
        if (actual.estado[j] != ARDIENDO) continue;
        double pendiente = limitar_factor(1 + e->altitud[i] - e->altitud[j]);
        int hacia_destino = (k + 3) % 6;
        suma += actual.biomasa[j] * viento[hacia_destino] * pendiente;
    }
    return suma;
}

void evolucionar(const Escenario *e, const double viento[6], const double sorteo[], Rejilla actual, Rejilla siguiente) {
    for (int i = 0; i < e->n; ++i) {
        int tipo = e->terreno[i];
        double biomasa = actual.biomasa[i];
        siguiente.estado[i] = actual.estado[i];
        siguiente.biomasa[i] = biomasa;

        if (actual.estado[i] == ARDIENDO) {
            double consumo = factor_combustion[tipo] * (.01 + .04*biomasa);
            siguiente.biomasa[i] = fmax(0, biomasa - consumo);
            if (siguiente.biomasa[i] == 0) siguiente.estado[i] = QUEMADA;
        } else if (actual.estado[i] == INTACTA) {
            double influencia = influencia_vecinal(e, actual, viento, i);
            double p = .20 * (1 - e->humedad[i]) * factor_ignicion[tipo] * influencia;
            if (sorteo[i] < fmin(1, p)) siguiente.estado[i] = ARDIENDO;
        }
    }
}

Estadisticas calcular_estadisticas(int n, Rejilla actual, Rejilla anterior, int paso) {
    Estadisticas s = {0};
    for (int i = 0; i < n; ++i) {
        ++s.cuentas[actual.estado[i]];
        s.biomasa_total += actual.biomasa[i];
        if (actual.estado[i] == ARDIENDO) {
            s.intensidad_total += actual.biomasa[i];
            s.intensidad_maxima = fmax(s.intensidad_maxima, actual.biomasa[i]);
            if (paso == 0 || anterior.estado[i] == INTACTA) ++s.nuevas;
        }
    }
    return s;
}

#ifndef SIN_MAIN
int main(int argc, char **argv) {
    if (argc != 5) error("uso: simular escenario.txt pasos semilla directorio_nuevo");
    Escenario escenario = leer_escenario(argv[1]);
    int pasos = entero(argv[2], 0, 1000000);
    int semilla = entero(argv[3], 0, 2147483647);
    srand(semilla);
    Salidas salidas = abrir_salidas(argv[4]);

    double viento[6];
    calcular_viento(&escenario, viento);
    Rejilla actual = asignar_rejilla(escenario.n);
    Rejilla siguiente = asignar_rejilla(escenario.n);
    double *sorteo = malloc(escenario.n * sizeof *sorteo);
    if (!sorteo) error("memoria insuficiente");
    inicializar(&escenario, actual);

    Estadisticas resumen = calcular_estadisticas(escenario.n, actual, actual, 0);
    escribir_paso(salidas, 0, escenario.n, actual, resumen);
    for (int paso = 1; paso <= pasos && resumen.cuentas[ARDIENDO] > 0; ++paso) {
        for (int i = 0; i < escenario.n; ++i) sorteo[i] = uniforme();
        evolucionar(&escenario, viento, sorteo, actual, siguiente);
        resumen = calcular_estadisticas(escenario.n, siguiente, actual, paso);
        escribir_paso(salidas, paso, escenario.n, siguiente, resumen);

        // Intercambiamos los punteros a los arrays.
        Rejilla temporal = actual;
        actual = siguiente;
        siguiente = temporal;
    }

    free(sorteo);
    cerrar_salidas(salidas);
    liberar_rejilla(actual);
    liberar_rejilla(siguiente);
    liberar_escenario(escenario);
    return 0;
}
#endif
