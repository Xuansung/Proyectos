#include "auxiliares.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sys/stat.h>

static const char *terrenos[] = {"AGUA","ROCA","HIERBA","MATORRAL","BOSQUE"};
static const char *estados[] = {"INERTE","INTACTA","ARDIENDO","QUEMADA"};

double uniforme(void) {
    return rand() / (RAND_MAX + 1.0);
}

void error(const char *mensaje) {
    fprintf(stderr, "%s\n", mensaje);
    exit(EXIT_FAILURE);
}

int entero(const char *texto, int minimo, int maximo) {
    char *fin;
    errno = 0;
    long valor = strtol(texto, &fin, 10);
    if (errno || fin == texto || *fin || valor < minimo || valor > maximo)
        error("entero fuera de rango");
    return (int)valor;
}

double real(const char *texto) {
    char *fin;
    double valor = strtod(texto, &fin);
    if (fin == texto || *fin || !isfinite(valor)) error("valor real incorrecto");
    return valor;
}

static void *asignar(int n, size_t tamano) {
    void *p = calloc(n, tamano);
    if (!p) error("memoria insuficiente");
    return p;
}

Escenario asignar_escenario(int columnas, int filas) {
    Escenario e = {0};
    e.columnas = columnas;
    e.filas = filas;
    e.n = columnas * filas;
    e.terreno = asignar(e.n, sizeof *e.terreno);
    e.foco = asignar(e.n, sizeof *e.foco);
    e.humedad = asignar(e.n, sizeof *e.humedad);
    e.altitud = asignar(e.n, sizeof *e.altitud);
    return e;
}

Rejilla asignar_rejilla(int n) {
    Rejilla r;
    r.estado = asignar(n, sizeof *r.estado);
    r.biomasa = asignar(n, sizeof *r.biomasa);
    return r;
}

void liberar_rejilla(Rejilla r) {
    free(r.estado);
    free(r.biomasa);
}

void liberar_escenario(Escenario e) {
    free(e.terreno);
    free(e.foco);
    free(e.humedad);
    free(e.altitud);
}

Escenario leer_escenario(const char *ruta) {
    FILE *f = fopen(ruta, "r");
    if (!f) error("no se puede abrir el escenario");
    char marca[16], columnas[32], filas[32];
    double velocidad, grados;
    if (fscanf(f, "%15s %31s %31s %lf %lf", marca, columnas, filas, &velocidad, &grados) != 5 ||
        strcmp(marca,"HEX1") || !isfinite(velocidad) || velocidad < 0 || !(grados >= 0 && grados < 360))
        error("cabecera HEX1 incorrecta");
    Escenario e = asignar_escenario(entero(columnas,1,10000), entero(filas,1,10000));
    e.velocidad = velocidad;
    e.grados = grados;
    for (int i = 0; i < e.n; ++i) {
        char nombre[32], foco[16];
        if (fscanf(f, "%31s %lf %lf %15s", nombre, &e.humedad[i], &e.altitud[i], foco) != 4 ||
            !(e.humedad[i] >= 0 && e.humedad[i] <= 1) || !isfinite(e.altitud[i]))
            error("datos de celda incorrectos");
        int tipo = 0;
        while (tipo < 5 && strcmp(nombre, terrenos[tipo])) ++tipo;
        e.foco[i] = entero(foco,0,1);
        if (tipo == 5 || (e.foco[i] && tipo < HIERBA)) error("terreno o foco incorrecto");
        e.terreno[i] = tipo;
    }
    if (fscanf(f,"%15s",marca) != EOF || ferror(f)) error("datos sobrantes o error de lectura");
    fclose(f);
    return e;
}

void escribir_escenario(FILE *f, const Escenario *e) {
    fprintf(f,"HEX1 %d %d %.17g %.17g\n",e->columnas,e->filas,e->velocidad,e->grados);
    for (int i = 0; i < e->n; ++i)
        fprintf(f,"%s %.17g %.17g %d\n",terrenos[e->terreno[i]],e->humedad[i],e->altitud[i],e->foco[i]);
    if (fflush(f) || ferror(f)) error("error al escribir el escenario");
}

static FILE *abrir_csv(const char *directorio, const char *nombre) {
    char *ruta = malloc(strlen(directorio) + strlen(nombre) + 2);
    if (!ruta) error("memoria insuficiente");
    sprintf(ruta,"%s/%s",directorio,nombre);
    FILE *f = fopen(ruta,"w");
    free(ruta);
    if (!f) error("no se puede crear el CSV");
    return f;
}

Salidas abrir_salidas(const char *directorio) {
    char *ruta = malloc(strlen(directorio) + 1);
    if (!ruta) error("memoria insuficiente");
    strcpy(ruta,directorio);
    size_t largo = strlen(ruta);
    while (largo > 1 && ruta[largo-1] == '/') ruta[--largo] = 0;
    for (size_t i = 1; i < largo; ++i) {
        if (ruta[i] != '/') continue;
        ruta[i] = 0;
        if (mkdir(ruta,0777) && errno != EEXIST) error("no se puede crear el directorio padre");
        ruta[i] = '/';
    }
    if (mkdir(ruta,0777)) error("se necesita un directorio de salida nuevo y accesible");
    free(ruta);
    Salidas s;
    s.evolucion = abrir_csv(directorio,"evolucion.csv");
    s.estadisticas = abrir_csv(directorio,"estadisticas.csv");
    fputs("paso,celda,estado,biomasa,intensidad\n",s.evolucion);
    fputs("paso,inertes,intactas,ardiendo,quemadas,nuevas,biomasa_total,intensidad_total,intensidad_maxima\n",s.estadisticas);
    return s;
}

void escribir_paso(Salidas s, int paso, int n, Rejilla r, Estadisticas resumen) {
    for (int i = 0; i < n; ++i) {
        double intensidad = r.estado[i] == ARDIENDO ? r.biomasa[i] : 0;
        fprintf(s.evolucion,"%d,%d,%s,%.17g,%.17g\n",paso,i,estados[r.estado[i]],r.biomasa[i],intensidad);
    }
    fprintf(s.estadisticas,"%d,%d,%d,%d,%d,%d,%.17g,%.17g,%.17g\n",paso,
            resumen.cuentas[INERTE],resumen.cuentas[INTACTA],resumen.cuentas[ARDIENDO],
            resumen.cuentas[QUEMADA],resumen.nuevas,resumen.biomasa_total,resumen.intensidad_total,resumen.intensidad_maxima);
    if (ferror(s.evolucion) || ferror(s.estadisticas)) error("error al escribir el paso");
}

void cerrar_salidas(Salidas s) {
    if (fclose(s.evolucion) || fclose(s.estadisticas)) error("error al cerrar los CSV");
}
