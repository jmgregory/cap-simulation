OBJECTS=TransducingLayer.o Matrix.o CharacteristicMatrix.o LaserBeam.o CapSimulation.o
CPPFLAGS=-O3 -Wall -Winvalid-pch

cap-sim: main.cpp $(OBJECTS)
	g++ $(CPPFLAGS) $^ -o $@

CapSimulation.o: CharacteristicMatrix.h DefaultCapMaterial.h LaserBeam.h TransducingLayer.h CapMaterialInterface.h
CharacteristicMatrix.o: Matrix.h

%.o: %.cpp %.h stdafx.h.gch
	g++ $(CPPFLAGS) -include stdafx.h -c $< -o $@

stdafx.h.gch: stdafx.h
	g++ $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@-rm *~
	@-rm *.o
	@-rm cap-sim
	@-rm stdafx.h.gch
	@cd test && make clean

.PHONY: test
test: cap-sim
	@cd test && make
