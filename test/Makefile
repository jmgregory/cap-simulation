my_tests:=$(patsubst %.cpp,%,$(wildcard *Test.cpp))

LIBS=-lUnitTest++
CPPFLAGS=-O0 -Wall -Winvalid-pch -I/opt/local/include/UnitTest++ -L/opt/local/lib

.SECONDARY: $(my_tests)

.PHONY: test-all
test-all test all: stdafx.h.gch $(patsubst %,%Run,$(my_tests))
	@echo "[0;37;42m                         "
	@echo "    All tests passed!    "
	@echo "                         [0;37;40m"
	@echo

CharacteristicMatrixTest: ../Matrix.cpp ../Matrix.h

stdafx.h.gch: stdafx.h
	@g++ $(CPPFLAGS) -o $@ -c $^

%Test: %Test.cpp main.cpp ../%.cpp ../%.h
	@echo Building $@...
	@g++ -include stdafx.h $(CPPFLAGS) $(LIBS) -o $@ $^

.PHONY: %TestRun
%TestRun: %Test
	@echo Running $<...
	@./$< ; if [ $$? -ne 0 ] ; then echo ; echo "[0;37;41m                          " ; echo "       Test Failed!       " ; echo "                          [0;37;40m" ; echo ; false ; fi
	@echo

.PHONY: clean
clean:
	-@rm *~
	-@rm $(my_tests)
	-@rm stdafx.h.gch