#include "EquiWorker.h"
#include <iostream>

extern void throwError(string s)
{
	throw(s);
}

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
		
	}
	else if (code->getType() == EQ_TR_MULTIPLICATIVE)
	{
		
	}
	else if (code->getType() == EQ_TR_UNARY)
	{
		
	}
	else if (code->getType() == EQ_TR_DECLARATION)
	{

	}
	else if (code->getType() == EQ_TR_CONST)
	{
		
	}
	else if (code->getType() == EQ_TR_TOKEN)
	{

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

	if (!killChildren)
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

