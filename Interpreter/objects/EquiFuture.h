#ifndef EQUI_FUTURE_H_
#define EQUI_FUTURE_H_

#include "EquiObject.h"

class EquiFuture : public EquiObject
{
private:
	int uid;
public:
	void setID(int i) { uid = i; };
	int getID() { return uid; };

	virtual EquiObject* spawnMyType() { return new EquiFuture; };
	virtual EquiObject* clone() 
	{ 
		EquiFuture* f = new EquiFuture;
		f->setID(uid);
		return f;
	};

	virtual inline string getType() { return E_FUTURE_TYPE; };
	virtual bool operator== (EquiObject& o) 
	{ 
		throwError("Cannot evaluate a future, use .get()");
		return false;
	};

	virtual EquiObject& operator= (EquiObject& o)
	{
		if (o.getType() != E_FUTURE_TYPE)
			throwError("Cannot evaluate a future, use .get()");
		else
		{
			EquiFuture* f = (EquiFuture*)&o;
			uid = f->getID();
		}
		return *this;
	}

	virtual string to_string() { return "UNEVALUATED_FUTURE"; };
};

#endif

