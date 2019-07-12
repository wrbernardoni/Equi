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

		case EC_LOGICAL_NEGATE:
			cout << "(!)Logical_Negate";
		break;

		case EC_ALGEBRAIC_NEGATE:
			cout << "(-)Algebraic_Negate";
		break;

		case EC_ASSIGN:
			cout << "Assign";
		break;

		case EC_NOT_EQUAL:
			cout << "Not_Equal";
		break;

		case EC_EQUAL:
			cout << "Equal";
		break;

		case EC_GREATER:
			cout << "Greater_Than";
		break;

		case EC_GREATER_EQ:
			cout << "Greater_Than_Or_Equal";
		break;

		case EC_LESS:
			cout << "Less_Than";
		break;

		case EC_LESS_EQ:
			cout << "Less_Than_Or_Equal";
		break;

		case EC_ARRAY_ACCESS:
			cout << "Array";
		break;

		case EC_MEMORY_ACCESS:
			cout << "Memory";
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
		CodeLine ln;
		string tok = ast->getTokens()[0];
		ln.reg = reg;
		ln.args.push_back("@0");
		ln.args.push_back("@1");
		if (tok == "!=")
		{
			ln.cmd = EC_NOT_EQUAL;
		}
		else
		{
			ln.cmd = EC_EQUAL;
		}

		code->push_back(ln);
	}
	else if (ast->getType() == EQ_TR_COMPARISON)
	{
		CodeLine ln;
		string tok = ast->getTokens()[0];
		ln.reg = reg;
		ln.args.push_back("@0");
		ln.args.push_back("@1");
		if (tok == ">")
		{
			ln.cmd = EC_GREATER;
		}
		else if (tok == ">=")
		{
			ln.cmd = EC_GREATER_EQ;
		}
		else if (tok == "<")
		{
			ln.cmd = EC_LESS;
		}
		else
		{
			ln.cmd = EC_LESS_EQ;
		}

		code->push_back(ln);

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
		CodeLine ln;
		string tok = ast->getTokens()[0];
		ln.reg = reg;
		ln.args.push_back("@0");
		if (tok == "!")
		{
			ln.cmd = EC_LOGICAL_NEGATE;
		}
		else
		{
			ln.cmd = EC_ALGEBRAIC_NEGATE;
		}

		code->push_back(ln);
	}
	else if (ast->getType() == EQ_TR_ARRAY)
	{
		CodeLine ln;
		ln.cmd = EC_ARRAY_ACCESS;
		ln.reg = reg;
		ln.args.push_back("@0");
		ln.args.push_back("@1");
		code->push_back(ln);
	}
	else if (ast->getType() == EQ_TR_MEMACCESS)
	{
		CodeLine ln;
		string tok = ast->getTokens()[0];
		ln.cmd = EC_MEMORY_ACCESS;
		ln.reg = reg;
		ln.args.push_back("@0");
		ln.args.push_back(tok);
		code->push_back(ln);
	}
	else if (ast->getType() == EQ_TR_ASSIGNMENT)
	{
		CodeLine ln;
		ln.reg = reg;
		ln.cmd = EC_ASSIGN;
		ln.args.push_back("@0");
		ln.args.push_back("@1");

		code->push_back(ln);
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