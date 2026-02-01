Grammar2CNF: main.o produccion.o terminal.o noterminal.o gramatica.o
	g++ -o Grammar2CNF main.o produccion.o terminal.o noterminal.o gramatica.o

main.o: main.cc  produccion.h terminal.h noterminal.h gramatica.h
	g++ -c main.cc

gramatica.o: produccion.h terminal.h noterminal.h gramatica.h gramatica.cc
	g++ -c gramatica.cc

produccion.o: produccion.cc produccion.h 
	g++ -c produccion.cc

terminal.o: terminal.cc terminal.h
	g++ -c terminal.cc

noterminal.o: noterminal.h noterminal.cc
	g++ -c noterminal.cc

clean:
	rm -f *.o programa_cliente
