#ifndef EQUI_CUSTOM_FUNCTION_H_
#define EQUI_CUSTOM_FUNCTION_H_

#include "AllObj.h"
#include <string>
#include "EquiWorker.h"

using namespace std;

class EQUI_custom_function : public EquiFunction
{
private:
	SyntaxTree* code;
	EquiFrame frame;
	string name;
	vector<pair<string,string>> inType;
public:
	string getDataType() { return E_FUNCTION_TYPE; };
	virtual EquiObject* clone()
	{
		SyntaxTree* tempC = new SyntaxTree("");
		*tempC = *code;

		map<string, EquiObject*>* t = frame.types.back();
		frame.types.pop_back();

		EquiObject* out = new EQUI_custom_function(inType, name, tempC, frame);

		frame.types.push_back(t);
		return out;
	};
	virtual EquiObject* spawnMyType() { return clone(); }; 

	EQUI_custom_function(vector<pair<string,string>> tt, string n, SyntaxTree* c, EquiFrame f)
	{
		inType = tt;
		name = n;
		code = c;
		frame = f;

		map<string, EquiObject*>* t = new map<string, EquiObject*>;
		(*t)[name] = this;
		frame.types.push_back(t);
	}

	EQUI_custom_function(vector<string> tok, SyntaxTree* c, EquiFrame f)
	{
		code = c;
		frame = f;
		name = tok[0];

		for (int i = 1; i < tok.size(); i++)
		{
			if (tok[i] == "->")
			{
				break;
			}

			vector<string> tp;
			string s = "";
			for (int j = 0; j < tok[i].size(); j++)
			{
				if (tok[i][j] == ' ')
				{
					tp.push_back(s);
					s = "";
				}
				else
				{
					s = s + tok[i][j];
				}
			}

			if (s != "")
			{
				tp.push_back(s);
			}

			if (tp.size() == 2)
			{
				pair<string, string> p(tp[0], tp[1]);
				inType.push_back(p);
			}
			else
			{
				string t = "^" + tp[0];
				pair<string, string> p(t, tp[2]);
				inType.push_back(p);
			}
		}

		map<string, EquiObject*>* t = new map<string, EquiObject*>;
		(*t)[name] = this;
		frame.types.push_back(t);
	}

	~EQUI_custom_function()
	{
		map<string, EquiObject*>* t = frame.types.back();
		frame.types.pop_back();
		delete t;
		delete code;
	}

	virtual EquiObject* operator() (EquiObject* in)
	{
		if (inType.size() > 1 && in->getType() != E_TUPLE_TYPE)
		{
			throwError("Missing inputs on function " + name);
		}

		if (inType.size() != 0)
		{
			if (inType.size() == 1)
			{
				if (inType[0].first[0] == '^')
				{
					if (in->getType() != E_ARRAY_TYPE)
					{
						throwError("Desired array input, input is " + in->getType());
					}
					string t = "";
					for (int i = 1; i < inType[0].first.size(); i++)
					{
						t = t + inType[0].first[i];
					}

					if (!frame.isType(t))
						throwError("Unrecognized desired type for input " + t);

					string dataT = frame.getType(t)->getDataType();

					if (in->getDataType()  != dataT)
					{
						throwError("Desired input type " + t + " got: " + in->getDataType());
					}
				}
				else
				{
					if (!frame.isType(inType[0].first))
						throwError("Unrecognized desired type for input " + inType[0].first);

					string dataT = frame.getType(inType[0].first)->getDataType();


					if (in->getDataType() != dataT)
					{
						throwError("Desired input type " + dataT + " got: " + in->getDataType());
					}
				}

				map<string,EquiObject*>* tok = new map<string,EquiObject*>;
				if (frame.isToken(inType[0].second))
				{
					*frame.getToken(inType[0].second) = *in;
					frame.getToken(inType[0].second)->setTemp(false);
				}
				else
				{
					EquiObject* newO = in->spawnMyType();
					*newO = *in;
					frame.emplaceToken(inType[0].second, newO);
					frame.getToken(inType[0].second)->setTemp(false);
				}
				
			}
			else
			{
				if (in->getType() != E_TUPLE_TYPE)
				{
					throwError("Input should be a tuple");
				}

				EquiTuple* tup = (EquiTuple*) in;
				vector<EquiObject*> tp = tup->getTuple();
				if (tp.size() != inType.size())
				{
					throwError("Input size does not match desired");
				}

				for (int i = 0; i < tp.size(); i++)
				{
					if (inType[i].first[0] == '^')
					{
						if (tp[i]->getType() != E_ARRAY_TYPE)
						{
							throwError("Desired array input, input is " + in->getType());
						}
						string t = "";
						for (int j = 1; j < inType[i].first.size(); j++)
						{
							t = t + inType[i].first[j];
						}

						if (!frame.isType(t))
							throwError("Unrecognized desired type for input " + t);

						string dataT = frame.getType(t)->getDataType();

						if (tp[i]->getDataType()  != dataT)
						{
							throwError("Desired input type " + t + " got: " + tp[i]->getDataType());
						}
					}
					else
					{

						if (!frame.isType(inType[i].first))
							throwError("Unrecognized desired type for input " + inType[i].first);

						string dataT = frame.getType(inType[i].first)->getDataType();

						if (tp[i]->getDataType() != dataT)
						{
							throwError("Desired input type " + dataT + " got: " + tp[i]->getDataType());
						}
					}

					if (frame.isToken(inType[i].second))
					{
						*frame.getToken(inType[i].second) = *tp[i];
						frame.getToken(inType[i].second)->setTemp(false);
					}
					else
					{
						EquiObject* newO = tp[i]->spawnMyType();
						*newO = *tp[i];
						frame.emplaceToken(inType[i].second, newO);
						frame.getToken(inType[i].second)->setTemp(false);
					}
				}
			}
		}

		EquiWorker work;
		work.loanType(&frame);

		pair<EquiObject*, bool> o = work.run(code);
		EquiObject* t = o.first->clone();

		if (o.second || work.killAnyways())
			delete o.first;

		return t;
	}
};

#endif
