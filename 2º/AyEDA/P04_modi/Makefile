CXX = g++
CXXFLAGS = -Wall -O2 -std=c++23

OBJ = main.o

all: tabla_hash

tabla_hash: $(OBJ)
	$(CXX) $(CXXFLAGS) -o tabla_hash $(OBJ)

main.o: main.cc nif.h dispersion.h exploration.h sequence.h hash.h
	$(CXX) $(CXXFLAGS) -c main.cc

clean:
	rm -f *.o tabla_hash
