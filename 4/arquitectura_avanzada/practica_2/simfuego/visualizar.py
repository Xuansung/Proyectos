"""Dibuja una instantánea hexagonal y la historia de una ejecución HEX1."""
import argparse
import csv
import math
from pathlib import Path

import matplotlib
from matplotlib.collections import PolyCollection
from matplotlib.patches import Patch

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("escenario", type=Path)
parser.add_argument("run", type=Path)
parser.add_argument("--paso", type=int, help="por defecto, último paso registrado")
parser.add_argument("--salida", type=Path, help="guardar imagen en lugar de abrir ventana")
args = parser.parse_args()
if args.salida:
    matplotlib.use("Agg")
import matplotlib.pyplot as plt

with args.escenario.open() as f:
    marca, ancho, alto, velocidad, grados = f.readline().split()
    terreno = [linea.split()[0] for linea in f if linea.strip()]
ancho, alto = int(ancho), int(alto)
if marca != "HEX1" or len(terreno) != ancho * alto:
    parser.error("escenario incompleto")
with (args.run / "estadisticas.csv").open() as f:
    historia = list(csv.DictReader(f))
pasos = [int(fila["paso"]) for fila in historia]
paso = pasos[-1] if args.paso is None else args.paso
if paso not in pasos:
    parser.error(f"paso fuera del intervalo 0 a {pasos[-1]}")
with (args.run / "evolucion.csv").open() as f:
    foto = []
    for fila in csv.DictReader(f):
        t = int(fila["paso"])
        if t > paso:
            break
        if t == paso:
            foto.append(fila)
if [int(f["celda"]) for f in foto] != list(range(len(terreno))):
    parser.error("instantánea incompleta o dimensiones incompatibles")

colores = dict(AGUA="#519bc0", ROCA="#98948e", HIERBA="#d5cf75",
               MATORRAL="#90a85d", BOSQUE="#486e48")
poligonos = []
tonos = []
for i, fila in enumerate(foto):
    y, x = divmod(i, ancho)
    cx, cy = x + .5*(y % 2), y*math.sqrt(3)/2
    poligonos.append([(cx + math.cos(math.radians(30+60*k))/math.sqrt(3),
                       cy + math.sin(math.radians(30+60*k))/math.sqrt(3)) for k in range(6)])
    color = colores[terreno[i]]
    if fila["estado"] == "ARDIENDO":
        color = plt.cm.autumn(float(fila["intensidad"])*.8)
    elif fila["estado"] == "QUEMADA":
        color = "#393332"
    tonos.append(color)

fig, (mapa, curva) = plt.subplots(1, 2, figsize=(12, 5.5), constrained_layout=True)
mapa.add_collection(PolyCollection(poligonos, facecolors=tonos, edgecolors="none"))
mapa.autoscale_view()
mapa.set_aspect("equal")
mapa.axis("off")
mapa.set_title(f"Paso {paso} · viento {float(velocidad):g} m/s hacia {float(grados):g}°")
leyenda = {**colores, "ARDIENDO": "#ff6900", "QUEMADA": "#393332"}
mapa.legend(handles=[Patch(color=c, label=k.capitalize()) for k,c in leyenda.items()],
            loc="upper center", bbox_to_anchor=(.5, 0), ncol=3, frameon=False)
for clave, color in [("ardiendo","#d75217"), ("quemadas","#393332"), ("nuevas","#a27214")]:
    curva.plot(pasos, [int(f[clave]) for f in historia], label=clave.capitalize(), color=color)
curva.axvline(paso, linestyle=":", color="gray")
curva.set(xlabel="Paso de simulación", ylabel="Celdas", title="Evolución del incendio", ylim=(0,None))
if "intensidad_maxima" in historia[0]:
    eje_intensidad = curva.twinx()
    eje_intensidad.plot(pasos, [float(f["intensidad_maxima"]) for f in historia],
                        color="#5d4384", linestyle="--", label="Intensidad máxima")
    eje_intensidad.set_ylabel("Intensidad máxima", color="#5d4384")
    eje_intensidad.set_ylim(0, 1.05)
curva.legend(frameon=False)
if args.salida:
    fig.savefig(args.salida, dpi=150)
else:
    plt.show()
