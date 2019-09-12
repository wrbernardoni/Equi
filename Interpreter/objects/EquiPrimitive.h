#ifndef EQUI_PRIMITIVE_OBJ_H_
#define EQUI_PRIMITIVE_OBJ_H_

#include "EquiObject.h"

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

	virtual EquiObject* getArray(int n)
	{
		EquiArray<EquiPrimitive<T>>* arr = new EquiArray<EquiPrimitive<T>>;
		for (int i = 0; i < n; i++)
		{
			arr->append(new EquiPrimitive<T>);
		}
		return arr;
	}

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
				throwError("Cannot compare a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
				return false;
			}
			if (getDataType() != "long" && getDataType() != "int" &&
				getDataType() != "double" && getDataType() != "float")
			{
				throwError("Cannot compare a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
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
				throwError("Cannot cast a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
			}
			if (getDataType() != "long" && getDataType() != "int" &&
				getDataType() != "double" && getDataType() != "float")
			{
				throwError("Cannot cast a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
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
				throwError("Cannot cast a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
			}
			if (getDataType() != "long" && getDataType() != "int" &&
				getDataType() != "double" && getDataType() != "float")
			{
				throwError("Cannot cast a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
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
				throwError("Cannot cast a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
			}
			if (getDataType() != "long" && getDataType() != "int" &&
				getDataType() != "double" && getDataType() != "float")
			{
				throwError("Cannot cast a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
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
				throwError("Cannot cast a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
			}
			if (getDataType() != "long" && getDataType() != "int" &&
				getDataType() != "double" && getDataType() != "float")
			{
				throwError("Cannot cast a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
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
				throwError("Cannot cast a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
			}
			if (getDataType() != "long" && getDataType() != "int" &&
				getDataType() != "double" && getDataType() != "float")
			{
				throwError("Cannot cast a numeric (" + getDataType() + ") to a non-numeric (" + o.getDataType() +")");
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

