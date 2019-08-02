#include "parse.h"

#ifndef EQUI_CORE_H_
#define EQUI_CORE_H_

using namespace std;

#include <vector>
#include <stack>
#include <map>
#include <string>
#include <deque>
#include <mutex>

class EquiObject;
class EquiFrame;
//class CodeLine;

class EquiTask
{
public:
	int lineCount;
	int scopeSince;
	bool breakFlag;
	bool continueFlag;
	bool elseFlag;
	EquiFrame* frame;
	vector<stack<pair<EquiObject*, bool>>> registers;
	vector<CodeLine>* code;

	EquiObject* fn;
	EquiObject* inp;
	vector<pair<string, EquiObject*>> apparentTok;

	EquiObject* out;
	bool complete;

	EquiTask()
	{
		complete = false;
		out = NULL;

		frame = NULL;

		lineCount = 0;
		scopeSince = 0;
		breakFlag = false;
		continueFlag = false;
		elseFlag = false;

		code = NULL;
		fn = NULL;
		inp = NULL;
	}

	void clean();
};

class EquiCore
{
private:
	deque<EquiTask*> tasks;
	mutex queueMutex;
	bool killingMode;
public:
	EquiCore();
	int addTask(EquiTask*);
	EquiTask* getTask();
	void setKillingMode();
};

extern EquiCore* globalCore;


#endif
