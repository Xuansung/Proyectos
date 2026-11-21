SIMULACIÓN DE INCENDIOS EN CUDA

El objetivo es reproducir en la GPU un resultado similar al código del simulador CPU y medir por
separado los tiempos del cálculo del nuevo estado y el de las estadísticas. En simular.c, la
función main muestra el flujo esperado y el orden de las operaciones. Las funciones de referencia son
las que se deberán convertir en kernels: evolucionar y calcular_estadisticas.
Los tipos de datos y las rutinas auxiliares se encuentran en auxiliares.h y auxiliares.c.

EJECUCIÓN DEL CODIGO REFERENCIA EN CPU

La compilación de la referencia en CPU requiere Make y un compilador C11 en un entorno
POSIX como Linux, macOS o WSL. La ejecución CUDA requiere una GPU NVIDIA y un
toolkit compatible. El comparador utiliza Python 3.8 o posterior; el visor
requiere además matplotlib.

Desde la carpeta descomprimida, la referencia CPU se obtiene con:

    make
    ./simular ejemplos/paisaje.txt 120 17 run_cpu

Los argumentos indican, por orden, el fichero de escenario, el máximo de pasos, la semilla de
ignición y un directorio nuevo para las salidas. La cabecera HEX1 del escenario
contiene las columnas, filas, velocidad y dirección del viento. Cada línea posterior
describe una celda: el tipo de terreno, la humedad, la altitud y si es foco inicial.

IMPLEMENTACIÓN

La implementación CUDA se escribirá en simular_cuda.cu. La cabecera auxiliares.h
permite reutilizar las rutinas de lectura y escritura del código de referencia. La
siguiente orden compila y enlaza el programa para generar simular_cuda:

    make cuda

Si la implementación se realiza usando varios ficheros, estos se añaden a la
regla CUDA del Makefile.

El programa principal lee el escenario, inicializa las celdas, calcula los seis
factores de viento y asigna la memoria de dispositivo. Tras copiar los datos
necesarios, calcular las estadísticas iniciales y escribir el paso 0 inicial. En cada
transición se preparan los sorteos, se calcula la siguiente rejilla y se obtienen
sus estadísticas. Se recuperan los resultados, se escriben ambos CSV y se
intercambian las rejillas. La ejecución termina al alcanzarse el máximo de pasos o
al extinguirse el incendio. Deben comprobarse los errores de CUDA y liberar la
memoria asignada.

EVOLUCIÓN DE UNA CELDA

En una implementación inicial puede asignarse un hilo a cada celda. Cada hilo con
índice válido lee la rejilla actual y escribe el estado y la biomasa de su celda
en la rejilla siguiente, incluso cuando ambos se conservan. Una celda que arde
consume biomasa y pasa a QUEMADA cuando esta se agota. Una celda intacta puede
prender según su humedad, su terreno y la influencia de las vecinas que arden. Una
celda recién encendida conserva su biomasa y empieza a transmitir en la transición
siguiente.

Para reproducir el comportamiento del código de referencia se mantienen la doble
precisión, los coeficientes, las fórmulas y el orden de los seis vecinos.
La paridad de la fila determina las diagonales. En los bordes solo se deben consultar
vecinos válidos. El viento y la pendiente se evalúan desde la celda que arde hacia la que puede prender.

La referencia llama una vez a srand con la semilla de simulación. Antes de cada
transición generar en CPU un sorteo por celda, en orden de índice, incluso para las
celdas que no pueden prender. Para poder comparar ambas versiones se copia ese mismo
array a la GPU. Los seis factores de viento pueden calcularse una sola vez en CPU.

ESTADÍSTICAS POR PASO

En estadisticas.csv se registra una fila para el paso 0 y otra para cada paso
posterior, incluido el último. La columna paso identifica la instantánea. Los ocho
valores globales se calculan de nuevo en cada paso:

inertes, intactas, ardiendo y quemadas. Cuatro recuentos enteros, uno por cada
estado actual. Su suma debe coincidir con el número de celdas.

nuevas. Número de celdas que han pasado de INTACTA a ARDIENDO en la última
transición. En el paso 0 se cuentan los focos iniciales. Este recuento ya está
incluido en ardiendo.

biomasa_total. Suma de la biomasa restante de todas las celdas, incluida la
vegetación intacta.

intensidad_total. Suma de la biomasa restante de las celdas que arden.

intensidad_maxima. Mayor biomasa restante entre las celdas que arden en ese paso.
Vale cero si ninguna arde.

Las dos sumas y el máximo se calculan en doble precisión. La reducción debe
combinar las contribuciones de los hilos sin perder actualizaciones.

VALIDACIÓN Y VISUALIZACIÓN

Se utilizan el mismo escenario, el mismo máximo de pasos y la misma semilla en CPU
y GPU. Cada ejecución requiere un directorio de salida nuevo. Una vez compilado
simular_cuda, la comprobación del ejemplo anterior se realiza con:

    ./simular_cuda ejemplos/paisaje.txt 120 17 run_gpu
    python3 comparar.py run_cpu run_gpu
    python3 visualizar.py ejemplos/paisaje.txt run_gpu --paso 0

El visor utiliza el escenario y ambos CSV. El comparador comprueba evolucion.csv
(paso, celda, estado, biomasa e intensidad) y estadisticas.csv. Los estados y
conteos deben coincidir exactamente. Para los valores reales se acepta una
diferencia absoluta de hasta el mayor de estos dos valores: 1e-12 o 1e-10
multiplicado por la mayor magnitud de los dos valores comparados. El código de
salida es 0 si las salidas coinciden, 1 si se detecta una diferencia y 2 ante un
error de lectura.

El primer paso y la primera variable que señala el comparador ayudan a localizar
el fallo. Una ejecución con cero pasos permite comprobar la inicialización. El
escenario ejemplos/barrera.txt permite examinar los bordes y la extinción. Para
comprobar una rejilla mayor se dispone de ejemplos/paisaje-mediano.txt.

MEDICIÓN DE LOS KERNELS

Se registran eventos CUDA antes y después de la evolución y, por separado, de la
reducción completa de estadísticas. Si la reducción necesita varios kernels, la
medida debe abarcar todos ellos. La puesta a cero de los acumuladores, los sorteos, las
transferencias y la escritura de los CSV quedan fuera de ambos intervalos.

Para cada operación se indica el número de llamadas y el tiempo acumulado en
milisegundos. Con N transiciones se realizan N llamadas de evolución y N + 1
cálculos de estadísticas, incluido el paso inicial. Con cero transiciones solo se
mide el cálculo inicial de estadísticas.
