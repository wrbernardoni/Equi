#include "EquiWorker.h"

void throwError(string s)
{
	throw(s);
}

EquiObject EquiWorker::run(SyntaxTree* code)
{
	vector<SyntaxTree*> children = code->getChildren();;
	vector<EquiObject> childOut;
	for (int i = 0; i < children.size(); i++)
	{
		childOut.push_back(run(children[i]));
	}

	if (code->getType() == EQ_TR_ROOT)
	{
		EquiVoid v;
		return v;
	}
	else if (code->getType() == EQ_TR_CODE)
	{
		EquiVoid v;
		return v;
	}
	else if (code->getType() == EQ_TR_COMMA)
	{
		
	}
	else if (code->getType() == EQ_TR_EQUALITY)
	{

	}
	else if (code->getType() == EQ_TR_COMPARISON)
	{
		
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

	throwError("Unknown syntax tree node type (?????)");
	EquiVoid v;
	return v;
}

