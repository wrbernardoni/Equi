#ifndef EQUI_OBJECT_H_
#define EQUI_OBJECT_H_

#include "global.h"
#include <string>
#include <vector>

#define E_GENERIC_TYPE "generic"
#define E_VOID_TYPE "void"
#define E_TUPLE_TYPE "tuple"
#define E_BOOL_TYPE "bool"
#define E_PRIMITIVE_TYPE "primitive"

using namespace std;

extern void throwError(string s);


class EquiObject
{
public:
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

	virtual string to_string() { return "()"; };
private:

};

class EquiVoid : public EquiObject
{
public:
	virtual inline string getType() { return E_VOID_TYPE; };
	virtual bool operator== (EquiObject& o) 
	{ 
		if (o.getType() == E_VOID_TYPE)
			return true;
		else
			return false;
	};
	virtual string to_string() { return "VOID"; };
};

class EquiTuple : public EquiObject
{
private:
	vector<EquiObject*> tuple;
public:
	~EquiTuple()
	{
		for (int i = 0; i < tuple.size(); i++)
			delete tuple[i];
	};

	virtual inline string getType() { return E_TUPLE_TYPE; };

	inline void setTuple(vector<EquiObject*> o) { tuple = o; };
	inline vector<EquiObject*> getTuple() { return tuple; };

	virtual bool operator== (EquiObject& o) 
	{
		if (o.getType() != E_TUPLE_TYPE)
			return false;
		EquiTuple* oTup = (EquiTuple*)&o;
		vector<EquiObject*> oT = oTup->getTuple();
		for (int i = 0; i < MIN(oT.size(), tuple.size()); i++)
		{
			if (*oT[i] != *tuple[i])
				return false;
		}

		return true; 
	};

	virtual bool operator> (EquiObject& o) 
	{ 
		throwError("Tuples may not be compared in this way");
		return false; 
	};
	virtual bool operator>= (EquiObject& o)
	{ 
		throwError("Tuples may not be compared in this way");
		return false; 
	};
	virtual bool operator< (EquiObject& o)
	{ 
		throwError("Tuples may not be compared in this way");
		return false; 
	}; 
	virtual bool operator<= (EquiObject& o)
	{ 
		throwError("Tuples may not be compared in this way");
		return false; 
	};

	virtual string to_string() {
		string s = "{";
		for (int i = 0; i < tuple.size(); i++)
		{
			s += tuple[i]->to_string();
			if (i != tuple.size() - 1)
				s += ", ";
		}
		s += "}";
		return s;
	};
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

template <class T>
class EquiPrimitive : public EquiObject
{
private:
	T data;
public:
	void setData(T t) { data = t; };
	T getData() {return data; };
	virtual string getDataType() 
	{ 
		TypeName<T> c; 
		return c.Get(); 
	}

	virtual inline string getType() 
	{ 
		TypeName<T> c;
		string s = E_PRIMITIVE_TYPE;
		s += "<";
		s += c.Get();
		s += ">";
		return s; 
	};

	virtual bool operator== (EquiObject& o) 
	{
		if (o.getType() != getType())
			return false;
		EquiPrimitive<T>* oTup = (EquiPrimitive<T>*)&o;
		return data == oTup->getData();
	};

	virtual bool operator> (EquiObject& o) 
	{ 
		if (o.getType() != getType())
		{
			if (o.getDataType() != "long" && o.getDataType() != "int" &&
				o.getDataType() != "double" && o.getDataType() != "float")
			{
				throwError("Unsupported type comparison");
				return false;
			}
			if (getDataType() != "long" && getDataType() != "int" &&
				getDataType() != "double" && getDataType() != "float")
			{
				throwError("Unsupported type comparison");
				return false;
			}


			long double d;
			if (o.getDataType() == "long")
			{
				EquiPrimitive<long>* oTup = (EquiPrimitive<long>*)&o;
				d = oTup->getData();
			}
			else if (o.getDataType() == "int")
			{
				EquiPrimitive<int>* oTup = (EquiPrimitive<int>*)&o;
				d = oTup->getData();
			}
			else if (o.getDataType() == "double")
			{
				EquiPrimitive<double>* oTup = (EquiPrimitive<double>*)&o;
				d = oTup->getData();
			}
			else if (o.getDataType() == "float")
			{
				EquiPrimitive<float>* oTup = (EquiPrimitive<float>*)&o;
				d = oTup->getData();
			}

			return data > d;
		}

		EquiPrimitive<T>* oTup = (EquiPrimitive<T>*)&o;
		return data > oTup->getData();
	};

	virtual string to_string() {
		return std::to_string(data);
	};
};

#endif
