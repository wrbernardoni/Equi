#include "EquiWorker.h"
#include <iostream>
#include <cmath>
#include <stack>

#include "EquiCustomFunction.h"
#include "compile.h"

#include "AllObj.h"
#include <map>
#include <string>
#include <deque>

#include "EquiCore.h"

extern bool isNum(string s);
extern bool isString(string s);

pair<EquiObject*, bool> EquiWorker::evalTask(EquiTask* tsk)
{
	if (tsk->code == NULL)
	{
		pair<EquiObject*, bool> o((*tsk->fn)(tsk->inp, tsk->apparentTok), true);
		return o;
	}
	else
	{
		pLine = -1;
		lineCount = tsk->lineCount;
		scopeSince = tsk->scopeSince;
		breakFlag = tsk->breakFlag;
		continueFlag = tsk->continueFlag;
		elseFlag = tsk->elseFlag;
		delete data;
		data = tsk->frame;
		registers = tsk->registers;
		resF = tsk->resumeAt;
		pair<EquiObject*, bool> out = runCodeLine(tsk->code);
		data = new EquiFrame;
		return out;
	}
}

