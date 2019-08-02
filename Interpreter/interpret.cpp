#include "parse.h"
#include "global.h"
#include <fstream>
#include <iostream>

#include "EquiWorker.h"
#include "EquiCore.h"

#include <thread>
#include <mutex>

EquiCore* globalCore;

void workerThread(bool* death)
{
  while(!(*death))
  {
    EquiWorker worker;

    try
    {
      while(!(*death))
      {
        EquiTask* t = globalCore->getTask();
        if (t != NULL)
        {
          pair<EquiObject*, bool> ret = worker.evalTask(t);
          t->out = ret.first->clone();
          if (ret.second)
            delete ret.first;
          t->complete = true;
          t->clean();
        }
        else
        {
          break;
        }
      }
    }
    catch (string m)
    {
      cerr << "Error on external task: " << m << endl;
    }
  }
}

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
  EquiCore scheduler;
  globalCore = &scheduler;
  bool death = false;

  vector<thread*> agents;
  for (int i = 0; i < numThreads; i++)
  {
    thread* t = new thread(workerThread, &death);
    agents.push_back(t);
  }

  EquiWorker core;

  if (!fullParse)
  {
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

      vector<SyntaxTree*> ast;
      ast.push_back(lineTree);

      if (verbose >= SYNTAX_TREE_LIST)
        lineTree->print(0);

      if (verbose >= SYNTAX_TREE_LIST)
        cout << endl;

      vector<CodeLine> cde = compile(ast);

      if (verbCompiled)
      {
        for (int i = 0; i < cde.size(); i++)
        {
          cout << "[" << i << "]\t";
          printCodeLine(cde[i]);
        }
      }

      EquiObject* o = NULL;
      try
      {
        pair<EquiObject*, bool> rn = core.run(&cde);
        P_VERB("-->" << rn.first->to_string() << endl, TOKEN_PRINT_VERB);
        if (rn.second || core.killAnyways())
          delete rn.first;
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
  }
  else
  {
    vector<SyntaxTree*> code;
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

      code.push_back(lineTree);
    }

    vector<CodeLine> cde = compile(code);

    if (verbCompiled)
    {
      for (int i = 0; i < cde.size(); i++)
      {
        cout << "[" << i << "]\t";
        printCodeLine(cde[i]);
      }
    }


    EquiObject* o = NULL;
    try
    {
      pair<EquiObject*, bool> rn = core.run(&cde);
      P_VERB("-->" << rn.first->to_string() << endl, TOKEN_PRINT_VERB);
      if (rn.second || core.killAnyways())
        delete rn.first;
    }
    catch (string m)
    {
      cerr << "Error on line: " << lineNum << ": " << m << endl;

      if (!failsafe)
      {
        return 1;
      }
    }

    for(int i = 0; i < code.size(); i++)
    {
      delete code[i];
    }
  }

  death = true;
  globalCore->setKillingMode();
  for(int i = 0; i < agents.size(); i++)
  {
    agents[i]->join();
  }

  return 0;
}

