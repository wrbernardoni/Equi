#ifndef EQUI_WORKER_H_
#define EQUI_WORKER_H_

#include "global.h"
#include "syntaxTree.h"
#include "EquiObject.h"

class EquiWorker
{
private:

public:
	EquiObject run(SyntaxTree*);
};

#endif
