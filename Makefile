cap-sim: main.cpp CapSimulation.o CharacteristicMatrix.o
	g++ -O3 -Wall main.cpp CapSimulation.o CharacteristicMatrix.o -o cap-sim

CapSimulation.o: CapSimulation.cpp CapSimulation.h CharacteristicMatrix.h CapMaterial.h
	g++ -O3 -Wall -c CapSimulation.cpp -o CapSimulation.o

CharacteristicMatrix.o: CharacteristicMatrix.cpp CharacteristicMatrix.h
	g++ -O3 -Wall -c CharacteristicMatrix.cpp -o CharacteristicMatrix.o

.PHONY: clean
clean:
	@-rm *~
	@-rm *.o
	@-rm cap-sim
	@cd test && make clean

.PHONY: test
test: cap-sim
	@cd test && make QUIET=yes
