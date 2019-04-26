#ifndef EQUI_FUNCTION_HANDLE_H_
#define EQUI_FUNCTION_HANDLE_H_

#include "AllObj.h"
#include "EquiFunction.h"

class EquiFunctionHandle : public EquiFunction
{
private:
	EquiFunction* f;
public:
	EquiFunction* cloneF()
	{
		if (f == NULL)
			return NULL;
		return (EquiFunction*)f->clone();
	}

	EquiFunctionHandle()
	{
		f = NULL;
	}

	~EquiFunctionHandle()
	{
		if (f != NULL)
			delete f;
	}

	virtual inline string getType() { return E_FUNCTION_TYPE; };
	virtual inline string getDataType() { return E_FUNCTION_HANDLE_TYPE; };

	virtual EquiObject* spawnMyType() { return new EquiFunctionHandle; };
	virtual EquiObject* clone()
	{
		return spawnMyType();
	}

	virtual EquiObject* operator() (EquiObject* in)
	{
		if (f == NULL)
		{
			throwError("Function handle pointing to nothing");
		}

		return (*f)(in);
	}

	virtual EquiObject& operator= (EquiObject& o)
	{
		if (o.getDataType() == E_FUNCTION_TYPE)
		{
			if (f != NULL)
				delete f;
			f = (EquiFunction*)o.clone();
		}
		else if (o.getDataType() == E_FUNCTION_HANDLE_TYPE)
		{
			EquiFunctionHandle* fH = (EquiFunctionHandle*)&o;
			if (f != NULL)
				delete f;
			f = fH->cloneF();
		}
		else
		{
			throwError("Cannot assign a " + o.getType() + " to a function handler");
		}

		return *this;
	}
};

#endif
