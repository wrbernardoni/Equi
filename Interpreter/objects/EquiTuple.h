#ifndef EQUI_TUPLE_OBJ_H_
#define EQUI_TUPLE_OBJ_H_

#include "EquiObject.h"
#include "EquiVoid.h"

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
};

#endif
