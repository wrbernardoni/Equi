#ifndef EQUI_FUNCTION_OBJ_H_
#define EQUI_FUNCTION_OBJ_H_

#include "EquiObject.h"

#define EQUI_FN(a) virtual string getDataType() { return E_FUNCTION_TYPE; }; \
	virtual EquiObject* clone() { return new a; }; \
	virtual EquiObject* spawnMyType() { return new a; }; 

class EquiFunction : public EquiObject
{
public:
	virtual inline string getType() { return E_FUNCTION_TYPE; };
	EQUI_FN(EquiFunction)
	virtual bool operator== (EquiObject& o) { return (getType() == o.getType()) && (getDataType() == o.getDataType()); };

	virtual bool operator> (EquiObject& o)
	{ 
		throwError("Cannot do a logical comparison on a function reference.");
		return false; 
	};

	virtual EquiObject& operator= (EquiObject& o)
	{
		//TODO: Make this do something
		//throwError("Ill defined equality declaration between " + getType() + " types.");
		return *this;
	};

	virtual EquiObject* operator() (EquiObject*)
	{
		throwError("Function " + getDataType() + " undefined.");
		EquiObject* n = new EquiObject;
		return n;
	}

	virtual string to_string()
	{ 
		return "FUNCTION"; 
	};
};

#endif

