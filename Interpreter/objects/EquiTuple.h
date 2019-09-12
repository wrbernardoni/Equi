#ifndef EQUI_TUPLE_OBJ_H_
#define EQUI_TUPLE_OBJ_H_

#include "EquiObject.h"
#include "EquiVoid.h"
#include "EquiArray.h"

class EquiTuple : public EquiObject
{
private:
	void purgeData()
	{
		vector<EquiObject*>* tuple = ((vector<EquiObject*>*) data);
		for (int i = 0; i < tuple->size(); i++)
		{
			if ((*tuple)[i]->getTemp())
				delete (*tuple)[i];
		}
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

	virtual EquiObject* getArray(int n)
	{
		EquiArray<EquiTuple>* arr = new EquiArray<EquiTuple>;
		for (int i = 0; i < n; i++)
		{
			arr->append(new EquiTuple);
		}
		return arr;
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
		purgeData();
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
		EquiTuple* newTup = new EquiTuple;
		vector<EquiObject*> tuple = getTuple();
		vector<EquiObject*> newT;
		for (int i = 0; i < tuple.size(); i++)
		{
			EquiObject* e = tuple[i]->spawnMyType();
			*e = *(tuple[i]);
			newT.push_back(e);
		}
		
		if (o.getType() == E_TUPLE_TYPE)
		{
			EquiTuple* oTup = (EquiTuple*)&o;
			vector<EquiObject*> oT = oTup->getTuple();
			
			for (int i = 0; i < oT.size(); i++)
			{
				EquiObject* e = oT[i]->spawnMyType();
				*e = *(oT[i]);
				newT.push_back(e);
			}		
		}
		else
		{
			EquiObject* e = o.spawnMyType();
			*e = o;
			newT.push_back(e);
		}

		newTup->setTuple(newT);
		
		return newTup;
	};

	virtual EquiObject* operator- (EquiObject& o)
	{
		throwError("Cannot subtract from a tuple.");
		EquiVoid* v = new EquiVoid;
		return v;
	};

	virtual EquiObject* operator[](int i)
	{
		vector<EquiObject*> nV = getTuple();
		if (i >= nV.size() || i < 0)
			throwError("Array index out of bounds");

		return nV[i];
	}

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

	virtual int maxIndex()
	{
		return getTuple()->size();
	}

private:
	class E_TUPLE_at : public EquiFunction
	{
	private:
		EquiTuple* ths;
	public:
		virtual EquiObject* clone() { return new E_TUPLE_at(ths); };
		virtual EquiObject* spawnMyType() { return new E_TUPLE_at; };

		E_TUPLE_at()
		{
			throwError("Cannot define tuple.at function outside a tuple");
			ths = NULL;
		}

		E_TUPLE_at(EquiTuple* p)
		{
			ths = p;
		}

		virtual EquiObject* operator() (EquiObject* in)
		{
			string ind = in->to_string();

			if (!isNum(ind))
			{
				throwError("t.at function must take a numeric type");
			}

			int i = stod(ind);

			vector<EquiObject*> thsArr = ths->getTuple();

			if (i < 0 || i >= thsArr.size())
				throwError("String index out of bounds");

			EquiObject* s = thsArr[i]->clone();
			return s;
		}
	};

	class E_TUPLE_size : public EquiFunction
	{
	private:
		EquiTuple* ths;
	public:
		virtual EquiObject* clone() { return new E_TUPLE_size(ths); };
		virtual EquiObject* spawnMyType() { return new E_TUPLE_size; };

		E_TUPLE_size()
		{
			throwError("Cannot define tuple.size function outside a tuple");
			ths = NULL;
		}

		E_TUPLE_size(EquiTuple* p)
		{
			ths = p;
		}

		virtual EquiObject* operator() (EquiObject* in)
		{
			if (in->getType() != E_VOID_TYPE)
				throwError("No input expected in array.size function.");

			vector<EquiObject*> thsArr = ths->getTuple();

			EquiPrimitive<int>* n = new EquiPrimitive<int>;
			n->setData(thsArr.size());
			return n;
		}
	};

public:
	EquiTuple()
	{
		members["at"] = new E_TUPLE_at(this);
		members["size"] = new E_TUPLE_size(this);

		data = new vector<EquiObject*>;
	};
};

#endif
