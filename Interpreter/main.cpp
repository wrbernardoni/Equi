#include <iostream>
#include <fstream>
#include <string>

#include "parse.h"
#include "global.h"

#include <thread>

using namespace std;

int verbose;
int numThreads;
bool failsafe;
bool fullParse;
bool showComp;
bool verbCompiled;

struct cmdline
{
	string file;
};

cmdline argHandler(int argc, char* argv[])
{
	cmdline opts;
	failsafe = false;
	fullParse = true;
	verbCompiled = false;
	numThreads = MAX(std::thread::hardware_concurrency()-1, 1);

	if (argc >= 2)
	{
		opts.file = argv[1];
	}

	if (opts.file == "--")
	{
		failsafe = true;
		fullParse = false;
	}

	for (int i = 2; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == "-V" || arg == "--verbose")
		{
			if (argc > i+1)
			{
				verbose = atoi(argv[i+1]);
				i++;
			}
			else
			{
				cerr << "Missing verbosity level.\n";
			}
		}
		else if (arg == "-t" || arg == "--threads")
		{
			if (argc > i+1)
			{
				numThreads = atoi(argv[i+1]);
				i++;
			}
			else
			{
				cerr << "Missing number of threads to spawn.\n";
			}
		}
		else if (arg == "-FS" || arg == "--failsafe")
		{
			failsafe = true;
		}
		else if (arg == "-NFS" || arg == "--notfailsafe")
		{
			failsafe = false;
		}
		else if (arg == "-PL" || arg == "--parseline")
		{
			fullParse = false;
		}
		else if (arg == "-PF" || arg == "--parsefull")
		{
			fullParse = true;
		}
		else if (arg == "-Vcomp")
		{
			verbCompiled = true;
		}
		else
		{
			cerr << "Unrecognized command line " << arg << endl;
		}
	}

	P_VERB("Arguments: " << endl, ALL_STEP_VERB);
	for (int i = 0; i < argc; i++)
	{
		P_VERB("[" << i << "] -- " << argv[i] << endl, ALL_STEP_VERB);
	}

	return opts;
}

int main(int argc, char* argv[])
{
	verbose = 0;
	cmdline opts = argHandler(argc, argv);

	if (opts.file == "")
	{
		cerr << "Need equi file to interpret or \"--\" to run stream interpreter.\n";
		return 1;
	}

	int error = interpret(opts.file);
	if (error != 0)
    {
      cerr << "Terminating with error code: " << error << endl;
      return 1;
    }
	return 0;
}

