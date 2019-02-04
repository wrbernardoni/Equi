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

void syntaxTree(vector<string> tokens)
{
	map<string, bool> operators = operatorMap();

	vector<tokTree> code;
	tokTree* current = NULL;
	bool setChild = false;

	deque<tokTree*> scope;
	scope.push_back(NULL);

	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "{" || tokens[i] == "(")
		{
			if (current == NULL)
			{
				if (code.size() == 0)
					;
				else
					current = &code[code.size() - 1];
			}
			else
			{
				if (current->subTree.size() == 0)
					;
				else
					current = &current->subTree[current->subTree.size() - 1];
			}

			scope.push_back(current);
		}
		else if (tokens[i] == "}" || tokens[i] == ")")
		{
			if (scope.size() <= 1)
				cerr << "Rogue " << tokens[i] << " at token " << i << endl;
			else
			{
				scope.pop_back();
				current = scope.back();
			}
		}
		else if (tokens[i] == ";" || tokens[i] == ",")
		{
			current = scope.back();
		}
		else if (current == NULL)
		{
			tokTree t;
			t.token = tokens[i];
			t.parent = NULL;
			code.push_back(t);
			current = &code[code.size() - 1];
		}
		else
		{
			tokTree t;
			t.token = tokens[i];
			t.parent = current;
			current->subTree.push_back(t);
			current = &current->subTree[current->subTree.size() - 1];
		}
	}

	deque<pair<tokTree,int>> dQ;
	for (int i = 0; i < code.size(); i++)
	{
		pair<tokTree, int> temp(code[i], 0);
		dQ.push_back(temp);
	}

	while (!dQ.empty())
	{
		pair<tokTree, int> c = dQ.front();
		dQ.pop_front();

		for (int j = c.first.subTree.size() - 1; j >= 0; j--)
		{
			pair<tokTree, int> temp(c.first.subTree[j], c.second + 1);
			dQ.push_front(temp);
		}

		for (int i = 0; i < c.second; i++)
		{
			cout << "> ";
		}
		cout << c.first.token << endl;
	}
}

