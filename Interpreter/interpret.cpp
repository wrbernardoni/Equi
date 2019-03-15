#include "parse.h"
#include <fstream>
#include <iostream>

int interpret(string fn)
{
  istream* in;

  if (fn == "--")
  {
    in = &cin;
  }
  else
  {
    in = new ifstream(fn);
    if (!(*in))
    {
      cerr << "Failed to open file: " << fn << "\n";
      return 1;
    }
  }

  int lineNum = 1;

  while(!in->eof())
  {
    vector<string> tokens = getLine(in, lineNum);
    cout << "<<" << lineNum << ">>: ";
    for (int i = 0; i < tokens.size(); i++)
    {
      cout << "[" << tokens[i] << "] ";
    }
    cout << endl;
  }

  return 0;
}

