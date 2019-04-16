#ifndef EQUI_INTERFACE_
#define EQUI_INTERFACE_

#include "AllObj.h"

using namespace std;


class EQUI_get_type : public EquiFunction
{
public:
	EQUI_FN(EQUI_get_type)

	virtual EquiObject* operator() (EquiObject* in)
	{
		EquiString* s = new EquiString;
		s->setString(in->getType());

		return s;
	}
};

void loadInterface(map<string, EquiObject*>* tok)
{
	EQUI_get_type* getType = new EQUI_get_type;
	(*tok)["TYPE"] = getType;
};

#endif
