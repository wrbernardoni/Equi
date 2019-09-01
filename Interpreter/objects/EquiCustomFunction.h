#ifndef EQUI_CUSTOM_FUNCTION_H_
#define EQUI_CUSTOM_FUNCTION_H_

#include "AllObj.h"
#include <string>
#include "EquiWorker.h"
#include "parse.h"

using namespace std;

class EQUI_custom_function : public EquiFunction
{
private:
	vector<CodeLine> code;
	EquiFrame frame;
	string name;
	vector<pair<string,string>> inType;
public:
	string getDataType() { return E_FUNCTION_TYPE; };
	virtual EquiObject* clone()
	{
		map<string, EquiObject*>* t = frame.types.back();
		frame.types.pop_back();

		EquiObject* out = new EQUI_custom_function(inType, name, code, frame);

		frame.types.push_back(t);
		return out;
	};
	virtual EquiObject* spawnMyType() { return clone(); }; 

	EQUI_custom_function(vector<pair<string,string>> tt, string n, vector<CodeLine> c, EquiFrame f)
	{
		inType = tt;
		name = n;
		code = c;
		frame = f;

		map<string, EquiObject*>* t = new map<string, EquiObject*>;
		(*t)[name] = this;
		frame.types.push_back(t);
	}

	EQUI_custom_function(vector<string> tok, vector<CodeLine> c, EquiFrame f)
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
	}

	virtual EquiObject* operator() (EquiObject* in)
	{
		vector<pair<string, EquiObject*>> n;
		return operator()(in, n);
	}

	virtual EquiObject* operator() (EquiObject* in, vector<pair<string, EquiObject*>> setFrame)
	{
		EquiFrame workingFrame(&frame, false, false);

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

					if (!workingFrame.isType(t))
						throwError("Unrecognized desired type for input " + t);

					string dataT = workingFrame.getType(t)->getDataType();

					if (in->getDataType()  != dataT)
					{
						throwError("Desired input type " + t + " got: " + in->getDataType());
					}
				}
				else
				{
					if (!workingFrame.isType(inType[0].first))
						throwError("Unrecognized desired type for input " + inType[0].first);

					string dataT = workingFrame.getType(inType[0].first)->getDataType();


					if (in->getDataType() != dataT)
					{
						throwError("Desired input type " + dataT + " got: " + in->getDataType());
					}
				}

				//map<string,EquiObject*>* tok = new map<string,EquiObject*>;
				if (workingFrame.isToken(inType[0].second))
				{
					*workingFrame.getToken(inType[0].second) = *in;
					workingFrame.getToken(inType[0].second)->setTemp(false);
				}
				else
				{
					EquiObject* newO = in->spawnMyType();
					*newO = *in;
					workingFrame.emplaceToken(inType[0].second, newO);
					workingFrame.getToken(inType[0].second)->setTemp(false);
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

						if (!workingFrame.isType(t))
							throwError("Unrecognized desired type for input " + t);

						string dataT = workingFrame.getType(t)->getDataType();

						if (tp[i]->getDataType()  != dataT)
						{
							throwError("Desired input type " + t + " got: " + tp[i]->getDataType());
						}
					}
					else
					{

						if (!workingFrame.isType(inType[i].first))
							throwError("Unrecognized desired type for input " + inType[i].first);

						string dataT = workingFrame.getType(inType[i].first)->getDataType();

						if (tp[i]->getDataType() != dataT)
						{
							throwError("Desired input type " + dataT + " got: " + tp[i]->getDataType());
						}
					}

					if (workingFrame.isToken(inType[i].second))
					{
						*workingFrame.getToken(inType[i].second) = *tp[i];
						workingFrame.getToken(inType[i].second)->setTemp(false);
					}
					else
					{
						EquiObject* newO = tp[i]->spawnMyType();
						*newO = *tp[i];
						workingFrame.emplaceToken(inType[i].second, newO);
						workingFrame.getToken(inType[i].second)->setTemp(false);
					}
				}
			}
		}

		EquiWorker work;
		work.setFrame(workingFrame);

		EquiFrame* f = work.touchFrame();
		for (int i = 0; i < setFrame.size(); i++)
		{
			f->emplaceToken(setFrame[i].first, setFrame[i].second);
		}

		work.scopeUp();
		vector<CodeLine>* cpCode = new vector<CodeLine>;
		*cpCode = code;
		pair<EquiObject*, bool> o = work.run(cpCode);
		delete cpCode;
		EquiObject* t = o.first->clone();
		work.scopeDown();

		if (o.second || work.killAnyways())
			delete o.first;

		return t;
	}
};

#endif
