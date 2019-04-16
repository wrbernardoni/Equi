#include "EquiString.h"
#include "EquiFunction.h"
#include "EquiPrimitive.h"

class E_STRING_at : public EquiFunction
{
private:
	EquiString* ths;
public:
	virtual string getDataType() { return "E_STRING_at"; };
	virtual EquiObject* clone() { return new E_STRING_at(ths); };
	virtual EquiObject* spawnMyType() { return new E_STRING_at; };

	E_STRING_at()
	{
		throwError("Cannot define string.at function outside a string");
		ths = NULL;
	}

	E_STRING_at(EquiString* p)
	{
		ths = p;
	}

	virtual EquiObject* operator() (EquiObject* in)
	{
		string ind = in->to_string();

		if (!isNum(ind))
		{
			throwError("string.at function must take a numeric type");
		}

		int i = stod(ind);

		string thsStr = ths->getString();

		if (i < 0 || i >= thsStr.size())
			throwError("String index out of bounds");

		EquiString* s = new EquiString;
		string a = "";
		a = a + thsStr[i];
		s->setString(a);
		return s;
	}
};

class E_STRING_size : public EquiFunction
{
private:
	EquiString* ths;
public:
	virtual string getDataType() { return "E_STRING_size"; };
	virtual EquiObject* clone() { return new E_STRING_size(ths); };
	virtual EquiObject* spawnMyType() { return new E_STRING_size; };

	E_STRING_size()
	{
		throwError("Cannot define string.size function outside a string");
		ths = NULL;
	}

	E_STRING_size(EquiString* p)
	{
		ths = p;
	}

	virtual EquiObject* operator() (EquiObject* in)
	{
		if (in->getType() != E_VOID_TYPE)
			throwError("No input expected in string.size function.");

		string thsStr = ths->getString();

		EquiPrimitive<int>* n = new EquiPrimitive<int>;
		n->setData(thsStr.size());
		return n;
	}
};

EquiString::EquiString()
{
	data = new string;
	members["at"] = new E_STRING_at(this);
	members["size"] = new E_STRING_size(this);
};