CXX=g++
CXXFLAGS= -Wall -std=c++11 -g -O3
EXEC_FILES= coord test

all: coord
# Regle generique
%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

exec: main.o coord.o fourmi.o colonie.o place.o grille.o
	$(CXX) -o $@ $^ $(LDFLAGS)
    
test: test.o coord.o fourmi.o colonie.o place.o grille.o
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
coord.o: coord.hpp
fourmi.o: fourmi.hpp
colonie.o: colonie.hpp
place.o: place.hpp
grille.o: grille.hpp
test.o: test.cpp

run: exec
	./exec

check: test
	./test

clean:
	rm -f *.o $(EXEC_FILES)