#include <iostream>
#include <fstream>
#include <string>

#include "parse.h"
#include "global.h"

using namespace std;

int verbose;
bool failsafe;

struct cmdline
{
	string file;
};

cmdline argHandler(int argc, char* argv[])
{
	cmdline opts;
	failsafe = false;

	if (argc >= 2)
	{
		opts.file = argv[1];
	}

	if (opts.file == "--")
	{
		failsafe = true;
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
		else if (arg == "-FS" || arg == "--failsafe")
		{
			failsafe = true;
		}
		else if (arg == "-NFS" || arg == "--notfailsafe")
		{
			failsafe = false;
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

