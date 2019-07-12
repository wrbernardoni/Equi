#include "global.h"
#include "parse.h"
#include "compile.h"
#include <cmath>
#include <iostream>

void printCodeLine(CodeLine l)
{
	cout << "R:" << l.reg << " ";

	switch (l.cmd)
	{
		case EC_SCOPE_UP:
			cout << "Scope_Up";
		break;

		case EC_SCOPE_DOWN:
			cout << "Scope_Down";
		break;

		case EC_ADD:
			cout << "Add";
		break;

		case EC_SUBTRACT:
			cout << "Subtract";
		break;

		case EC_LOAD_CONST:
			cout << "Constant";
		break;

		case EC_LOAD_TOKEN:
			cout << "Token";
		break;

		case EC_MULTIPLY:
			cout << "Multiply";
		break;

		case EC_DIVIDE:
			cout << "Divide";
		break;

		case EC_MODULUS:
			cout << "Modulus";
		break;

		default:
			cout << "???";
	}

	
	for (int i = 0; i < l.args.size(); i++)
	{
		cout << " " << l.args[i];
	}
	cout << endl;
}

void interpretAST(vector<CodeLine>* code, SyntaxTree* ast, int reg)
{
	vector<SyntaxTree*> children = ast->getChildren();
	if (ast->getType() != EQ_TR_LOGICAL_BLOCK && ast->getType() != EQ_TR_DO_WHILE
		&& ast->getType() != EQ_TR_WHILE && ast->getType() != EQ_TR_FOR
		&& ast->getType() != EQ_TR_FUNCTION_DEC)
	{
		for (int i = 0; i < children.size(); i++)
		{
			interpretAST(code, children[i], i);
		}
	}

	if (ast->getType() == EQ_TR_ROOT || ast->getType() == EQ_TR_CODE)
	{
		//None
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
		CodeLine ln;
		string tok = ast->getTokens()[0];
		if (tok == "+")
		{
			ln.cmd = EC_ADD;
			ln.reg = reg;
			ln.args.push_back("@0");
			ln.args.push_back("@1");
		}
		else
		{
			ln.cmd = EC_SUBTRACT;
			ln.reg = reg;
			ln.args.push_back("@0");
			ln.args.push_back("@1");
		}

		code->push_back(ln);
	}
	else if (ast->getType() == EQ_TR_MULTIPLICATIVE)
	{
		CodeLine ln;
		string tok = ast->getTokens()[0];
		ln.reg = reg;
		ln.args.push_back("@0");
		ln.args.push_back("@1");
		if (tok == "*")
		{
			ln.cmd = EC_MULTIPLY;
		}
		else if (tok == "/")
		{
			ln.cmd = EC_DIVIDE;
		}
		else
		{
			ln.cmd = EC_MODULUS;
		}

		code->push_back(ln);
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
		CodeLine ln;
		string tok = ast->getTokens()[0];
		ln.cmd = EC_LOAD_CONST;
		ln.reg = reg;
		ln.args.push_back(tok);
		code->push_back(ln);
	}
	else if (ast->getType() == EQ_TR_TOKEN)
	{
		CodeLine ln;
		string tok = ast->getTokens()[0];
		ln.cmd = EC_LOAD_TOKEN;
		ln.reg = reg;
		ln.args.push_back(tok);
		code->push_back(ln);
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
		interpretAST(&code, ast[i], 0);
	}

	return code;
}