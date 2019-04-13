#ifndef EQUI_OBJECT_H_
#define EQUI_OBJECT_H_

#include "global.h"
#include <string>
#include <vector>
#include <typeinfo>

#define E_GENERIC_TYPE "generic"
#define E_VOID_TYPE "void"
#define E_TUPLE_TYPE "tuple"
#define E_PRIMITIVE_TYPE "primitive"
#define E_STRING_TYPE "string"
#define E_FUNCTION_TYPE "function"

using namespace std;

extern void throwError(string s);


class EquiObject
{
public:
	EquiObject() {};
	virtual ~EquiObject() {};

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

	virtual string to_string() { return "()"; };
protected:
	void* data;
};

template <class T>
class EquiArray : public EquiObject
{
private:
	vector<T*>* formatData()
	{
		return (vector<T*>*)data;
	}
public:
	EquiArray()
	{
		vector<T*>* nV = new vector<T*>;
		data = (void*)nV;
	}

	virtual ~EquiArray()
	{
		vector<T*>* nV = formatData();
		for (int i = 0; i < nV->size(); i++)
			delete (*nV)[i];

		delete nV;
	}

	vector<T*> getArray()
	{
		vector<T*>* nV = formatData();
		return *nV;
	}

	void append(T* nT)
	{
		vector<T*>* nV = formatData();
		nV->push_back(nT);
	}

	virtual EquiObject* operator[](int i)
	{
		vector<T*>* nV = formatData();
		if (i >= nV->size())
			throwError("Array index out of bounds");

		return (*nV)[i];
	}

	virtual EquiObject* spawnMyType() { return new EquiArray<T>; };

	virtual EquiObject* clone()
	{ 
		EquiArray<T>* newAr = new EquiArray<T>;
		vector<T*>* nV = formatData();
		for (int i = 0; i < nV->size(); i++)
			newAr->append((T*)((*nV)[i])->clone());

		return newAr;
	};
};

#define EQUI_FN(a) virtual string getDataType() { return #a; }; \
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

	virtual EquiObject* operator() (EquiObject*)
	{
		throwError("Function " + getDataType() + " undefined.");
		EquiObject* n = new EquiObject;
		return n;
	}

	virtual string to_string()
	{ 
		throwError("Cannot stringify a function reference");
		return ""; 
	};
};

class EquiString : public EquiObject
{
private:

public:
	EquiString()
	{
		data = new string;
	};
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
	virtual EquiObject* clone() 
	{ 
		EquiTuple* newTup = new EquiTuple;
		vector<EquiObject*> tuple = newTup->getTuple();
		vector<EquiObject*> me = *((vector<EquiObject*>*)data);
		for (int i = 0; i < me.size(); i++)
		{
			EquiObject* e = me[i]->spawnMyType();
			*e = *(me[i]);
			tuple.push_back(e);
		}
		newTup->setTuple(tuple);
		return newTup;
	};

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

	virtual EquiObject* operator+ (EquiObject& o)
	{
		EquiTuple* newTup = (EquiTuple*)clone();//new EquiTuple;
		vector<EquiObject*> tuple = newTup->getTuple();
		
		if (o.getType() == E_TUPLE_TYPE)
		{
			EquiTuple* oTup = (EquiTuple*)&o;
			vector<EquiObject*> oT = oTup->getTuple();
			
			for (int i = 0; i < oT.size(); i++)
			{
				EquiObject* e = oT[i]->spawnMyType();
				*e = *(oT[i]);
				tuple.push_back(e);
			}		
		}
		else
		{
			EquiObject* e = o.spawnMyType();
			*e = o;
			tuple.push_back(e);
		}

		newTup->setTuple(tuple);
		
		return newTup;
	};

	virtual EquiObject* operator- (EquiObject& o)
	{
		throwError("Cannot subtract from a tuple.");
		EquiVoid* v = new EquiVoid;
		return v;
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
	virtual EquiObject* clone() 
	{ 
		EquiPrimitive<T>* newT = new EquiPrimitive<T>;
		newT->setData(getData());
		return newT; 
	};

	EquiPrimitive()
	{
		data = new T;
	};
	~EquiPrimitive()
	{
		delete ((T*)data);
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


			T d;
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


			T d;
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


			T d;
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

	virtual EquiObject* operator+ (EquiObject& o)
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


			T d;
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

			T out = *((T*)data) + d;
			EquiPrimitive<T>* newT = new EquiPrimitive<T>;
			newT->setData(out);
			return newT;
		}
		else
		{
			EquiPrimitive<T>* oTup = (EquiPrimitive<T>*)&o;
			T out = *((T*)data) + oTup->getData();
			EquiPrimitive<T>* newT = new EquiPrimitive<T>;
			newT->setData(out);
			return newT;
		}
	}

	virtual EquiObject* operator- (EquiObject& o)
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


			T d;
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

			T out = *((T*)data) - d;
			EquiPrimitive<T>* newT = new EquiPrimitive<T>;
			newT->setData(out);
			return newT;
		}
		else
		{
			EquiPrimitive<T>* oTup = (EquiPrimitive<T>*)&o;
			T out = *((T*)data) - oTup->getData();
			EquiPrimitive<T>* newT = new EquiPrimitive<T>;
			newT->setData(out);
			return newT;
		}
	}

	virtual EquiObject* operator* (EquiObject& o)
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


			T d;
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

			T out = *((T*)data) * d;
			EquiPrimitive<T>* newT = new EquiPrimitive<T>;
			newT->setData(out);
			return newT;
		}
		else
		{
			EquiPrimitive<T>* oTup = (EquiPrimitive<T>*)&o;
			T out = *((T*)data) * oTup->getData();
			EquiPrimitive<T>* newT = new EquiPrimitive<T>;
			newT->setData(out);
			return newT;
		}
	}

	virtual EquiObject* operator/ (EquiObject& o)
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


			T d;
			long double d0;
			if (o.getDataType() == "long")
			{
				EquiPrimitive<long>* oTup = (EquiPrimitive<long>*)&o;
				d0 = oTup->getData();
				d = oTup->getData();
			}
			else if (o.getDataType() == "int")
			{
				EquiPrimitive<int>* oTup = (EquiPrimitive<int>*)&o;
				d0 = oTup->getData();
				d = oTup->getData();
			}
			else if (o.getDataType() == "double")
			{
				EquiPrimitive<double>* oTup = (EquiPrimitive<double>*)&o;
				d0 = oTup->getData();
				d = oTup->getData();
			}
			else if (o.getDataType() == "float")
			{
				EquiPrimitive<float>* oTup = (EquiPrimitive<float>*)&o;
				d0 = oTup->getData();
				d = oTup->getData();
			}

			if (d0 == 0)
				throwError("Divide by 0");

			T out = *((T*)data) / d;
			EquiPrimitive<T>* newT = new EquiPrimitive<T>;
			newT->setData(out);
			return newT;
		}
		else
		{
			EquiPrimitive<T>* oTup = (EquiPrimitive<T>*)&o;
			T out = *((T*)data) / oTup->getData();
			EquiPrimitive<T>* newT = new EquiPrimitive<T>;
			newT->setData(out);
			return newT;
		}
	}

	virtual EquiObject* operator! ()
	{
		EquiPrimitive<T>* newT = new EquiPrimitive<T>;
		newT->setData(!getData());
		return newT;
	};

	virtual EquiObject* operator- ()
	{
		EquiPrimitive<T>* newT = new EquiPrimitive<T>;
		newT->setData(-getData());
		return newT;
	};

	virtual EquiObject& operator-- ()
	{
		setData(getData() - 1);
		return *this;
	};

	virtual EquiObject& operator++ ()
	{
		setData(getData() + 1);
		return *this;
	};


	virtual string to_string() {
		return std::to_string(*((T*)data));
	};
};

#endif
