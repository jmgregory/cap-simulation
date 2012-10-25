OBJECTS=TransducingLayer.o CharacteristicMatrix.o CapSimulation.o
CPPFLAGS=-O3 -Wall

cap-sim: main.cpp $(OBJECTS)
	g++ $(CPPFLAGS) $^ -o $@

CapSimulation.o: CapSimulation.cpp CapSimulation.h CharacteristicMatrix.h CapMaterial.h LaserBeam.h TransducingLayer.h
	g++ $(CPPFLAGS) -c $< -o $@

%.o: %.cpp %.h
	g++ $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@-rm *~
	@-rm *.o
	@-rm cap-sim
	@cd test && make clean

.PHONY: test
test: cap-sim
	@cd test && make QUIET=yes
