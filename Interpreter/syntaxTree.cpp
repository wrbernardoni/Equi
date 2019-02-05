#include "parse.h"
#include "global.h"

#include <iostream>
#include <fstream>
#include <deque>
#include <utility>
#include <map>


struct tokTree
{
	string token;
	vector<tokTree> subTree;
	tokTree* parent;
};

map<string, bool> operatorMap()
{
	map<string, bool> op;
	op["+"] = true;
	op["-"] = true;
	op["/"] = true;
	op["%"] = true;
	op["="] = true;


	return op;
}

//TODO: Recursive Descent Parser
void syntaxTree(vector<string> tokens)
{
	
}

