#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct cmdline
{
	string file;
};

cmdline argHandler(int argc, char* argv[])
{
	cmdline opts;

	cout << "Arguments: " << endl;
	for (int i = 0; i < argc; i++)
	{
		cout << "[" << i << "] -- " << argv[i] << endl;
	}

	if (argc >= 2)
	{
		opts.file = argv[1];
	}

	cout << opts.file << endl;

	return opts;
}

int main(int argc, char* argv[])
{
	cmdline opts = argHandler(argc, argv);

	if (opts.file == "")
	{
		cout << "Need equi file to interpret.\n";
		return;
	}

	cout << "Hello world.\n";
}