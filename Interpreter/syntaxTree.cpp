#include "syntaxTree.h"
#include <iostream>

SyntaxTree::~SyntaxTree()
{
  for (int i = 0; i < children.size(); i++)
  {
    if (children[i] != NULL)
      delete children[i];
  }
}

SyntaxTree::SyntaxTree(string typ)
{
  type = typ;
}

SyntaxTree& SyntaxTree::operator=(SyntaxTree& o)
{
  type = o.getType();
  vector<SyntaxTree*> c = o.getChildren();
  tokens = o.getTokens();
  for (int i = 0; i < c.size(); i++)
  {
    SyntaxTree* newC = new SyntaxTree("");
    (*newC) = (*c[i]);

    children.push_back(newC);
  }
  return *this;
}


void SyntaxTree::print(int i)
{
  for (int j = 0; j < i; j++)
    cout << "|--";
  cout << type << ":";
  for (int j = 0; j < tokens.size(); j++)
  {
    cout << " \"" << tokens[j] << "\"";
  }
  cout << endl;

  for (int j = 0; j < children.size(); j++)
  {
    children[j]->print(i + 1);
  }
}

void SyntaxTree::addChild(SyntaxTree* child)
{
  children.push_back(child);
}

void SyntaxTree::addToken(string tok)
{
  tokens.push_back(tok);
}

void SyntaxTree::drop(int d)
{
  vector<SyntaxTree*> newC;
  for(int i = 0; i < children.size(); i++)
  {
    if (i != d)
    {
      newC.push_back(children[i]);
    }
  }

  children = newC;
}

int SyntaxTree::numChildren()
{
  return children.size();
}

