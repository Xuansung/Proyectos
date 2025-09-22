programa_cliente: main.o analizador.o variable.o bucle.o comentario.o case.o
	g++ -o programa_cliente main.o analizador.o variable.o bucle.o comentario.o case.o

main.o: main.cc analizador.h variable.h bucle.h comentario.h case.h
	g++ -c main.cc

analizador.o: analizador.cc analizador.h variable.h bucle.h comentario.h case.h
	g++ -c analizador.cc

variable.o: variable.cc variable.h 
	g++ -c variable.cc

bucle.o: bucle.cc bucle.h
	g++ -c bucle.cc

comentario.o: comentario.h comentario.cc
	g++ -c comentario.cc

case.o: case.h case.cc
	g++ -c case.cc
clean:
	rm -f *.o programa_cliente
