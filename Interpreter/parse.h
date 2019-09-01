#ifndef EQUI_PARSE_H_
#define EQUI_PARSE_H_

#include <string>
#include <vector>
#include <istream>

#include "syntaxTree.h"

using namespace std;

int interpret(string);

vector<string> getLine(istream*, int&);
SyntaxTree* lineParse(vector<string>);

struct CodeLine
{
	int cmd;
	int reg;
	vector<string> args;
};
void printCodeLine(CodeLine);

vector<CodeLine> compile(vector<SyntaxTree*>);

#endif

