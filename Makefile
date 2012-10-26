OBJECTS=TransducingLayer.o CharacteristicMatrix.o CapSimulation.o LaserBeam.o Matrix.o
CPPFLAGS=-O3 -Wall

cap-sim: main.cpp $(OBJECTS)
	g++ $(CPPFLAGS) $^ -o $@

CapSimulation.o: CharacteristicMatrix.h DefaultCapMaterial.h LaserBeam.h TransducingLayer.h CapMaterialInterface.h
CharacteristicMatrix.o: Matrix.h

%.o: %.cpp %.h
	g++ $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@-rm *~
	@-rm *.o
	@-rm cap-sim
	@cd test && make clean

.PHONY: test
test:
	@cd test && make
