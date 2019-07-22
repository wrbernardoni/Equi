#include "EquiWorker.h"
#include <iostream>
#include <cmath>

#include "EquiCustomFunction.h"

extern void throwError(string s)
{
	throw(s);
}

extern bool isNum(string s);

extern bool isString(string s);

bool tokQ(string s)
{
  return (s != "," && s != ";" && s != "!=" &&
    s != "==" && s != ">" && s != "<" && s != ">=" &&
    s != "<=" && s != "+" && s != "-" && s != "/" &&
    s != "*" && s != "%" && s != "!" && s != "=" &&
    s != "(" && s != ")" && s != "false" && s != "true" &&
    s != "{" && s != "}" && s != "[" && s != "]" &&
    s != "" && !isNum(s) && !isString(s));
}

bool tokenFormat(string si){
	for (int i = 0; i < si.size(); i++)
	{
		string s = si.substr(i, 1);
		if (!tokQ(s))
		{
			return false;
		}
	}
  return true;
}

EquiWorker::EquiWorker()
{
	runElse = false;
	breakFlag = false;
	continueFlag = false;
	returnFlag = false;
	elseFlag = false;
	killReturn = false;

	//EquiFrame def;
	//setFrame(def);

	ownedFrame = true;
	data = new EquiFrame;
}

EquiWorker::EquiWorker(EquiFrame* f)
{
	runElse = false;
	breakFlag = false;
	continueFlag = false;
	returnFlag = false;
	elseFlag = false;
	killReturn = false;

	//EquiFrame def;
	//setFrame(def);

	ownedFrame = false;
	data = f;
}

EquiWorker::~EquiWorker()
{
	if (ownedFrame)
		delete data;
}

EquiFrame EquiWorker::getFrame()
{
	return *data;
}

void EquiWorker::setFrame(const EquiFrame& o)
{
	if (ownedFrame)
		(*data) = o;
	else
	{
		data = new EquiFrame;
		(*data) = o;
	}
}

void EquiWorker::loanFrame(EquiFrame* f)
{
	if (ownedFrame)
		delete data;
	ownedFrame = false;
	data = f;
}

void EquiWorker::loanType(EquiFrame* f)
{
	if (ownedFrame)
		delete data;
	ownedFrame = true;
	data = new EquiFrame(f, true, false);
}

EquiObject* EquiWorker::getToken(string n)
{
	for (int i = data->tokens.size() - 1; i >= 0; i--)
	{
		if (data->tokens[i]->count(n) != 0)
		{
			return (*(data->tokens[i]))[n];
		}
	}
	throwError("Token " + n + " not found");
	return NULL;
}

EquiObject* EquiWorker::getType(string n)
{
	for (int i = data->types.size() - 1; i >= 0; i--)
	{
		if (data->types[i]->count(n) != 0)
		{
			return (*(data->types[i]))[n];
		}
	}
	throwError("Type " + n + " not found");
	return NULL;
}

bool EquiWorker::isToken(string n)
{
	for (int i = 0; i < data->tokens.size(); i++)
	{
		if (data->tokens[i]->count(n) != 0)
		{
			return true;
		}
	}
	return false;
}

void EquiWorker::emplaceToken(string n, EquiObject* o)
{
	if (data->tokens[data->tokens.size() - 1]->count(n) != 0)
		throwError("Token " + n + " already defined");

	o->setTemp(false);
	(*(data->tokens[data->tokens.size() - 1]))[n] = o;
}

bool EquiWorker::isType(string n)
{
	for (int i = 0; i < data->types.size(); i++)
	{
		if (data->types[i]->count(n) != 0)
		{
			return true;
		}
	}
	return false;
}

void EquiWorker::emplaceType(string n, EquiObject* o)
{
	if (data->types[data->types.size() - 1]->count(n) != 0)
		throwError("Token " + n + " already defined");

	o->setTemp(false);
	(*(data->types[data->types.size() - 1]))[n] = o;
}

void EquiWorker::scopeUp()
{
	map<string, EquiObject*>* tok = new map<string, EquiObject*>;
	data->tokens.push_back(tok);

	map<string, EquiObject*>* typ = new map<string, EquiObject*>;
	data->types.push_back(typ);
}

void EquiWorker::scopeDown()
{
	map<string, EquiObject*>* tok = data->tokens[data->tokens.size() - 1];
	for (auto const& y : *tok)
	{
		delete y.second;
	}

	delete tok;
	data->tokens.pop_back();

	map<string, EquiObject*>* typ = data->types[data->types.size() - 1];
	for (auto const& y : *typ)
	{
		delete y.second;
	}

	delete typ;
	data->types.pop_back();
}

void EquiWorker::resetScope()
{
	breakFlag = false;
	continueFlag = false;
	returnFlag = false;
	while (data->tokens.size() > 1)
	{
		scopeDown();
	}
}

/*
pair<EquiObject*, bool> EquiWorker::run(SyntaxTree* code)
{
	if (breakFlag || continueFlag)
	{
		pair<EquiObject*, bool> retPair(new EquiVoid, false);
		return retPair;
	}

	if (returnFlag)
	{
		pair<EquiObject*, bool> retPair(returnItem, false);
		return retPair;
	}

	vector<SyntaxTree*> children = code->getChildren();
	vector<EquiObject*> childOut;
	vector<bool> killKid;
	if (code->getType() != EQ_TR_LOGICAL_BLOCK && code->getType() != EQ_TR_DO_WHILE
		&& code->getType() != EQ_TR_WHILE && code->getType() != EQ_TR_FOR
		&& code->getType() != EQ_TR_FUNCTION_DEC)
	{
		for (int i = 0; i < children.size(); i++)
		{
			pair<EquiObject*, bool> cO = run(children[i]);
			childOut.push_back(cO.first);
			killKid.push_back(cO.second);
		}
	}

	bool killChildren = true;

	EquiObject* out = NULL;
	bool killOut = true;

	if (returnFlag)
	{

	}
	else if (code->getType() == EQ_TR_ROOT)
	{
		
	}
	else if (code->getType() == EQ_TR_CODE)
	{
		if (childOut.size() > 1 || childOut.size() == 0)
		{
			EquiVoid* v = new EquiVoid;
			out = v;
		}
		else
		{
			killOut = killKid[0];
			out = childOut[0];
			killChildren = false;
		}
		
	}
	else if (code->getType() == EQ_TR_FUNCTION_DEC)
	{
		string fn = code->getTokens()[0];
		EquiFrame f;
		f.setTypes(data->types);
		SyntaxTree* cd = new SyntaxTree("");
		(*cd) = *(code->getChildren()[0]);
		EQUI_custom_function* newF = new EQUI_custom_function(code->getTokens(), cd,f);
		emplaceType(fn, newF);
	}
	else if (code->getType() == EQ_TR_COMMA)
	{
		EquiTuple* t = new EquiTuple;
		vector<EquiObject*> o;
		for (int i = 0; i < childOut.size(); i++)
		{
			o.push_back(childOut[i]);
		}

		t->setTuple(o);
		killChildren = false;
		out = t;
	}
	else if (code->getType() == EQ_TR_AS)
	{
		out = childOut[0];
		killOut = killKid[0];
		killKid[0] = false;
	}
	else if (code->getType() == EQ_TR_EQUALITY)
	{
		if (childOut.size() != 2 || code->getTokens().size() != 1)
			throwError("Invalid number of arguments on binary operation");

		EquiPrimitive<bool>* res = new EquiPrimitive<bool>;
		if (code->getTokens()[0] == "!=")
			res->setData(*childOut[0] != *childOut[1]);
		else
			res->setData(*childOut[0] == *childOut[1]);
		out = res;
	}
	else if (code->getType() == EQ_TR_COMPARISON)
	{
		if (childOut.size() != 2 || code->getTokens().size() != 1)
			throwError("Invalid number of arguments on binary operation");

		EquiPrimitive<bool>* res = new EquiPrimitive<bool>;
		string tok = code->getTokens()[0];
		if (tok == ">")
			res->setData(*childOut[0] > *childOut[1]);
		else if (tok == ">=")
			res->setData(*childOut[0] >= *childOut[1]);
		else if (tok == "<")
			res->setData(*childOut[0] < *childOut[1]);
		else
			res->setData(*childOut[0] <= *childOut[1]);
		out = res;
	}
	else if (code->getType() == EQ_TR_ADDITIVE)
	{
		if (childOut.size() != 2 || code->getTokens().size() != 1)
			throwError("Invalid number of arguments on binary operation");

		string tok = code->getTokens()[0];
		if (tok == "+")
		{
			EquiObject* r = (*childOut[0] + *childOut[1]);
			out = r;
		}
		else
		{
			EquiObject* r = (*childOut[0] - *childOut[1]);
			out = r;
		}
		
	}
	else if (code->getType() == EQ_TR_MULTIPLICATIVE)
	{
		if (childOut.size() != 2 || code->getTokens().size() != 1)
			throwError("Invalid number of arguments on binary operation");

		string tok = code->getTokens()[0];
		if (tok == "*")
		{
			EquiObject* r = (*childOut[0] * *childOut[1]);
			out = r;
		}
		else if (tok == "/")
		{
			EquiObject* r = (*childOut[0] / *childOut[1]);
			out = r;
		}
		else
		{
			EquiObject* r = (*childOut[0] % *childOut[1]);
			out = r;
		}
	}
	else if (code->getType() == EQ_TR_UNARY)
	{
		if (childOut.size() != 1 || code->getTokens().size() != 1)
			throwError("Invalid number of arguments on unary operation");

		string tok = code->getTokens()[0];
		if (tok == "!")
		{
			EquiObject* r = !(*childOut[0]);
			out = r;
		}
		else
		{
			EquiObject* r = -(*childOut[0]);
			out = r;
		}
	}
	else if (code->getType() == EQ_TR_ARRAY)
	{
		if (childOut.size() != 2)
			throwError("Invalid number of operators on array index reference");

		string ts = childOut[1]->to_string();
		EquiObject* arr = childOut[0];
		arr->setTemp(childOut[0]->getTemp());

		killOut = false;

		if (!isNum(ts))
		{

			out = (*arr)[ts];
		}
		else
		{
			out = (*arr)[stoi(ts)];
		}
		out->setTemp(arr->getTemp());
		if (killKid[0])
		{
			killKid[0] = false;
			out->addDependant(arr);
		}
	}
	else if (code->getType() == EQ_TR_MEMACCESS)
	{
		if (childOut.size() != 1 || code->getTokens().size() != 1)
			throwError("Invalid number of operators on memory dereference");

		EquiObject* arr = childOut[0];
		out = (*arr)[code->getTokens()[0]];
		killOut = false;
		out->setTemp(arr->getTemp());
		if (killKid[0])
		{
			out->addDependant(childOut[0]);
			killKid[0] = false;
		}
	}
	else if (code->getType() == EQ_TR_ASSIGNMENT)
	{
		if (childOut.size() != 2)
			throwError("Invalid number of arguments on assignment");

		if (! childOut[0]->getTemp())
		{
			((*childOut[0]) = (*childOut[1]));
		}
		else if (childOut[0]->getType() == E_TUPLE_TYPE)
		{
			EquiTuple* tupA = (EquiTuple*)childOut[0];
			vector<EquiObject*> oA = tupA->getTuple();
			if (childOut[1]->getType() == E_TUPLE_TYPE)
			{
				EquiTuple* tupB = (EquiTuple*)childOut[1];
				vector<EquiObject*> oB = tupB->getTuple();
				if (oA.size() != oB.size())
					throwError("Tuple assignment must be between tuples of the same size");
				for (int i = 0; i < oA.size(); i++)
				{
					if (oA[i]->getTemp())
						throwError("Cannot assign to a temporary variable");
					((*oA[i]) = (*oB[i]));
				}
			}
			else
			{
				for (int i = 0; i < oA.size(); i++)
				{
					if (oA[i]->getTemp())
						throwError("Cannot assign to a temporary variable");
					((*oA[i]) = (*childOut[1]));
				}
			}
		}
		else
		{
			throwError("Cannot assign to a temporary variable.");
		}
		out = childOut[0];
		killOut = killKid[0];
		killKid[0] = false;

	}
	else if (code->getType() == EQ_TR_DECLARATION)
	{
		if (childOut.size() > 1 || (code->getTokens().size() > 3))
			throwError("Invalid number of arguments on unary operation");

		string tok = code->getTokens()[0];
		EquiObject* obj = NULL;
		if (code->getTokens().size() == 2 && (tokenFormat(code->getTokens()[1]) || code->getTokens()[1] == "++" || code->getTokens()[1] == "--"))
		{
			string type = code->getTokens()[0];
			tok = code->getTokens()[1];
			EquiObject* newObj = NULL;

			if (tok == "++" || tok == "--" || type == "++" || type == "--")
			{
				string op = "";
				bool pre = false;
				if (tok == "++" || tok == "--")
				{
					op = tok;
					tok = type;
					pre = false;
				}
				else
				{
					op = type;
					pre = true;
				}
				
				if (!isToken(tok))
				{
					throwError("Undefined token " + tok);
				}

				if (childOut.size() != 0)
				{
					throwError("Cannot assign to an increment");
				}

				if (op == "++")
					++(*getToken(tok));
				else
					--(*getToken(tok));
			}
			else
			{
				if (childOut.size() != 0)
				{
					if (!isToken(tok))
					{
						newObj = getType(type)->spawnMyType();
						(*newObj) = (*childOut[0]);
						emplaceToken(tok, newObj);
					}
				}
				else
				{
					newObj = getType(type)->spawnMyType();
					emplaceToken(tok, newObj);
				}
			}
		}
		else if (code->getTokens().size() == 2)
		{
			if (!isToken(tok))
			{
				throwError("Undefined token " + tok);
			}

			int index = 0;
			if (isNum(code->getTokens()[1]))
				index = stoi(code->getTokens()[1]);
			else
			{
				string ind = code->getTokens()[1].substr(1, code->getTokens()[1].size() - 2);
				if (!isToken(ind))
				{
					throwError("Undefined token " + ind);
				}

				EquiObject* o = getToken(ind);
				string oS = o->to_string();
				if (!isNum(oS))
				{
					throwError("Must index by a numeric");
				}

				index = stoi(oS);
			}

			obj = (*getToken(tok))[index];
		}
		else if (code->getTokens().size() == 3)
		{
			string type = code->getTokens()[0];

			int index = 0;

			if (isNum(code->getTokens()[1]))
			{
				index = stoi(code->getTokens()[1]);
			}
			else
			{
				string ind = code->getTokens()[1].substr(1, code->getTokens()[1].size() - 2);
				if (!isToken(ind))
				{
					throwError("Undefined token " + ind);
				}

				EquiObject* o = getToken(ind);
				string oS = o->to_string();
				if (!isNum(oS))
				{
					throwError("Must index by a numeric");
				}

				index = stoi(oS);
			}


			tok = code->getTokens()[2];

			if (tok == "++" || tok == "--")
			{
				//a[32]++
				if (!isToken(type))
				{
					throwError("Undefined token " + type);
				}

				if (childOut.size() != 0)
				{
					throwError("Cannot assign to an increment");
				}

				obj = (*getToken(type))[index];

				if (tok == "++")
					++(*obj);
				else
					--(*obj);
			}
			else
			{
				if (childOut.size() != 0)
				{
					if (!isToken(tok))
					{
						//int[32] a;
						EquiObject* newObj = NULL;
						if (type == "int")
						{
							newObj = new EquiArray<EquiPrimitive<int>>;
						}
						else if (type == "long")
						{
							newObj = new EquiArray<EquiPrimitive<long>>;
						}
						else if (type == "double")
						{
							newObj = new EquiArray<EquiPrimitive<long>>;
						}
						else if (type == "float")
						{
							newObj = new EquiArray<EquiPrimitive<float>>;
						}
						else if (type == "bool")
						{
							newObj = new EquiArray<EquiPrimitive<bool>>;
						}
						else if (type == "string")
						{
							newObj = new EquiArray<EquiString>;
						}
						else if (type == "()")
						{
							newObj = new EquiArray<EquiTuple>;
						}
						else
						{
							throwError("Unrecognized type name");
						}

						(*newObj) = (*childOut[0]);
						emplaceToken(tok, newObj);

						obj = newObj;
					}
				}
				else
				{
					//int[32] a;
					EquiObject* newObj = NULL;
					if (type == "int")
					{
						newObj = new EquiArray<EquiPrimitive<int>>;
						EquiPrimitive<int>* tO;
						for (int i = 0; i < index; i++)
						{
							tO = new EquiPrimitive<int>;
							tO->setData(0);
							((EquiArray<EquiPrimitive<int>>*)newObj)->append(tO);
						}
					}
					else if (type == "long")
					{
						newObj = new EquiArray<EquiPrimitive<long>>;
						EquiPrimitive<long>* tO;
						for (int i = 0; i < index; i++)
						{
							tO = new EquiPrimitive<long>;
							tO->setData(0);
							((EquiArray<EquiPrimitive<long>>*)newObj)->append(tO);
						}
					}
					else if (type == "double")
					{
						newObj = new EquiArray<EquiPrimitive<long>>;
						EquiPrimitive<long>* tO;
						for (int i = 0; i < index; i++)
						{
							tO = new EquiPrimitive<long>;
							tO->setData(0);
							((EquiArray<EquiPrimitive<long>>*)newObj)->append(tO);
						}

						newObj = new EquiPrimitive<double>;
						((EquiPrimitive<double>*)newObj)->setData(0);
					}
					else if (type == "float")
					{
						newObj = new EquiArray<EquiPrimitive<float>>;
						EquiPrimitive<float>* tO;
						for (int i = 0; i < index; i++)
						{
							tO = new EquiPrimitive<float>;
							tO->setData(0);
							((EquiArray<EquiPrimitive<float>>*)newObj)->append(tO);
						}
					}
					else if (type == "bool")
					{
						newObj = new EquiArray<EquiPrimitive<bool>>;
						EquiPrimitive<bool>* tO;
						for (int i = 0; i < index; i++)
						{
							tO = new EquiPrimitive<bool>;
							tO->setData(0);
							((EquiArray<EquiPrimitive<bool>>*)newObj)->append(tO);
						}
					}
					else if (type == "string")
					{
						newObj = new EquiArray<EquiString>;
						EquiString* tO;
						for (int i = 0; i < index; i++)
						{
							tO = new EquiString;
							tO->setString("");
							((EquiArray<EquiString>*)newObj)->append(tO);
						}
					}
					else if (type == "()")
					{
						newObj = new EquiArray<EquiTuple>;
						EquiTuple* tO;
						for (int i = 0; i < index; i++)
						{
							tO = new EquiTuple;
							((EquiArray<EquiTuple>*)newObj)->append(tO);
						}
					}
					else
					{
						throwError("Unrecognized type name");
					}

					emplaceToken(tok, newObj);

					obj = newObj;
				}
			}
		}
		
		if (obj == NULL)
		{
			if (!isToken(tok))
			{
				throwError("Undefined token " + tok);
			}

			obj = getToken(tok);
		}

		killOut = false;
		out = (obj);
	}
	else if (code->getType() == EQ_TR_CONST)
	{
		if (childOut.size() != 0 || code->getTokens().size() != 1 )
			throwError("Invalid number of arguments on constant???");

		if (code->getTokens()[0] == "false")
		{
			EquiPrimitive<bool>* fals = new EquiPrimitive<bool>;
			fals->setData(false);
			out = fals;
		}
		else if (code->getTokens()[0] == "true")
		{
			EquiPrimitive<bool>* tru = new EquiPrimitive<bool>;
			tru->setData(true);
			out = tru;
		}
		else if (code->getTokens()[0] == "()")
		{
			EquiTuple* tup = new EquiTuple;
			out = tup;
		}
		else if (isString(code->getTokens()[0]))
		{
			string s = code->getTokens()[0].substr(1, code->getTokens()[0].size() - 2);
			EquiString* str = new EquiString;
			str->setString(s);
			out = str;
		}
		else if (isNum(code->getTokens()[0]))
		{
			double dat = stod(code->getTokens()[0]);
			EquiPrimitive<double>* d = new EquiPrimitive<double>;
			d->setData(dat);
			out = d;
		}
		else
		{
			throwError("Unrecognized constant value");
		}
	}
	else if (code->getType() == EQ_TR_TOKEN)
	{
		if (childOut.size() != 0 || code->getTokens().size() != 1 )
			throwError("Invalid number of arguments on token???");

		string tok = code->getTokens()[0];
		if (!isToken(tok) && !isType(tok))
		{
			throwError("Undefined reference to token " + tok);
		}

		killOut = false;
		if (isToken(tok))
			out = getToken(tok);
		else
			out = getType(tok);
	}
	else if (code->getType() == EQ_TR_FUNCTION)
	{
		if (childOut.size() == 0)
			throwError("Invalid number of arguments for functions");

		if (childOut.size()  == 1)
		{
			EquiVoid* v = new EquiVoid;
			childOut.push_back(v);
		}

		int input = 0;
		vector<pair<string, EquiObject*>> setFrame;
		for (int i = 1; i < children.size(); i++)
		{
			if (children[i]->getType() == EQ_TR_AS)
			{
				pair<string, EquiObject*> t (children[i]->getTokens()[0], childOut[i]->clone());
				setFrame.push_back(t);
			}
			else
			{
				if (input == 0)
					input = i;
				else
				{
					throwError("Cannot have two inputs to a function");
				}
			}
		}
		if (input == 0)
			input = 1;

		out = (*childOut[0])(childOut[input], setFrame);
	}
	else if (code->getType() == EQ_TR_SPECIAL)
	{
		if (code->getTokens().size() != 1 )
			throwError("Invalid number of arguments for keyword");
		string tok = code->getTokens()[0];

		if (tok == "break")
		{
			breakFlag = true;
		}
		else if (tok == "continue")
		{
			continueFlag = true;
		}
		else if (tok == "return")
		{
			returnFlag = true;
			if (childOut.size() == 0)
			{
				returnItem = new EquiVoid;
				killReturn = true;
			}
			else
			{
				returnItem = childOut[0]->clone();
				killReturn = true;
				//killKid[0] = false;
			}
		}
	}
	else if (code->getType() == EQ_TR_DO_WHILE)
	{
		if (children.size() != 2)
		{
			throwError("Incorrect number of arguments on do while");
		}

		bool keepRun = true;
		do
		{
			if (returnFlag)
			{
				keepRun = false;
				break;
			}

			scopeUp();
			pair<EquiObject*, bool> rn = run(children[0]);
			if (rn.second)
				delete rn.first;
			scopeDown();
			if(breakFlag)
			{
				breakFlag = false;
				keepRun = false;
			}
			else if (returnFlag)
			{
				keepRun = false;
			}
			else
			{
				if (continueFlag)
					continueFlag = false;

				EquiPrimitive<bool> fals;
				fals.setData(false);
				pair<EquiObject*, bool> eval = run(children[1]);
				keepRun = (fals) != (*(eval.first));
				if (eval.second)
					delete eval.first;
			}
		}while(keepRun);
	}
	else if (code->getType() == EQ_TR_WHILE)
	{
		if (children.size() != 2)
		{
			throwError("Incorrect number of arguments on while");
		}

		bool keepRun = true;
		scopeUp();
		EquiPrimitive<bool> fals;
		fals.setData(false);
		pair<EquiObject*, bool> eval = run(children[0]);
		keepRun =  (fals) != (*(eval.first));
		if (eval.second)
			delete eval.first;
		scopeDown();
		
		if(breakFlag)
		{
			breakFlag = false;
			keepRun = false;
		}
		else if (returnFlag)
		{
			keepRun = false;
		}

		if (continueFlag)
			continueFlag = false;

		while(keepRun)
		{
			scopeUp();
			pair<EquiObject*, bool> rn = run(children[1]);
			if (rn.second)
				delete rn.first;
			scopeDown();
			if(breakFlag)
			{
				breakFlag = false;
				keepRun = false;
			}
			else if (returnFlag)
			{
				keepRun = false;
			}
			else
			{
				if (continueFlag)
					continueFlag = false;

				EquiPrimitive<bool> fals;
				fals.setData(false);
				pair<EquiObject*, bool> eval = run(children[0]);
				keepRun = (fals) != (*(eval.first));
				if (eval.second)
					delete eval.first;
			}
		}
	}
	else if (code->getType() == EQ_TR_FOR)
	{
		if (children.size() != 4)
		{
			throwError("Incorrect number of arguments on for loop");
		}

		EquiPrimitive<bool> fals;
		fals.setData(false);

		scopeUp();
		pair<EquiObject*, bool> dec = run(children[0]);
		if (dec.second)
			delete dec.first;
		
		pair<EquiObject*, bool> eval = run(children[1]);
		bool keepRun = (fals) != (*(eval.first));
		if (eval.second)
			delete eval.first;
				
		if(breakFlag)
		{
			breakFlag = false;
			keepRun = false;
		}
		if (continueFlag)
			continueFlag = false;

		if (returnFlag)
		{
			keepRun = false;
		}

		while(keepRun)
		{
			scopeUp();
			pair<EquiObject*, bool> rn = run(children[3]);
			if (rn.second)
				delete rn.first;
			scopeDown();
			if(breakFlag)
			{
				breakFlag = false;
				keepRun = false;
			}
			else if (returnFlag)
			{
				keepRun = false;
			}
			else
			{
				if (continueFlag)
					continueFlag = false;

				pair<EquiObject*, bool> endDec = run(children[2]);
				if (endDec.second)
					delete endDec.first;
				if (continueFlag)
					continueFlag = false;

				if(breakFlag)
				{
					breakFlag = false;
					keepRun = false;
				}
				else
				{
					eval = run(children[1]);
					keepRun = (fals) != (*(eval.first));
					if (eval.second)
						delete eval.first;
				}
			}
		}

		scopeDown();
	}
	else if (code->getType() == EQ_TR_LOGICAL_BLOCK)
	{
		scopeUp();
		if (code->getTokens().size() == 1)
		{
			if (code->getTokens()[0] == "if")
			{
				if (children.size() != 2)
				{
					throwError("Incorrect number of arguments on else");
				}

				pair<EquiObject*, bool> cond = run(children[0]);
				childOut.push_back(cond.first);
				killKid.push_back(cond.second);
				EquiPrimitive<bool> fals;
				fals.setData(false);
				if (fals != *childOut[0])
				{
					EquiPrimitive<bool>* tru = new EquiPrimitive<bool>;
					tru->setData(true);
					out = tru;
					pair<EquiObject*, bool> cod = run(children[1]);
					childOut.push_back(cod.first);
					killKid.push_back(cod.second);
					runElse = false;
				}
				else
				{
					EquiPrimitive<bool>* tru = new EquiPrimitive<bool>;
					tru->setData(false);
					out = tru;
					runElse = true;
				}

			}
			else if (code->getTokens()[0] == "else")
			{
				if (children.size() != 1)
				{
					throwError("Incorrect number of arguments on else");
				}

				if (runElse)
				{
					EquiPrimitive<bool>* tru = new EquiPrimitive<bool>;
					tru->setData(true);
					out = tru;
					pair<EquiObject*, bool> cod = run(children[0]);
					childOut.push_back(cod.first);
					killKid.push_back(cod.second);
				}
				else
				{
					EquiPrimitive<bool>* tru = new EquiPrimitive<bool>;
					tru->setData(false);
					out = tru;
				}
				runElse = false;
			}
			else
			{
				throwError("Unknown logical block type.");
			}
		}
		else if (code->getTokens().size() == 2)
		{
			if (children.size() != 2)
			{
				throwError("Incorrect number of arguments on else if");
			}

			if (runElse)
			{
				pair<EquiObject*, bool> rn = run(children[0]);
				childOut.push_back(rn.first);
				killKid.push_back(rn.second);
				EquiPrimitive<bool> fals;
				fals.setData(false);
				if (fals != *childOut[0])
				{
					EquiPrimitive<bool>* tru = new EquiPrimitive<bool>;
					tru->setData(true);
					out = tru;
					pair<EquiObject*, bool> cod = run(children[1]);
					childOut.push_back(cod.first);
					killKid.push_back(cod.second);
					runElse = false;
				}
				else
				{
					EquiPrimitive<bool>* tru = new EquiPrimitive<bool>;
					tru->setData(false);
					out = tru;
					runElse = true;
				}
			}
			else
			{
				EquiPrimitive<bool>* tru = new EquiPrimitive<bool>;
				tru->setData(false);
				out = tru;
			}
		}
		else
		{
			throwError("Too many arguments in logical block");
		}

		scopeDown();
	}
	else
	{
		for (int i = 0; i < childOut.size(); i++)
		{
			delete childOut[i];
		}
		throwError("Unknown syntax tree node type (?????)");
	}

	if (killChildren)
	{
		for (int i = 0; i < childOut.size(); i++)
		{
			if (killKid[i])
				delete childOut[i];
		}
	}

	if (out == NULL)
		out = new EquiVoid;

	if (returnFlag)
	{
		if (killOut)
			delete out;

		pair<EquiObject*, bool> retPair(returnItem, false);
		return retPair;
	}

	pair<EquiObject*, bool> retPair(out, killOut);
	return retPair;
}
*/
