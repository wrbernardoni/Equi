#ifndef EQUI_OBJECT_H_
#define EQUI_OBJECT_H_

#include "global.h"
#include <string>

#define E_GENERIC_TYPE "generic"
#define E_VOID_TYPE "void"

using namespace std;

class EquiObject
{
public:
	virtual inline string getType() { return E_GENERIC_TYPE; };
private:

};

class EquiVoid : public EquiObject
{
public:
	virtual inline string getType() { return E_VOID_TYPE; };
};

#endif
