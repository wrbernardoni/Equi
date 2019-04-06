#include "../EquiObject.h"
#include <map>
#include <iostream>

using namespace std;

class cnsl_std_EquiPrint : public EquiFunction
{
public:
	EQUI_FN(cnsl_std_EquiPrint)

	virtual EquiObject* operator() (EquiObject* in)
	{
		if (in->getType() != E_TUPLE_TYPE)
			cout << in->to_string() << endl;
		else
		{
			EquiTuple* tup = (EquiTuple*) in;
			vector<EquiObject*> toPrint = tup->getTuple();
			if ((toPrint.size() > 1) && (toPrint[0]->getType() == E_STRING_TYPE))
			{
				EquiString* str = (EquiString*)toPrint[0];
				string s = str->getString();
				vector<size_t> insert;
				size_t found = -1;
				do
				{
					found = s.find("%i", found + 1);
					if (found != string::npos && !(found != 0 && s[found - 1] == '\\'))
					{
						insert.push_back(found);
					}
				}while (found != string::npos);

				if (insert.size() != 0)
				{
					string build = "";
					for (int i = 0; i < insert.size(); i++)
					{
						if (i == 0)
						{
							build += s.substr(0, insert[i]);
							if (i < toPrint.size() - 1)
								build += toPrint[i + 1]->to_string();
							else
								build += "()";
						}
						else
						{
							build += s.substr(insert[i - 1] + 2, (insert[i] - insert[i-1]) - 2);
							if (i < toPrint.size() - 1)
								build += toPrint[i + 1]->to_string();
							else
								build += "()";
						}
					}

					build += s.substr(insert[insert.size() - 1] + 2);
					cout << build << endl;
				}
				else
				{
					cout << in->to_string() << endl;
				}
			}
			else
			{
				cout << in->to_string() << endl;
			}
		}

		EquiVoid* n = new EquiVoid;
		return n;
	}
private:

};

template <class T>
class cnsl_std_EquiGetPrimitive : public EquiFunction
{
public:
	EQUI_FN(cnsl_std_EquiGetPrimitive<T>)

	virtual EquiObject* operator() (EquiObject* in)
	{
		if (in->getType() != E_VOID_TYPE)
			throwError("No input expected in getInt function.");
		T i = 0;
		cin >> i;

		EquiPrimitive<T>* n = new EquiPrimitive<T>;
		n->setData(i);
		return n;
	}
private:

};

class cnsl_std_EquiGetLine : public EquiFunction
{
public:
	EQUI_FN(cnsl_std_EquiGetLine)

	virtual EquiObject* operator() (EquiObject* in)
	{
		if (in->getType() != E_VOID_TYPE)
			throwError("No input expected in getline function.");
		string i = "";
		cin >> i;

		EquiString* n = new EquiString;
		n->setString(i);
		return n;
	}
private:

};

void loadConsoleStd(map<string, EquiObject*>* tok)
{
	cnsl_std_EquiPrint* print = new cnsl_std_EquiPrint;
	(*tok)["print"] = print;

	cnsl_std_EquiGetPrimitive<int>* getint = new cnsl_std_EquiGetPrimitive<int>;
	(*tok)["getint"] = getint;

	cnsl_std_EquiGetPrimitive<long>* getlong = new cnsl_std_EquiGetPrimitive<long>;
	(*tok)["getlong"] = getlong;

	cnsl_std_EquiGetPrimitive<double>* getdouble = new cnsl_std_EquiGetPrimitive<double>;
	(*tok)["getdouble"] = getdouble;

	cnsl_std_EquiGetPrimitive<float>* getfloat = new cnsl_std_EquiGetPrimitive<float>;
	(*tok)["getfloat"] = getfloat;

	cnsl_std_EquiGetLine* getline = new cnsl_std_EquiGetLine;
	(*tok)["getline"] = getline;
}
