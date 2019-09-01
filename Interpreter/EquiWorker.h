#ifndef EQUI_WORKER_H_
#define EQUI_WORKER_H_

#include "global.h"
#include "parse.h"
//#include "syntaxTree.h"
#include "EquiObject.h"
#include "EquiFrame.h"
#include "EquiCore.h"


using namespace std;

class EquiWorker
{
private:
	int resF;

	bool ownedFrame;
	EquiFrame* data;

	int pLine;
	int lineCount;
	int scopeSince;
	vector<stack<pair<EquiObject*, bool>>> registers;

	bool runElse;
	bool breakFlag;
	bool continueFlag;
	bool elseFlag;
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
	int froze;
	bool mainWorker;

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
	
	//pair<EquiObject*, bool> run(SyntaxTree*);
	pair<EquiObject*, bool> run(vector<CodeLine>*);
	pair<EquiObject*, bool> runCodeLine(vector<CodeLine>*);
	pair<EquiObject*, bool> evalTask(EquiTask*);
};

#endif
