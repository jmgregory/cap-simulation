my_tests:=$(patsubst %.cpp,%,$(wildcard *Test.cpp))

TESTLIBS=-lcpptest

.PHONY: test-all
test-all: $(my_tests)

%Test: %Test.cpp ../%.cpp
	@g++ $(TESTLIBS) -o $@ $^
	-@./$@
	-@rm ./$@

.PHONY: clean
clean:
	-rm *~
