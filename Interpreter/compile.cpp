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

		case EC_CREATE_TUPLE:
			cout << "Create_Tuple";
		break;

		case EC_CREATE_FRAME:
			cout << "Create_Frame";
		break;

		case EC_ADD_TO_FRAME:
			cout << "Add_To_Frame";
		break;

		case EC_FUNCTION_CALL:
			cout << "Function_Call";
		break;

		case EC_INVOKE_TASK:
			cout << "Invoke_Task";
		break;

		case EC_MOVE_REG0_TO:
			cout << "Move_Register_0_To";
		break;

		case EC_BREAK_FLAG:
			cout << "Break";
		break;

		case EC_CONTINUE_FLAG:
			cout << "Continue";
		break;

		case EC_RETURN_FLAG:
			cout << "Return";
		break;

		case EC_RESET_CONTINUE:
			cout << "Reset_Continue";
		break;

		case EC_RESET_BREAK:
			cout << "Reset_Break";
		break;

		case EC_RESET_REGISTERS:
			cout << "Reset_Registers";
		break;

		case EC_JUMP:
			cout << "Jump";
		break;

		case EC_JUMP_REL:
			cout << "Jump_Rel";
		break;

		case EC_JUMP_ALWAYS:
			cout << "Jump_Always";
		break;

		case EC_JUMP_ALWAYS_REL:
			cout << "Jump_Always_Rel";
		break;

		case EC_SET_ELSE_FLAG:
			cout << "Set_Else_Flag";
		break;

		case EC_LOAD_ELSE_FLAG:
			cout << "Load_Else_Flag";
		break;

		case EC_LOAD_NOT_ELSE_FLAG:
			cout << "Load_Not_Else_Flag";
		break;

		case EC_CLEAR_ELSE_FLAG:
			cout << "Clear_Else_Flag";
		break;

		case EC_INCREMENT:
			cout << "Increment";
		break;

		case EC_DECREMENT:
			cout << "Decrement";
		break;

		case EC_SPAWN:
			cout << "Spawn";
		break;

		case EC_SPAWN_ARRAY:
			cout << "Spawn_Array";
		break;

		case EC_IS_TOKEN:
			cout << "Is_Token";
		break;

		case EC_DEFINE_FUNCTION:
			cout << "Define_Function";
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
		&& ast->getType() != EQ_TR_FUNCTION_DEC && ast->getType() != EQ_TR_ROOT && ast->getType() != EQ_TR_CODE
		&& ast->getType() != EQ_TR_DECLARATION)
	{
		for (int i = 0; i < children.size(); i++)
		{
			interpretAST(code, children[i], i);
		}
	}

	if (ast->getType() == EQ_TR_ROOT || ast->getType() == EQ_TR_CODE)
	{
		for (int i = 0; i < children.size(); i++)
		{
			interpretAST(code, children[i], reg);
		}
	}
	else if (ast->getType() == EQ_TR_FUNCTION_DEC)
	{
		int defF = code->size();
		CodeLine ln;
		ln.cmd = EC_DEFINE_FUNCTION;
		ln.reg = reg;
		
		code->push_back(ln);

		CodeLine pop;
		pop.cmd = EC_RESET_REGISTERS;
		pop.reg = 0;
		code->push_back(pop);

		interpretAST(code, ast->getChildren()[0], 0);

		(*code)[defF].args.push_back(to_string(code->size() - defF));

		for (int i = 0; i < ast->getTokens().size(); i++)
		{
			(*code)[defF].args.push_back(ast->getTokens()[i]);
		}
	}
	else if (ast->getType() == EQ_TR_COMMA)
	{
		CodeLine ln;
		ln.cmd = EC_CREATE_TUPLE;
		ln.reg = reg;
		ln.args.push_back(to_string(children.size()));
		code->push_back(ln);
	}
	else if (ast->getType() == EQ_TR_AS)
	{
		CodeLine ln;
		ln.cmd = EC_MOVE_REG0_TO;
		ln.reg = reg;
		ln.args.push_back(to_string(reg));
		code->push_back(ln);
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
		if (ast->getTokens().size() == 2)
		{
			string tok = ast->getTokens()[1];
			string typ = ast->getTokens()[0];

			if (tok == "++" || tok == "--" || typ == "++" || typ == "--")
			{
				string op = "";
				if (tok == "++" || tok == "--")
				{
					op = tok;
					tok = typ;
				}
				else
				{
					op = typ;
				}

				CodeLine ln;
				ln.cmd = EC_LOAD_TOKEN;
				ln.reg = reg;
				ln.args.push_back(tok);
				code->push_back(ln);

				CodeLine ln2;
				ln2.reg = reg;
				ln2.args.push_back(to_string(reg));
				if (op == "++")
				{
					ln2.cmd = EC_INCREMENT;
				}
				else
				{
					ln2.cmd = EC_DECREMENT;
				}
				code->push_back(ln2);
			}
			else
			{
				if (children.size() != 0)
				{
					CodeLine checkExists;
					checkExists.cmd = EC_IS_TOKEN;
					checkExists.reg = 0;
					checkExists.args.push_back(tok);
					code->push_back(checkExists);

					CodeLine jumpIf;
					jumpIf.cmd = EC_JUMP_REL;
					jumpIf.reg = 0;
					jumpIf.args.push_back("0");
					int jumpNum = code->size();
					code->push_back(jumpIf);

					CodeLine ln;
					ln.cmd = EC_SPAWN;
					ln.reg = 0;
					ln.args.push_back(typ);
					ln.args.push_back(tok);
					code->push_back(ln);

					interpretAST(code, children[0], 1);
					CodeLine assn;
					assn.reg = reg;
					assn.cmd = EC_ASSIGN;
					assn.args.push_back("@0");
					assn.args.push_back("@1");
					code->push_back(assn);

					CodeLine skipLoad;
					skipLoad.cmd = EC_JUMP_ALWAYS_REL;
					skipLoad.reg = 0;
					skipLoad.args.push_back("2");
					code->push_back(skipLoad);

					(*code)[jumpNum].args.push_back(to_string(code->size() - jumpNum));

					CodeLine loadTok;
					loadTok.cmd = EC_LOAD_TOKEN;
					loadTok.reg = reg;
					loadTok.args.push_back(tok);
					code->push_back(loadTok);
				}
				else
				{
					CodeLine ln;
					ln.cmd = EC_SPAWN;
					ln.reg = reg;
					ln.args.push_back(typ);
					ln.args.push_back(tok);
					code->push_back(ln);
				}
			}
		}
		else // 3 args
		{
			string a0 = ast->getTokens()[0];
			string a1 = ast->getTokens()[1];
			string a2 = ast->getTokens()[2];
			
			int index = 0;
			if (isNum(a1))
			{
				index = stoi(a1);
			}

			if (a2 == "++" || a2 == "--")
			{
				//a[32]++
				CodeLine loadTok;
				loadTok.cmd = EC_LOAD_TOKEN;
				loadTok.reg = 0;
				loadTok.args.push_back(a0);
				code->push_back(loadTok);

				CodeLine cons;
				cons.cmd = EC_LOAD_CONST;
				cons.reg = 1;
				cons.args.push_back(to_string(index));
				code->push_back(cons);				

				CodeLine ln;
				ln.cmd = EC_ARRAY_ACCESS;
				ln.reg = 0;
				ln.args.push_back("@0");
				ln.args.push_back("@1");
				code->push_back(ln);

				CodeLine ln2;
				ln2.reg = reg;
				ln2.args.push_back("0");
				if (a2 == "++")
				{
					ln2.cmd = EC_INCREMENT;
				}
				else
				{
					ln2.cmd = EC_DECREMENT;
				}
				code->push_back(ln2);
			}
			else
			{
				if (children.size() != 0)
				{
					CodeLine checkExists;
					checkExists.cmd = EC_IS_TOKEN;
					checkExists.reg = 0;
					checkExists.args.push_back(a2);
					code->push_back(checkExists);

					CodeLine jumpIf;
					jumpIf.cmd = EC_JUMP_REL;
					jumpIf.reg = 0;
					jumpIf.args.push_back("0");
					int jumpNum = code->size();
					code->push_back(jumpIf);

					CodeLine ln;
					ln.cmd = EC_SPAWN_ARRAY;
					ln.reg = 0;
					ln.args.push_back(a0);
					ln.args.push_back(a1);
					ln.args.push_back(a2);
					code->push_back(ln);

					interpretAST(code, children[0], 1);
					CodeLine assn;
					assn.reg = reg;
					assn.cmd = EC_ASSIGN;
					assn.args.push_back("@0");
					assn.args.push_back("@1");
					code->push_back(assn);

					CodeLine skipLoad;
					skipLoad.cmd = EC_JUMP_ALWAYS_REL;
					skipLoad.reg = 0;
					skipLoad.args.push_back("2");
					code->push_back(skipLoad);

					(*code)[jumpNum].args.push_back(to_string(code->size() - jumpNum));

					CodeLine loadTok;
					loadTok.cmd = EC_LOAD_TOKEN;
					loadTok.reg = reg;
					loadTok.args.push_back(a2);
					code->push_back(loadTok);
				}
				else
				{
					CodeLine ln;
					ln.cmd = EC_SPAWN_ARRAY;
					ln.reg = reg;
					ln.args.push_back(a0);
					ln.args.push_back(a1);
					ln.args.push_back(a2);
					code->push_back(ln);
				}
			}
		}
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
		CodeLine ln;
		ln.cmd = EC_CREATE_FRAME;
		ln.reg = children.size();
		code->push_back(ln);

		int input = 0;

		for (int i = 1; i < children.size(); i++)
		{
			if ( children[i]->getType() == EQ_TR_AS)
			{
				CodeLine addFrame;
				addFrame.cmd = EC_ADD_TO_FRAME;
				addFrame.reg = children.size();
				addFrame.args.push_back(to_string(i));
				addFrame.args.push_back(children[i]->getTokens()[0]);
				code->push_back(addFrame);
			}
			else
			{
				if (input == 0)
				{
					input = i;
				}
			}
		}

		CodeLine fn;
		fn.cmd = EC_FUNCTION_CALL;
		fn.reg = reg;
		fn.args.push_back("@0");
		fn.args.push_back(to_string(children.size()));
		fn.args.push_back(to_string(input));
		code->push_back(fn);
	}
	else if (ast->getType() == EQ_TR_TASK)
	{
		CodeLine ln;
		ln.cmd = EC_CREATE_FRAME;
		ln.reg = children.size();
		code->push_back(ln);

		int input = 0;

		for (int i = 1; i < children.size(); i++)
		{
			if ( children[i]->getType() == EQ_TR_AS)
			{
				CodeLine addFrame;
				addFrame.cmd = EC_ADD_TO_FRAME;
				addFrame.reg = children.size();
				addFrame.args.push_back(to_string(i));
				addFrame.args.push_back(children[i]->getTokens()[0]);
				code->push_back(addFrame);
			}
			else
			{
				if (input == 0)
				{
					input = i;
				}
			}
		}

		CodeLine fn;
		fn.cmd = EC_INVOKE_TASK;
		fn.reg = reg;
		fn.args.push_back("@0");
		fn.args.push_back(to_string(children.size()));
		fn.args.push_back(to_string(input));
		code->push_back(fn);
	}
	else if (ast->getType() == EQ_TR_SPECIAL)
	{
		string tok = ast->getTokens()[0];
		if (tok == "break")
		{
			CodeLine ln;
			ln.cmd = EC_BREAK_FLAG;
			ln.reg = reg;
			code->push_back(ln);
		}
		else if (tok == "continue")
		{
			CodeLine ln;
			ln.cmd = EC_CONTINUE_FLAG;
			ln.reg = reg;
			code->push_back(ln);
		}
		else if (tok == "return")
		{
			CodeLine ln;
			ln.cmd = EC_RETURN_FLAG;
			ln.reg = reg;
			if (children.size() == 0)
			{
			}
			else
			{
				ln.args.push_back("0");
			}
			code->push_back(ln);
		}
	}
	else if (ast->getType() == EQ_TR_DO_WHILE)
	{
		CodeLine scopeUp;
		scopeUp.cmd = EC_SCOPE_UP;
		scopeUp.reg = 0;
		code->push_back(scopeUp);

		int start = code->size();

		code->push_back(scopeUp);

		interpretAST(code, children[0], reg + 1);

		CodeLine scopeDown;
		scopeDown.cmd = EC_SCOPE_DOWN;
		scopeDown.reg = 0;
		code->push_back(scopeDown);

		CodeLine catchCont;
		catchCont.cmd = EC_RESET_CONTINUE;
		catchCont.reg = 0;
		code->push_back(catchCont);

		interpretAST(code, children[1], reg + 1);
		CodeLine jumpIf;
		jumpIf.cmd = EC_JUMP_REL;
		jumpIf.reg = 0;
		jumpIf.args.push_back(to_string(reg + 1));
		jumpIf.args.push_back("-" + to_string(code->size() - start));
		code->push_back(jumpIf);

		CodeLine catchBreak;
		catchBreak.cmd = EC_RESET_BREAK;
		catchBreak.reg = 0;
		code->push_back(catchBreak);
		
		code->push_back(scopeDown);

		CodeLine clearElse;
		clearElse.cmd = EC_CLEAR_ELSE_FLAG;
		clearElse.reg = 0;
		code->push_back(clearElse);
	}
	else if (ast->getType() == EQ_TR_WHILE)
	{
		CodeLine scopeUp;
		scopeUp.cmd = EC_SCOPE_UP;
		scopeUp.reg = 0;
		code->push_back(scopeUp);

		int start = code->size();

		interpretAST(code, children[0], reg + 1);

		CodeLine skipBreak;
		skipBreak.cmd = EC_JUMP_REL;
		skipBreak.reg = 0;
		skipBreak.args.push_back(to_string(reg + 1));
		skipBreak.args.push_back("2");
		code->push_back(skipBreak);

		int endJump = code->size();
		CodeLine ln;
		ln.cmd = EC_JUMP_ALWAYS_REL;
		ln.reg = reg;
		code->push_back(ln);

		code->push_back(scopeUp);

		interpretAST(code, children[1], reg + 1);

		CodeLine scopeDown;
		scopeDown.cmd = EC_SCOPE_DOWN;
		scopeDown.reg = 0;
		code->push_back(scopeDown);

		CodeLine catchCont;
		catchCont.cmd = EC_RESET_CONTINUE;
		catchCont.reg = 0;
		code->push_back(catchCont);

		CodeLine jumpIf;
		jumpIf.cmd = EC_JUMP_ALWAYS_REL;
		jumpIf.reg = 0;
		jumpIf.args.push_back("-" + to_string((code->size() - start)));
		code->push_back(jumpIf);

		CodeLine catchBreak;
		catchBreak.cmd = EC_RESET_BREAK;
		catchBreak.reg = 0;
		code->push_back(catchBreak);

		(*code)[endJump].args.push_back(to_string(code->size() - endJump));

		code->push_back(scopeDown);

		CodeLine clearElse;
		clearElse.cmd = EC_CLEAR_ELSE_FLAG;
		clearElse.reg = 0;
		code->push_back(clearElse);
	}
	else if (ast->getType() == EQ_TR_FOR)
	{
		CodeLine scopeUp;
		scopeUp.cmd = EC_SCOPE_UP;
		scopeUp.reg = 0;
		code->push_back(scopeUp);

		interpretAST(code, children[0], reg + 1);

		int start = code->size();

		interpretAST(code, children[1], reg + 1);

		code->push_back(scopeUp);

		CodeLine skipBreak;
		skipBreak.cmd = EC_JUMP_REL;
		skipBreak.reg = 0;
		skipBreak.args.push_back(to_string(reg + 1));
		skipBreak.args.push_back("2");
		code->push_back(skipBreak);

		int endJump = code->size();
		CodeLine ln;
		ln.cmd = EC_JUMP_ALWAYS_REL;
		ln.reg = reg;
		code->push_back(ln);

		code->push_back(scopeUp);

		interpretAST(code, children[3], reg + 1);

		CodeLine scopeDown;
		scopeDown.cmd = EC_SCOPE_DOWN;
		scopeDown.reg = 0;
		code->push_back(scopeDown);
		
		CodeLine catchCont;
		catchCont.cmd = EC_RESET_CONTINUE;
		catchCont.reg = 0;
		code->push_back(catchCont);

		interpretAST(code, children[2], reg + 1);

		CodeLine jumpIf;
		jumpIf.cmd = EC_JUMP_ALWAYS_REL;
		jumpIf.reg = 0;
		jumpIf.args.push_back("-" + to_string(code->size() - start));
		code->push_back(jumpIf);

		CodeLine catchBreak;
		catchBreak.cmd = EC_RESET_BREAK;
		catchBreak.reg = 0;
		code->push_back(catchBreak);

		(*code)[endJump].args.push_back(to_string(code->size() - endJump));

		code->push_back(scopeDown);

		CodeLine clearElse;
		clearElse.cmd = EC_CLEAR_ELSE_FLAG;
		clearElse.reg = 0;
		code->push_back(clearElse);
	}
	else if (ast->getType() == EQ_TR_LOGICAL_BLOCK)
	{
		CodeLine scopeUp;
		scopeUp.cmd = EC_SCOPE_UP;
		scopeUp.reg = 0;
		code->push_back(scopeUp);

		if (ast->getTokens().size() == 1)
		{
			if (ast->getTokens()[0] == "if")
			{	// If
				CodeLine clearElse;
				clearElse.cmd = EC_CLEAR_ELSE_FLAG;
				clearElse.reg = 0;
				code->push_back(clearElse);

				interpretAST(code, children[0], reg + 1);

				CodeLine skipBreak;
				skipBreak.cmd = EC_JUMP_REL;
				skipBreak.reg = 0;
				skipBreak.args.push_back(to_string(reg + 1));
				skipBreak.args.push_back("3");
				code->push_back(skipBreak);

				CodeLine setElse;
				setElse.cmd = EC_SET_ELSE_FLAG;
				setElse.reg = 0;
				code->push_back(setElse);

				int jumpToEndNum = code->size();
				CodeLine jumpEnd;
				jumpEnd.cmd = EC_JUMP_ALWAYS_REL;
				jumpEnd.reg = 0;
				code->push_back(jumpEnd);

				interpretAST(code, children[1], reg + 1);
				code->push_back(clearElse);
				(*code)[jumpToEndNum].args.push_back(to_string(code->size() - jumpToEndNum));
			}
			else
			{	// Else
				CodeLine ifElse;
				ifElse.cmd = EC_LOAD_ELSE_FLAG;
				ifElse.reg = 0;
				code->push_back(ifElse);

				CodeLine skipBreak;
				skipBreak.cmd = EC_JUMP_REL;
				skipBreak.reg = 0;
				skipBreak.args.push_back("0");
				skipBreak.args.push_back("3");
				code->push_back(skipBreak);

				CodeLine clearElse;
				clearElse.cmd = EC_CLEAR_ELSE_FLAG;
				clearElse.reg = 0;
				code->push_back(clearElse);

				int jumpToEndNum = code->size();
				CodeLine jumpEnd;
				jumpEnd.cmd = EC_JUMP_ALWAYS_REL;
				jumpEnd.reg = 0;
				code->push_back(jumpEnd);

				interpretAST(code, children[0], reg + 1);
				code->push_back(clearElse);
				(*code)[jumpToEndNum].args.push_back(to_string(code->size() - jumpToEndNum));
			}
		}
		else
		{	// Else if
			CodeLine ifElse;
			ifElse.cmd = EC_LOAD_NOT_ELSE_FLAG;
			ifElse.reg = 0;
			code->push_back(ifElse);

			CodeLine skipElse;
			skipElse.cmd = EC_JUMP_REL;
			skipElse.reg = 0;
			skipElse.args.push_back("0");
			code->push_back(skipElse);

			int preCond = code->size() - 1;

			interpretAST(code, children[0], reg + 1);

			CodeLine skipBreak;
			skipBreak.cmd = EC_JUMP_REL;
			skipBreak.reg = 0;
			skipBreak.args.push_back(to_string(reg + 1));
			skipBreak.args.push_back("2");
			code->push_back(skipBreak);

			int jumpToEndNum = code->size();
			CodeLine jumpEnd;
			jumpEnd.cmd = EC_JUMP_ALWAYS_REL;
			jumpEnd.reg = 0;
			code->push_back(jumpEnd);

			CodeLine clearElse;
			clearElse.cmd = EC_CLEAR_ELSE_FLAG;
			clearElse.reg = 0;
			code->push_back(clearElse);

			interpretAST(code, children[1], reg + 1);
			code->push_back(clearElse);
			(*code)[preCond].args.push_back(to_string(code->size() - preCond));
			(*code)[jumpToEndNum].args.push_back(to_string(code->size() - jumpToEndNum));
		}

		CodeLine scopeDown;
		scopeDown.cmd = EC_SCOPE_DOWN;
		scopeDown.reg = 0;
		code->push_back(scopeDown);
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
		CodeLine pop;
		pop.cmd = EC_RESET_REGISTERS;
		pop.reg = 0;
		code.push_back(pop);

		interpretAST(&code, ast[i], 0);
	}

	return code;
}