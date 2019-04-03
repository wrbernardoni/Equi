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
	map<string, EquiObject> tokens;
public:
	EquiObject* run(SyntaxTree*);
};

#endif
