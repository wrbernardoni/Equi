cd Interpreter
if ./g++build.sh
then
	cd ../EquiTests
	./runTests.sh Equi
fi

