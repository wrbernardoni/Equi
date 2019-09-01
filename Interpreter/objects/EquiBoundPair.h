#ifndef EQUI_BOUND_PAIR_H_
#define EQUI_BOUND_PAIR_H_

#include "EquiObject.h"

class EquiBoundPair : public EquiObject
{
private:
	EquiObject* parent;
	EquiObject* child;

public:
	EquiBoundPair(EquiObject* p, EquiObject* c)
	{
		parent = p;
		child = c;
	}

	~EquiBoundPair()
	{
		delete parent;
	}

	virtual EquiObject* spawnMyType() { return child->spawnMyType(); };
	virtual EquiObject* clone() 
	{ 
		throwError("Cannot clone a bound object");
		return NULL;
	};

	virtual EquiObject* operator+(EquiObject& o)
	{
		return *child + o;
	}

	virtual EquiObject* operator-(EquiObject& o)
	{
		return *child - o;
	}

	virtual EquiObject* operator*(EquiObject& o)
	{
		return *child * o;
	}

	virtual EquiObject* operator/(EquiObject& o)
	{
		return *child / o;
	}

	virtual EquiObject* operator%(EquiObject& o)
	{
		return *child % o;
	}

	virtual EquiObject* operator!()
	{
		return !*child;
	}

	virtual EquiObject* operator-()
	{
		return -*child;
	}

	virtual EquiObject* operator() (EquiObject* o)
	{
		return (*child)(o);
	}

	virtual EquiObject* operator() (EquiObject* i, vector<pair<string, EquiObject*>> n)
	{
		return (*child)(i, n);
	}

	virtual EquiObject& operator++()
	{
		return (*child)++;
	}

	virtual EquiObject& operator++(int)
	{
		return ++(*child);
	}

	virtual EquiObject& operator--()
	{
		return (*child)--;
	}

	virtual EquiObject& operator--(int)
	{
		return --(*this);
	}

	virtual EquiObject* operator[](int n)
	{
		return (*child)[n];
	}

	virtual EquiObject* operator[](string i)
	{

		return (*child)[i];
	}

	virtual string to_string() { return child->to_string(); };
};

#endif
