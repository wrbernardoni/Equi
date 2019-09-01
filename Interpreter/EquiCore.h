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
	int uid;
	int lineCount;
	int scopeSince;
	bool breakFlag;
	bool continueFlag;
	bool elseFlag;

	int resumeAt;

	EquiFrame* frame;
	vector<stack<pair<EquiObject*, bool>>> registers;
	vector<CodeLine>* code;

	EquiObject* fn;
	EquiObject* inp;
	vector<pair<string, EquiObject*>> apparentTok;

	EquiObject* out;
	bool complete;

	vector<EquiTask*> post;

	EquiTask()
	{
		uid = -1;
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

		resumeAt = -1;
	}

	void clean();
};

class TaskResult
{
public:
	EquiTask* tsk;
	int refCount;

	TaskResult()
	{
		tsk = NULL;
		refCount = 0;
	};
};

class EquiCore
{
private:
	map<int, TaskResult> allT;
	int uidc;
	deque<EquiTask*> tasks;
	mutex queueMutex;
	bool killingMode;
public:
	EquiCore();
	int addTask(EquiTask*);
	int addResult(EquiObject*);
	EquiTask* getTask();
	void setKillingMode();
	int tasksLeft();
	int getUID();

	void addReference(int);
	void deadReference(int);
	bool isComplete(int);
	void markComplete(int);
	int addPost(int, EquiTask*);
	EquiObject* getResult(int);
};

extern EquiCore* globalCore;


#endif
