from functools import reduce

Temperaturas = [22, 27, 19, 31, 25, 18, 30, 24, 28, 33, 21, 26, 17, 29, 35, 23, 25, 32, 20, 27, 16, 34, 28, 22,
31, 24, 26, 19, 30, 36, 21, 25, 29, 18, 33, 27, 23, 32, 20, 28]

# 1- Primera y última temperatura
Temperaturas[0]
Temperaturas[-1]
# 2- Recorriendo todas las temperaturas
for i in Temperaturas:
    print(i)

# 3- Clasificando cada valor
for i in Temperaturas:
    print(i)
    if i > 25:
        print(" alta")
    else:
        print (" baja")

# 4- Creando una nueva lista:
altas = []
for i in Temperaturas:
    if i > 25:
        altas.append(i)

print(altas)

# Alternativa
altas = [i for i in Temperaturas if i > 25]
print(altas)

# 5-Detectando una alerta
for i in Temperaturas:
    if i > 30:
        print("ALERTA ", i)
        break

# 6- Bonus
valores = input()
valores_separados = valores.split()
Temperaturas2 = []
for i in valores_separados:
    Temperaturas2.append(i)
print(Temperaturas2)

# 2.1 Funciones
def limites(temperaturas,limite = 25):
    count = 0
    for i in temperaturas:
        if i > limite:
            count += 1
    return count 

# 2.2 Modificación de listas
Temperaturas.append(24)
Temperaturas.insert(2,20)
# 2.3 Métodos de listas
Temperaturas_ordenadas = Temperaturas.sort(reverse=True)
num_25 = Temperaturas.count(25)

# 2.4 Filtrado y acumulación
def es_alta(temp):
    return temp > 25

Temperaturas3 = filter(es_alta,Temperaturas)
def sumar(a,b):
    return a + b

temp_total = reduce(sumar, Temperaturas)