#ifndef EQUI_OBJECT_H_
#define EQUI_OBJECT_H_

#include "global.h"
#include <string>
#include <vector>
#include <typeinfo>
#include <map>

#define E_GENERIC_TYPE "generic"
#define E_VOID_TYPE "void"
#define E_TUPLE_TYPE "tuple"
#define E_PRIMITIVE_TYPE "primitive"
#define E_STRING_TYPE "string"
#define E_FUNCTION_TYPE "function"
#define E_ARRAY_TYPE "array"

using namespace std;

extern void throwError(string s);


class EquiObject
{
protected:
	void* data;
	map<string, EquiObject*> members;

	void clearMem()
	{
		for (auto i : members)
		{
			if (i.first != "this")
				delete i.second;
		}
	}

public:

	EquiObject()
	{
		members["this"] = this;
	};

	virtual ~EquiObject()
	{
		clearMem();
	};

	virtual EquiObject* spawnMyType() { return new EquiObject; };
	virtual EquiObject* clone() { return spawnMyType(); };

	virtual inline string getType() { return E_GENERIC_TYPE; };
	virtual string getDataType() 
	{ 
		return getType();
	}

	virtual bool operator== (EquiObject& o) { return false; }; 
	virtual bool operator!= (EquiObject& o) { return !(*this == o); };

	virtual bool operator> (EquiObject& o) { return false; };
	virtual bool operator>= (EquiObject& o) { return !(o > *this); }; 
	virtual bool operator< (EquiObject& o) { return o > *this; }; 
	virtual bool operator<= (EquiObject& o) { return !(o < *this); };
	
	virtual EquiObject& operator= (EquiObject& o)
	{
		throwError("Ill defined equality declaration between " + getType() + " types.");
		return *this;
	};
	

	virtual EquiObject* operator+ (EquiObject& o)
	{
		throwError("Cannot add to a " + getType() + " type.");
		EquiObject* n = new EquiObject;
		return n;
	};

	virtual EquiObject* operator- (EquiObject& o)
	{
		throwError("Cannot subtract from a " + getType() + " type.");
		EquiObject* n = new EquiObject;
		return n;
	};

	virtual EquiObject* operator* (EquiObject& o)
	{
		throwError("Cannot multiply to a " + getType() + " type.");
		EquiObject* n = new EquiObject;
		return n;
	};

	virtual EquiObject* operator/ (EquiObject& o)
	{
		throwError("Cannot divide from a " + getType() + " type.");
		EquiObject* n = new EquiObject;
		return n;
	};

	virtual EquiObject* operator% (EquiObject& o)
	{
		EquiObject* div = operator/(o);
		EquiObject* mult = o.operator*(*div);
		EquiObject* n = operator-(*mult);
		delete div;
		delete mult;
		return n;
	};

	virtual EquiObject* operator! ()
	{
		throwError("Cannot logical negate from a " + getType() + " type.");
		EquiObject* n = new EquiObject;
		return n;
	};

	virtual EquiObject* operator- ()
	{
		throwError("Cannot negate from a " + getType() + " type.");
		EquiObject* n = new EquiObject;
		return n;
	};

	virtual EquiObject* operator() (EquiObject*)
	{
		throwError("Cannot call a " + getType() + " as a function.");
		EquiObject* n = new EquiObject;
		return n;
	}

	virtual EquiObject& operator++()
	{
		throwError("Cannot increment " + getType() + ".");
		return *this;
	}

	virtual EquiObject& operator++(int)
	{
		return ++(*this);
	}

	virtual EquiObject& operator--()
	{
		throwError("Cannot decrement " + getType() + ".");
		return *this;
	}

	virtual EquiObject& operator--(int)
	{
		return --(*this);
	}

	virtual EquiObject* operator[](int)
	{
		throwError("Cannot index " + getType() + " as an array");
		EquiObject* n = new EquiObject;
		return n;
	}

	virtual EquiObject* operator[](string i)
	{
		if (members.count(i) == 0)
			throwError(getType() + " has no member named " + i);

		return members[i];
	}

	void setMember(string i, EquiObject* o)
	{
		if (members.count(i) == 0)
			throwError(getType() + " has no member named " + i);

		if (i != "this")
			delete members[i];

		members[i] = o;
	}

	virtual string to_string() { return "()"; };

};

template <typename T>
struct TypeName
{
    static const char* Get()
    {
        return typeid(T).name();
    }
};

template <>
struct TypeName<int>
{
    static const char* Get()
    {
        return "int";
    }
};

template <>
struct TypeName<bool>
{
    static const char* Get()
    {
        return "bool";
    }
};

template <>
struct TypeName<double>
{
    static const char* Get()
    {
        return "double";
    }
};

template <>
struct TypeName<float>
{
    static const char* Get()
    {
        return "float";
    }
};

template <>
struct TypeName<long>
{
    static const char* Get()
    {
        return "long";
    }
};


#endif