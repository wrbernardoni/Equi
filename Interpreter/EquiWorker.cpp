#include "EquiWorker.h"
#include <iostream>
#include <cmath>

extern void throwError(string s)
{
	throw(s);
}

extern bool isNum(string s);

extern bool isString(string s);

EquiObject* EquiWorker::run(SyntaxTree* code)
{
	vector<SyntaxTree*> children = code->getChildren();;
	vector<EquiObject*> childOut;
	for (int i = 0; i < children.size(); i++)
	{
		childOut.push_back(run(children[i]));
	}

	bool killChildren = true;

	EquiObject* out = NULL;

	if (code->getType() == EQ_TR_ROOT)
	{
		
	}
	else if (code->getType() == EQ_TR_CODE)
	{
		if (childOut.size() == 1)
			P_VERB("-->" << childOut[0]->to_string() << endl << endl, TOKEN_PRINT_VERB)
	}
	else if (code->getType() == EQ_TR_COMMA)
	{
		EquiTuple* t = new EquiTuple;
		t->setTuple(childOut);
		out = t;
		killChildren = false;
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
	else if (code->getType() == EQ_TR_DECLARATION)
	{
		if (childOut.size() != 1 || (code->getTokens().size() != 1 && code->getTokens().size() != 2))
			throwError("Invalid number of arguments on unary operation");

		string tok = code->getTokens()[0];
		if (code->getTokens().size() == 2)
		{
			string type = code->getTokens()[0];
			tok = code->getTokens()[1];
			EquiObject* newObj = NULL;

			if (type == "int")
			{
				newObj = new EquiPrimitive<int>;
			}
			else if (type == "long")
			{
				newObj = new EquiPrimitive<long>;
			}
			else if (type == "double")
			{
				newObj = new EquiPrimitive<double>;
			}
			else if (type == "float")
			{
				newObj = new EquiPrimitive<float>;
			}
			else if (type == "bool")
			{
				newObj = new EquiPrimitive<bool>;
			}
			else
			{
				throwError("Unrecognized type name");
			}

			if(tokens.count(tok) != 0)
			{
				throwError("Reinstantiation of token");
				delete tokens[tok];
			}

			tokens[tok] = newObj;
		}
		
		if (tokens.count(tok) == 0)
		{
			throwError("Undefined token");
		}
		out = (*tokens[tok] = *(childOut[0])).clone();
	}
	else if (code->getType() == EQ_TR_CONST)
	{
		if (childOut.size() != 0 || code->getTokens().size() != 1 )
			throwError("Invalid number of arguments on constant???");

		if (isString(code->getTokens()[0]))
		{
			// TODO
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
		if (tokens.count(tok) == 0)
		{
			throwError("Undefined reference to token " + tok);
		}

		out = tokens[tok]->clone();
	}
	else if (code->getType() == EQ_TR_FUNCTION)
	{
		
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
			delete childOut[i];
		}
	}

	if (out == NULL)
		out = new EquiVoid;
	return out;
}

