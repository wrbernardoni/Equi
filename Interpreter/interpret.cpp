#include "parse.h"
#include "global.h"
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

  int lineNum = 0;

  while(!in->eof())
  {
    vector<string> tokens = getLine(in, lineNum);
    if (tokens.size() == 0)
      continue;

    if (verbose >= TOKEN_LIST_VERB)
    {
      cout << "<<" << lineNum << ">>: ";
      for (int i = 0; i < tokens.size(); i++)
      {
        cout << "[" << tokens[i] << "] ";
      }
      cout << endl << endl;
    }

    SyntaxTree* lineTree = lineParse(tokens);
    if (verbose >= SYNTAX_TREE_LIST)
      lineTree->print(0);

    delete lineTree;
  }

  return 0;
}

