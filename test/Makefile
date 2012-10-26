my_tests:=$(patsubst %.cpp,%,$(wildcard *Test.cpp))

LIBS=-lUnitTest++
CPPFLAGS=-O0 -Wall -Winvalid-pch -I/opt/local/include/UnitTest++ -L/opt/local/lib

.SECONDARY: $(my_tests)

.PHONY: test-all
test-all test all: stdafx.h.gch $(patsubst %,%Run,$(my_tests))

CharacteristicMatrixTest: ../Matrix.cpp ../Matrix.h

stdafx.h.gch: stdafx.h
	@g++ $(CPPFLAGS) -o $@ -c $^

%Test: %Test.cpp main.cpp ../%.cpp ../%.h
	@echo Building $@...
	@g++ -include stdafx.h $(CPPFLAGS) $(LIBS) -o $@ $^

.PHONY: %TestRun
%TestRun: %Test
	@echo Running $<...
	@./$<
	@echo

.PHONY: clean
clean:
	-@rm *~
	-@rm $(my_tests)
	-@rm stdafx.h.gch