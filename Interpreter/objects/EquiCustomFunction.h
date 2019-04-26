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
	vector<string> init;
public:
	string getDataType() { return E_FUNCTION_TYPE; };
	virtual EquiObject* clone()
	{
		SyntaxTree* tempC = new SyntaxTree("");
		*tempC = *code;
		return new EQUI_custom_function(init, tempC, frame);
	};
	virtual EquiObject* spawnMyType() { return clone(); }; 

	EQUI_custom_function(vector<string> tok, SyntaxTree* c, EquiFrame f)
	{
		init = tok;
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

			if (tp[0] == "()")
			{
				tp[0] = "tuple";
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
	}

	~EQUI_custom_function()
	{
		delete code;
	}

	virtual EquiObject* operator() (EquiObject* in)
	{
		EquiFrame f = frame;

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

					if (in->getDataType()  != t)
					{
						throwError("Desired input type " + t + " got: " + in->getDataType());
					}
				}
				else
				{
					if (in->getDataType() != inType[0].first)
					{
						throwError("Desired input type " + inType[0].first + " got: " + in->getDataType());
					}
				}

				map<string,EquiObject*>* tok = new map<string,EquiObject*>;
				(*tok)[inType[0].second] = in->spawnMyType();
				*(*tok)[inType[0].second] = *in;
				(*tok)[inType[0].second]->setTemp(false);
				f.tokens.push_back(tok);
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

				map<string,EquiObject*>* toks = new map<string,EquiObject*>;

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

						if (tp[i]->getDataType()  != t)
						{
							throwError("Desired input type " + t + " got: " + tp[i]->getDataType());
						}
					}
					else
					{
						if (tp[i]->getDataType() != inType[i].first)
						{
							throwError("Desired input type " + inType[i].first + " got: " + tp[i]->getDataType());
						}
					}

					(*toks)[inType[i].second] = tp[i]->spawnMyType();
					*(*toks)[inType[i].second] = *tp[i];
					(*toks)[inType[i].second]->setTemp(false);
				}

				f.tokens.push_back(toks);
			}
		}

		EquiWorker work;
		work.setFrame(f);

		pair<EquiObject*, bool> o = work.run(code);

		if (o.first->getTemp())
			return o.first;
		else
			return o.first->clone();
	}
};

#endif
