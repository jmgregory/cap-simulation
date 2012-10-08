cap-sim: main.cpp cap-simulation.o characteristic_matrix.o
	g++ -O3 -Wall main.cpp cap-simulation.o characteristic_matrix.o -o cap-sim

cap-simulation.o: cap-simulation.cpp cap-simulation.h characteristic_matrix.h cap-material.h
	g++ -O3 -Wall -c cap-simulation.cpp -o cap-simulation.o

characteristic_matrix.o: characteristic_matrix.cpp characteristic_matrix.h
	g++ -O3 -Wall -c characteristic_matrix.cpp -o characteristic_matrix.o

.PHONY: clean
clean:
	-rm *~
	-rm *.o
	-rm cap-sim
