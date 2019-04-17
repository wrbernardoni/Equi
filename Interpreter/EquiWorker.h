#ifndef EQUI_WORKER_H_
#define EQUI_WORKER_H_

#include "global.h"
#include "syntaxTree.h"
#include "AllObj.h"

#include <map>
#include <string>
#include <deque>

using namespace std;

class EquiWorker
{
private:
	deque<map<string, EquiObject*>*> tokens;
	bool runElse;
	bool breakFlag;
	bool continueFlag;
	
	EquiObject* getToken(string);
	bool isToken(string);
	void emplaceToken(string, EquiObject*);

	void scopeUp();
	void scopeDown();

public:
	void resetScope();

	EquiWorker();
	~EquiWorker();
	
	pair<EquiObject*, bool> run(SyntaxTree*);
};

#endif
