#ifndef EQUI_OBJECT_H_
#define EQUI_OBJECT_H_

#include "global.h"
#include <string>
#include <vector>
#include <typeinfo>

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
	virtual EquiObject* spawnMyType() { return new EquiObject; };

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
		throwError("Ill defined equality declaration between generic types");
		return *this;
	};

	virtual string to_string() { return "()"; };
protected:
	void* data;
};

class EquiVoid : public EquiObject
{
public:
	virtual EquiObject* spawnMyType() { return new EquiVoid; };

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

	virtual string to_string() { return "VOID"; };
};

class EquiTuple : public EquiObject
{
private:
	void purgeData()
	{
		vector<EquiObject*>* tuple = ((vector<EquiObject*>*) data);
		for (int i = 0; i < tuple->size(); i++)
			delete (*tuple)[i];
		tuple->clear();	
	};
public:
	virtual EquiObject* spawnMyType() { return new EquiTuple; };

	EquiTuple()
	{
		data = new vector<EquiObject*>;
	}

	~EquiTuple()
	{
		purgeData();
		vector<EquiObject*>* tuple = ((vector<EquiObject*>*) data);	
		delete tuple;
	};

	virtual inline string getType() { return E_TUPLE_TYPE; };

	inline void setTuple(vector<EquiObject*> o)
	{
		vector<EquiObject*>* tuple = (vector<EquiObject*>*) data;
		*tuple = o; 
	};
	inline vector<EquiObject*> getTuple()
	{ 
		vector<EquiObject*>* tuple = (vector<EquiObject*>*) data;
		return *tuple; 
	};

	virtual bool operator== (EquiObject& o) 
	{
		if (o.getType() != E_TUPLE_TYPE)
			return false;
		EquiTuple* oTup = (EquiTuple*)&o;
		vector<EquiObject*> oT = oTup->getTuple();
		vector<EquiObject*>* tuple = (vector<EquiObject*>*) data;
		if (oT.size() != tuple->size())
			return false;
		for (int i = 0; i < tuple->size(); i++)
		{
			if (*oT[i] != *(*tuple)[i])
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

	virtual EquiObject& operator= (EquiObject& o)
	{
		purgeData();

		if (o.getType() == E_TUPLE_TYPE)
		{
			EquiTuple* oTup = (EquiTuple*)&o;
			vector<EquiObject*> oT = oTup->getTuple();
			vector<EquiObject*>* tuple = (vector<EquiObject*>*) data;
			for (int i = 0; i < oT.size(); i++)
			{
				EquiObject* e = oT[i]->spawnMyType();
				*e = *(oT[i]);
				tuple->push_back(e);
			}						
		}
		else
		{
			EquiObject* e = o.spawnMyType();
			*e = o;
			vector<EquiObject*>* tuple = (vector<EquiObject*>*) data;
			tuple->push_back(e);
		}
		return *this;
	};

	virtual string to_string() {
		string s = "{";
		vector<EquiObject*>* tuple = (vector<EquiObject*>*) data;	
		for (int i = 0; i < tuple->size(); i++)
		{
			s += (*tuple)[i]->to_string();
			if (i != tuple->size() - 1)
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
public:
	virtual EquiObject* spawnMyType() { return new EquiPrimitive<T>; };

	EquiPrimitive()
	{
		data = new T;
	};
	~EquiPrimitive()
	{
		delete data;
	}

	void setData(T t) { *((T*)data) = t; };
	T getData() {return *((T*)data); };
	virtual string getDataType() 
	{ 
		TypeName<T> c; 
		return c.Get(); 
	}

	virtual inline string getType() 
	{ 
		return E_PRIMITIVE_TYPE; 
	};

	virtual bool operator== (EquiObject& o) 
	{
		if (o.getDataType() != getDataType())
		{
			if (o.getDataType() != "long" && o.getDataType() != "int" &&
				o.getDataType() != "double" && o.getDataType() != "float")
			{
				//throwError("Unsupported type comparison");
				return false;
			}
			if (getDataType() != "long" && getDataType() != "int" &&
				getDataType() != "double" && getDataType() != "float")
			{
				//throwError("Unsupported type comparison");
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

			return *((T*)data) == d;
		}

		EquiPrimitive<T>* oTup = (EquiPrimitive<T>*)&o;
		return *((T*)data) == oTup->getData();
	};

	virtual bool operator> (EquiObject& o) 
	{ 
		if (o.getDataType() != getDataType())
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

			return *((T*)data) > d;
		}

		EquiPrimitive<T>* oTup = (EquiPrimitive<T>*)&o;
		return *((T*)data) > oTup->getData();
	};

	virtual EquiObject& operator= (EquiObject& o) 
	{
		if (o.getDataType() != getDataType())
		{
			if (o.getDataType() != "long" && o.getDataType() != "int" &&
				o.getDataType() != "double" && o.getDataType() != "float")
			{
				throwError("Cannot cast a numeric to a non-numeric");
			}
			if (getDataType() != "long" && getDataType() != "int" &&
				getDataType() != "double" && getDataType() != "float")
			{
				throwError("Cannot cast a numeric to a nonnumeric");
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

			*((T*)data) = d;
		}
		else
		{
			EquiPrimitive<T>* oTup = (EquiPrimitive<T>*)&o;
			*((T*)data) = oTup->getData();
		}

		return *this;
	};

	virtual string to_string() {
		return std::to_string(*((T*)data));
	};
};

#endif
