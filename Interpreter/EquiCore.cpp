#include "EquiCore.h"
#include "AllObj.h"
#include "EquiFrame.h"
#include "compile.h"

#include <iostream>

EquiCore::EquiCore()
{
	killingMode = false;
	uidc = 0;
}

void EquiCore::setKillingMode()
{
	killingMode = true;
}

int EquiCore::addTask(EquiTask* t)
{
	unique_lock<mutex> guard(queueMutex, defer_lock);

	guard.lock();
	int n;
	if (t->uid == -1)
	{
		n = uidc++;
	}
	else
	{
		n = t->uid;
	}
	tasks.push_back(t);
	allT[n].tsk = t;
	t->uid = n;
	guard.unlock();
	return n;
}

int EquiCore::addResult(EquiObject* out)
{
	unique_lock<mutex> guard(queueMutex, defer_lock);

	guard.lock();
	int n = uidc++;
	EquiTask* t = new EquiTask;
	allT[n].tsk = t;
	t->uid = n;
	t->out = out;
	t->complete = true;
	guard.unlock();
	return n;
}

int EquiCore::getUID()
{
	unique_lock<mutex> guard(queueMutex, defer_lock);

	guard.lock();
	int n = uidc++;
	guard.unlock();
	return n;
}

void EquiCore::addReference(int n)
{
	unique_lock<mutex> guard(queueMutex, defer_lock);
	guard.lock();
	allT[n].refCount = allT[n].refCount + 1;
	guard.unlock();
}

bool EquiCore::isComplete(int n)
{
	unique_lock<mutex> guard(queueMutex, defer_lock);
	guard.lock();
	if (allT.count(n) == 0)
	{
		guard.unlock();
		return false;
	}
	else if (allT[n].tsk == NULL)
	{
		guard.unlock();
		return false;
	}
	else
	{
		bool cmp = allT[n].tsk->complete;
		guard.unlock();
		return cmp;
	}
}

EquiObject* EquiCore::getResult(int n)
{
	if (!isComplete(n))
		return NULL;

	unique_lock<mutex> guard(queueMutex, defer_lock);
	guard.lock();
	EquiObject* o = allT[n].tsk->out;
	guard.unlock();

	return o;
}

void EquiCore::markComplete(int n)
{
	unique_lock<mutex> guard(queueMutex, defer_lock);
	guard.lock();
	if (allT.count(n) == 0)
	{
		guard.unlock();
		return;
	}
	else if (allT[n].tsk == NULL)
	{
		guard.unlock();
		return;
	}
	else
	{
		allT[n].tsk->complete = true;
		guard.unlock();
		for (int i = 0; i < allT[n].tsk->post.size(); i++)
		{
			addTask(allT[n].tsk->post[i]);
		}
		guard.lock();
		allT[n].tsk->post.clear();
		guard.unlock();
	}
}

int EquiCore::addPost(int n, EquiTask* tsk)
{
	unique_lock<mutex> guard(queueMutex, defer_lock);
	guard.lock();
	int id = -1;

	if (tsk->uid == -1)
	{
		id = uidc++;
		tsk->uid = id;
	}
	else
	{
		id = tsk->uid;
	}
	allT[id].tsk = tsk;

	if (allT.count(n) == 0)
	{
		guard.unlock();
	}
	else if (allT[n].tsk == NULL)
	{
		guard.unlock();
	}
	else
	{
		bool cmp = allT[n].tsk->complete;
		if (!cmp)
		{
			allT[n].tsk->post.push_back(tsk);
		}
		else
		{
			tasks.push_back(tsk);
		}
		guard.unlock();
	}

	return id;
}

void EquiCore::deadReference(int n)
{
	unique_lock<mutex> guard(queueMutex, defer_lock);
	guard.lock();
	allT[n].refCount = allT[n].refCount - 1;
	guard.unlock();
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
	/*
	cout << "Reg" << endl;
	for (int i = 0; i < registers.size(); i++)
	{
		while (registers[i].size() != 0)
		{
			pair<EquiObject*, bool> t = registers[i].top();
			registers[i].pop();
			if (t.second)
			{
				cout << "Deleting: [" << i << "] " << t.first << "[" << t.first->getType() << "]" <<  ":" << t.first->to_string() << endl;
				delete t.first;
			}
		}
	}
	registers.clear();
	*/

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
	
	for (int i = 0; i < apparentTok.size(); i++)
	{
		delete apparentTok[i].second;
	}

	if (frame != NULL)
		delete frame;

	frame = NULL;
}

