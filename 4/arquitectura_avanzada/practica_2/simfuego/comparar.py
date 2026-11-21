"""Compara las dos salidas CSV en orden de paso, sin cargar las rejillas en memoria."""
import argparse
import csv
from itertools import zip_longest
import math
from pathlib import Path
import sys

FORMATOS = {
    "evolucion.csv": "paso celda estado biomasa intensidad".split(),
    "estadisticas.csv": "paso inertes intactas ardiendo quemadas nuevas biomasa_total intensidad_total intensidad_maxima".split(),
}
REALES = {"biomasa", "intensidad", "biomasa_total", "intensidad_total", "intensidad_maxima"}

def diferencia(nombre, cpu, gpu, absoluta, relativa):
    campos = FORMATOS[nombre]
    with (cpu/nombre).open(newline="") as a, (gpu/nombre).open(newline="") as b:
        lectores = [csv.DictReader(a), csv.DictReader(b)]
        if any(r.fieldnames != campos for r in lectores):
            raise ValueError(f"{nombre}: cabecera inesperada")
        vistas = 0
        for x, y in zip_longest(*lectores):
            vistas += 1
            fila = x or y
            paso = int(fila["paso"])
            lugar = f"{nombre}, paso {paso}"
            if "celda" in fila:
                lugar += f", celda {fila['celda']}"
            if x is None or y is None:
                lado = "CPU" if x is None else "GPU"
                return paso, f"{lugar}: falta la fila en {lado}"
            for campo in campos:
                u, v = x[campo], y[campo]
                if campo in REALES:
                    igual = math.isclose(float(u), float(v), abs_tol=absoluta, rel_tol=relativa)
                elif campo == "estado":
                    igual = u == v
                else:
                    igual = int(u) == int(v)
                if not igual:
                    return paso, f"{lugar}, {campo}: CPU={u}, GPU={v}"
        if vistas == 0:
            raise ValueError(f"{nombre}: fichero vacío")

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("cpu", type=Path)
    p.add_argument("gpu", type=Path)
    p.add_argument("--abs", dest="absoluta", type=float, default=1e-12)
    p.add_argument("--rel", dest="relativa", type=float, default=1e-10)
    args = p.parse_args()
    try:
        fallos = [d for nombre in FORMATOS if
                  (d := diferencia(nombre,args.cpu,args.gpu,args.absoluta,args.relativa))]
    except (OSError, ValueError, TypeError, csv.Error) as exc:
        print(exc, file=sys.stderr)
        return 2
    if fallos:
        print(min(fallos, key=lambda d: d[0])[1])
        return 1
    print("Evolución y estadísticas coinciden dentro de las tolerancias.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
