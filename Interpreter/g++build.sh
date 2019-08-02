if g++ --std=c++11 -O3 -o build/Equi -I . -I ./standardLibrary -I ./objects *.cpp objects/*.cpp -pthread
then
	exit 0
else
	exit 1
fi

