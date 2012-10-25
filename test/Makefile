TESTLIBS=-lcpptest

.PHONY: test-all
test-all: CharacteristicMatrixTest

.PHONY: CharacteristicMatrixTest
CharacteristicMatrixTest: CharacteristicMatrixTest.cpp
	g++ $(TESTLIBS) -o CharacteristicMatrixTest CharacteristicMatrixTest.cpp ../CharacteristicMatrix.cpp
	-./CharacteristicMatrixTest
	-rm ./CharacteristicMatrixTest
