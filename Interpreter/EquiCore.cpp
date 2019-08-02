#include "EquiCore.h"
#include "AllObj.h"
#include "EquiFrame.h"
#include "compile.h"

#include <iostream>

EquiCore::EquiCore()
{
	killingMode = false;
}

void EquiCore::setKillingMode()
{
	killingMode = true;
}

int EquiCore::addTask(EquiTask* t)
{
	unique_lock<mutex> guard(queueMutex, defer_lock);
	guard.lock();
	tasks.push_back(t);
	guard.unlock();
	return 0;
}

int EquiCore::tasksLeft()
{
	int n = 0;
	unique_lock<mutex> guard(queueMutex, defer_lock);
	guard.lock();
	n = tasks.size();
	guard.unlock();
	return n;
}

EquiTask* EquiCore::getTask()
{
	unique_lock<mutex> guard(queueMutex, defer_lock);

	guard.lock();
	while(tasks.size() == 0 && !killingMode)
	{
		guard.unlock();

		guard.lock();
	}

	if (tasks.size() == 0)
	{
		guard.unlock();
		return NULL;
	}

	EquiTask* t = tasks.front();
	tasks.pop_front();
	guard.unlock();
	return t;
}

void EquiTask::clean()
{
	if (frame != NULL)
		delete frame;

	for (int i = 0; i < registers.size(); i++)
	{
		while (registers[i].size() != 0)
		{
			pair<EquiObject*, bool> t = registers[i].top();
			registers[i].pop();
			if (t.second)
				delete t.first;
		}
	}
	registers.clear();
	
	if (code != NULL)
		delete code;
	if (fn != NULL)
		delete fn;
	if (inp != NULL)
		delete inp;

	lineCount = 0;
	scopeSince = 0;
	breakFlag = false;
	continueFlag = false;
	elseFlag = false;

	code = NULL;
	fn = NULL;
	inp = NULL;
	frame = NULL;

	
	for (int i = 0; i < apparentTok.size(); i++)
	{
		delete apparentTok[i].second;
	}
}

