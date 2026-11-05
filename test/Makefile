EXE:=./test
CXX:=g++ -DDEBUG -g -fopenmp -o $(EXE)
INCLUDE:=-I ../include
SVG_DIR:=./svg

SRC=../src/core.cpp ../src/shape.cpp ../src/element.cpp ../src/system.cpp ../src/drawer.cpp ../src/catalog.cpp

.PHONY:	matrix naa seq

matrix:
	$(CXX) $(INCLUDE) -o $(EXE) matrix.cpp
	$(EXE)

naa:
	$(CXX) $(INCLUDE) -o $(EXE) nearaxis.cpp -L$(EXE) $(SRC)
	$(EXE)

seq: $(SVG_DIR)
	mkdir -p $(SVG_DIR)/seq
	$(CXX) $(INCLUDE) -o $(EXE) sequential.cpp -L$(EXE) $(SRC)
	$(EXE) OHARA_*.csv

tessar: $(SVG_DIR)
	$(CXX) $(INCLUDE) -o $(EXE) tessar.cpp -L$(EXE) $(SRC)
	$(EXE) OHARA_*.csv

$(SVG_DIR):
	mkdir -p $(SVG_DIR)