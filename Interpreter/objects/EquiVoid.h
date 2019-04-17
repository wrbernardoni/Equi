#ifndef EQUI_VOID_OBJ_H_
#define EQUI_VOID_OBJ_H_

#include "EquiObject.h"

class EquiVoid : public EquiObject
{
public:
	virtual EquiObject* spawnMyType() { return new EquiVoid; };
	virtual EquiObject* clone() { return new EquiVoid; };

	virtual inline string getType() { return E_VOID_TYPE; };
	virtual bool operator== (EquiObject& o) 
	{ 
		if (o.getType() == E_VOID_TYPE)
			return true;
		else
			return false;
	};

	virtual EquiObject& operator= (EquiObject& o)
	{
		if (o.getType() != E_VOID_TYPE)
			throwError("Cannot set a void to a non-Void value");
		return *this;
	}

	virtual string to_string() { return ""; };
};

#endif
