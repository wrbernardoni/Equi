#ifndef EQUI_WORKER_H_
#define EQUI_WORKER_H_

#include "global.h"
#include "syntaxTree.h"
#include "AllObj.h"
#include "EquiFrame.h"

#include <map>
#include <string>
#include <deque>

using namespace std;

class EquiWorker
{
private:
	deque<map<string, EquiObject*>*> tokens;
	deque<map<string, EquiObject*>*> types;
	bool runElse;
	bool breakFlag;
	bool continueFlag;
	
	EquiObject* getToken(string);
	EquiObject* getType(string);
	bool isToken(string);
	void emplaceToken(string, EquiObject*);

	void scopeUp();
	void scopeDown();

public:
	EquiFrame getFrame();
	void setFrame(const EquiFrame&);
	
	void resetScope();

	EquiWorker();
	~EquiWorker();
	
	pair<EquiObject*, bool> run(SyntaxTree*);
};

#endif
