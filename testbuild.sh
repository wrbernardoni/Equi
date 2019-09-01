cd Interpreter
echo "Building"
if ./g++build.sh
then
	echo "Beginning tests"
	cd ../EquiTests
	./runTests.sh Equi
fi
