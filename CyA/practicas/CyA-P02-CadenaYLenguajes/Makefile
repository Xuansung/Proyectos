programa_cliente: main.o cadena.o alfabeto.o lenguaje.o
	g++ -o programa_cliente main.o cadena.o alfabeto.o lenguaje.o

main.o: main.cc cadena.h alfabeto.h lenguaje.h
	g++ -c main.cc

cadena.o: cadena.cc cadena.h alfabeto.h
	g++ -c cadena.cc

alfabeto.o: alfabeto.cc alfabeto.h
	g++ -c alfabeto.cc

lenguaje.o: lenguaje.cc lenguaje.h cadena.h alfabeto.h
	g++ -c lenguaje.cc

clean:
	rm -f *.o programa_cliente
