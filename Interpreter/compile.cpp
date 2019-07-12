#include "global.h"
#include "parse.h"
#include "compile.h"
#include <cmath>
#include <iostream>

void printCodeLine(CodeLine)
{
	cout << "Yolo swag" << endl;
}

void interpretAST(vector<CodeLine>* code, SyntaxTree* ast)
{
	vector<SyntaxTree*> children = ast->getChildren();
	if (ast->getType() != EQ_TR_LOGICAL_BLOCK && ast->getType() != EQ_TR_DO_WHILE
		&& ast->getType() != EQ_TR_WHILE && ast->getType() != EQ_TR_FOR
		&& ast->getType() != EQ_TR_FUNCTION_DEC)
	{
		for (int i = 0; i < children.size(); i++)
		{
			interpretAST(code, children[i]);
		}
	}

	if (ast->getType() == EQ_TR_ROOT || ast->getType() == EQ_TR_CODE)
	{

	}
	else if (ast->getType() == EQ_TR_FUNCTION_DEC)
	{

	}
	else if (ast->getType() == EQ_TR_COMMA)
	{

	}
	else if (ast->getType() == EQ_TR_AS)
	{

	}
	else if (ast->getType() == EQ_TR_EQUALITY)
	{

	}
	else if (ast->getType() == EQ_TR_COMPARISON)
	{

	}
	else if (ast->getType() == EQ_TR_ADDITIVE)
	{

	}
	else if (ast->getType() == EQ_TR_MULTIPLICATIVE)
	{

	}
	else if (ast->getType() == EQ_TR_UNARY)
	{

	}
	else if (ast->getType() == EQ_TR_ARRAY)
	{

	}
	else if (ast->getType() == EQ_TR_MEMACCESS)
	{

	}
	else if (ast->getType() == EQ_TR_ASSIGNMENT)
	{

	}
	else if (ast->getType() == EQ_TR_DECLARATION)
	{

	}
	else if (ast->getType() == EQ_TR_CONST)
	{

	}
	else if (ast->getType() == EQ_TR_TOKEN)
	{

	}
	else if (ast->getType() == EQ_TR_FUNCTION)
	{

	}
	else if (ast->getType() == EQ_TR_SPECIAL)
	{

	}
	else if (ast->getType() == EQ_TR_DO_WHILE)
	{

	}
	else if (ast->getType() == EQ_TR_WHILE)
	{

	}
	else if (ast->getType() == EQ_TR_FOR)
	{

	}
	else if (ast->getType() == EQ_TR_LOGICAL_BLOCK)
	{

	}
	else
	{
		// Unknown syntax tree type
	}
}

vector<CodeLine> compile(vector<SyntaxTree*> ast)
{
	vector<CodeLine> code;
	
	for (int i = 0; i < ast.size(); i++)
	{
		interpretAST(&code, ast[i]);
	}

	return code;
}