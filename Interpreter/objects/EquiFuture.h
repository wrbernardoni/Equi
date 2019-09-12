#ifndef EQUI_FUTURE_H_
#define EQUI_FUTURE_H_

#include "EquiObject.h"
#include "EquiArray.h"
#include <iostream>

class EquiFuture : public EquiObject
{
private:
	int uid;
public:
	void setID(int i) { uid = i; };
	int getID() { return uid; };

	~EquiFuture()
	{
		globalCore->deadReference(uid);
	}

	virtual EquiObject* spawnMyType() { return new EquiFuture; };
	virtual EquiObject* clone() 
	{ 
		EquiFuture* f = new EquiFuture;
		f->setID(uid);
		return f;
	};

	virtual EquiObject* getArray(int n)
	{
		EquiArray<EquiFuture>* arr = new EquiArray<EquiFuture>;
		for (int i = 0; i < n; i++)
		{
			arr->append(new EquiFuture);
		}
		return arr;
	}

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
			globalCore->deadReference(uid);
			uid = f->getID();
			globalCore->addReference(uid);
		}
		return *this;
	}

	virtual string to_string() { return "UNEVALUATED_FUTURE"; };

private:
	class E_FUTURE_get : public EquiFunction
	{
	private:
		
	public:
		EquiFuture* ths;
		bool own;
		virtual EquiObject* clone() { return new E_FUTURE_get(ths); };
		virtual EquiObject* spawnMyType() { return new E_FUTURE_get; };

		E_FUTURE_get()
		{
			ths = NULL;
			own = true;
		}

		E_FUTURE_get(EquiFuture* p)
		{
			ths = p;
			own = false;
		}

		virtual EquiObject* operator() (EquiObject* in)
		{
			int n = ths->getID();

			if (!globalCore->isComplete(n))
			{
				throw n;
			}

			EquiObject* s = globalCore->getResult(n);
			s = s->clone();
			return s;
		}
	};

public:
	EquiFuture()
	{
		members["get"] = new E_FUTURE_get(this);
	};
};

#endif

