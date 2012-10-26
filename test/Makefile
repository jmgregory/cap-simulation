my_tests:=$(patsubst %.cpp,%,$(wildcard *Test.cpp))

LIBS=-lUnitTest++
CPPFLAGS=-O3 -Wall -I/opt/local/include/UnitTest++ -L/opt/local/lib

.SECONDARY: $(my_tests)

.PHONY: test-all
test-all: $(patsubst %,%Run,$(my_tests))

CharacteristicMatrixTest: ../Matrix.cpp ../Matrix.h

%Test: %Test.cpp main.cpp ../%.cpp ../%.h
	@g++ $(CPPFLAGS) $(LIBS) -o $@ $^

.PHONY: %TestRun
%TestRun: %Test
	@./$<

.PHONY: clean
clean:
	-@rm *~
	-@rm $(my_tests)
