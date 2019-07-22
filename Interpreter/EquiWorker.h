#ifndef EQUI_WORKER_H_
#define EQUI_WORKER_H_

#include "global.h"
#include "parse.h"
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

public:
	void scopeUp();
	void scopeDown();

	inline bool killAnyways() { return returnFlag && killReturn; };
	EquiFrame getFrame();
	void setFrame(const EquiFrame&);
	void loanFrame(EquiFrame*);
	void loanType(EquiFrame*);

	inline EquiFrame* touchFrame() {return data;};
	
	void resetScope();

	EquiWorker();
	EquiWorker(EquiFrame*);
	~EquiWorker();
	
	pair<EquiObject*, bool> run(SyntaxTree*);
	pair<EquiObject*, bool> run(vector<CodeLine>*);
};

#endif
