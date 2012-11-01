my_tests:=$(patsubst %.cpp,%,$(wildcard *Test.cpp))

LIBS=-lUnitTest++
CPPFLAGS=-O0 -Wall -Winvalid-pch -I/opt/local/include/UnitTest++ -L/opt/local/lib

PASSMESSAGE=echo "[0;37;42m                                     [0;37;40m" ; echo "[0;37;42m          All tests passed!          [0;37;40m" ; echo "[0;37;42m                                     [0;37;40m"; echo
FAILMESSAGE=echo "[0;37;41m                                     [0;37;40m" ; echo "[0;37;41m           Test(s) Failed!           [0;37;40m" ; echo "[0;37;41m                                     [0;37;40m" ; echo

.SECONDARY: $(my_tests) ../CapSimulation.o

.PHONY: test-all test all check
test-all test all check: stdafx.h.gch $(patsubst %,%Run,$(my_tests))
	@$(PASSMESSAGE)

CharacteristicMatrixTest: ../Matrix.o
CapSimulationTest: ../TransducingLayer.o ../CharacteristicMatrix.o ../Matrix.o ../LaserBeam.o ../HomogeneousCharacteristicMatrix.o

stdafx.h.gch: stdafx.h
	@g++ $(CPPFLAGS) -o $@ -c $^

../%.o:
	@cd .. && make $*.o

%Test: %Test.cpp main.cpp ../%.o stdafx.h.gch
	@echo Building $@...
	@g++ -include stdafx.h $(CPPFLAGS) $(LIBS) -o $@ $(subst stdafx.h.gch,,$^)

.PHONY: %TestRun
%TestRun: %Test
	@echo Running $<...
	@./$< ; if [ $$? -ne 0 ] ; then echo ; $(FAILMESSAGE) ; false ; fi
	@echo

.PHONY: clean
clean:
	-@rm *~
	-@rm $(my_tests)
	-@rm stdafx.h.gch
	-@rm cap-sim-output-dump.dat
