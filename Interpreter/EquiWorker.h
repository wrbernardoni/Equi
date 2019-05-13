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
	bool ownedFrame;
	EquiFrame* data;

	bool runElse;
	bool breakFlag;
	bool continueFlag;
	bool returnFlag;
	bool killReturn;
	EquiObject* returnItem;
	
	EquiObject* getToken(string);
	EquiObject* getType(string);
	bool isToken(string);
	void emplaceToken(string, EquiObject*);
	bool isType(string);
	void emplaceType(string, EquiObject*);

	void scopeUp();
	void scopeDown();

public:
	inline bool killAnyways() { return returnFlag && killReturn; };
	EquiFrame getFrame();
	void setFrame(const EquiFrame&);
	
	void resetScope();

	EquiWorker();
	~EquiWorker();
	
	pair<EquiObject*, bool> run(SyntaxTree*);
};

#endif
