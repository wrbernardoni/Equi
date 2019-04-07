#ifndef EQUI_WORKER_H_
#define EQUI_WORKER_H_

#include "global.h"
#include "syntaxTree.h"
#include "EquiObject.h"

#include <map>
#include <string>

using namespace std;

class EquiWorker
{
private:
	map<string, EquiObject*> tokens;
	bool runElse;
public:
	EquiWorker();
	~EquiWorker();
	
	EquiObject* run(SyntaxTree*);
};

#endif