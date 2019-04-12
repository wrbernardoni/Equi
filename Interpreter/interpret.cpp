#include "parse.h"
#include "global.h"
#include <fstream>
#include <iostream>

#include "EquiWorker.h"

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
  // Spawn single worker
  EquiWorker core;

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
      cout << endl;
    }

    SyntaxTree* lineTree = NULL;
    try
    {
      lineTree = lineParse(tokens);
    }
    catch (string m)
    {
      cerr << "Error on line: " << lineNum << ": " << m << endl;

      if (!failsafe)
      {
        return 1;
      }
    }

    if (lineTree == NULL)
      continue;

    if (verbose >= SYNTAX_TREE_LIST)
      lineTree->print(0);

    if (verbose >= SYNTAX_TREE_LIST)
      cout << endl;

    EquiObject* o = NULL;
    try
    {
      EquiObject* o = core.run(lineTree);
      P_VERB("-->" << o->to_string() << endl, TOKEN_PRINT_VERB);
      delete o;
    }
    catch (string m)
    {
      cerr << "Error on line: " << lineNum << ": " << m << endl;

      if (!failsafe)
      {
        return 1;
      }
    }

    core.resetScope();

    delete lineTree;
  }

  return 0;
}

