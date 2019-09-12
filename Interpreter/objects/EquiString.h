#ifndef EQUI_STRING_OBJ_H_
#define EQUI_STRING_OBJ_H_

#include "EquiObject.h"
#include "EquiArray.h"

class EquiString : public EquiObject
{
private:

public:
	EquiString();
	
	~EquiString()
	{
		string* s = (string*)data;
		delete s;
	};

	string getString()
	{
		string* s = (string*)data;
		return *s;
	}

	virtual EquiObject* getArray(int n)
	{
		EquiArray<EquiString>* arr = new EquiArray<EquiString>;
		for (int i = 0; i < n; i++)
		{
			arr->append(new EquiString);
		}
		return arr;
	}

	void setString(string nu)
	{
		string* s = (string*)data;
		*s = nu;
	}

	virtual inline string getType() { return E_STRING_TYPE; };

	virtual EquiObject* clone() 
	{ 
		EquiString* newS = new EquiString;
		newS->setString(getString());
		return newS;
	};

	virtual EquiObject* spawnMyType() { return new EquiString; };

	virtual bool operator== (EquiObject& o)
	{ 
		if (o.getType() == E_STRING_TYPE)
			return getString() == o.to_string();
		else
			return false;
	};

	virtual bool operator> (EquiObject& o)
	{ 
		if (o.getType() != E_STRING_TYPE)
			throwError("Cannot logically compare strings and non strings");
		return getString() > o.to_string(); 
	};

	virtual EquiObject& operator= (EquiObject& o)
	{
		setString(o.to_string());
		return *this;
	};

	virtual EquiObject* operator+ (EquiObject& o)
	{
		EquiString* n = new EquiString;
		string s = getString() + o.to_string();
		n->setString(s);
		return n;
	};

	virtual string to_string() { return getString(); };
};

#endif
