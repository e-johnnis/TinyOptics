EXE:=./test
CXX:=g++ -g -fopenmp -o $(EXE)
INCLUDE:=-I ../include

SRC=../src/optcore.cpp ../src/shape.cpp ../src/element.cpp ../src/system.cpp

.PHONY:	matrix naa

matrix:
	$(CXX) $(INCLUDE) -o $(EXE) matrix.cpp
	$(EXE)

naa:
	$(CXX) $(INCLUDE) -o $(EXE) nearaxis.cpp -L$(EXE) $(SRC)
	$(EXE)