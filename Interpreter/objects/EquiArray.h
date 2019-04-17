#ifndef EQUI_ARRAY_OBJ_H_
#define EQUI_ARRAY_OBJ_H_

#include "EquiObject.h"

template <class T>
class EquiArray : public EquiObject
{
private:
	vector<T*>* formatData()
	{
		return (vector<T*>*)data;
	}
public:

	virtual inline string getType() { return E_ARRAY_TYPE; };
	virtual string getDataType() 
	{ 
		T t;
		return t.getType();
	}

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
		if (i >= nV->size() || i < 0)
			throwError("Array index out of bounds");

		return (*nV)[i];
	}

	virtual EquiObject& operator= (EquiObject& o)
	{
		if (o.getType() != E_ARRAY_TYPE)
			throwError("Cannot set an array to be a non-array");
		else if (o.getDataType() != getDataType())
			throwError(o.getDataType() + " is the wrong type of array to set this to, must be " + getDataType());
		
		EquiArray* oArr = (EquiArray<T>*) &o;

		vector<T*>* nV = formatData();
		for (int i = 0; i < nV->size(); i++)
			delete (*nV)[i];

		delete nV;

		nV = new vector<T*>;
		data = (void*)nV;

		vector<T*> toBeArray = oArr->getArray();

		for (int i = 0; i < toBeArray.size(); i++)
		{
			T* pB = (T*)toBeArray[i]->clone();
			nV->push_back(pB);
		}

		return *this;
	};

	virtual EquiObject* spawnMyType() { return new EquiArray<T>; };

	virtual EquiObject* clone()
	{ 
		EquiArray<T>* newAr = new EquiArray<T>;
		vector<T*>* nV = formatData();
		for (int i = 0; i < nV->size(); i++)
			newAr->append((T*)((*nV)[i])->clone());

		return newAr;
	};

	virtual string to_string()
	{ 
		string s = "[";
		vector<T*>* nV = formatData();	
		for (int i = 0; i < nV->size(); i++)
		{
			s += (*nV)[i]->to_string();
			if (i != nV->size() - 1)
				s += ", ";
		}
		s += "]";
		return s;
	};
};

#endif

