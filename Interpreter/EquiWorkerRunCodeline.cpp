#include "EquiWorker.h"
#include <iostream>
#include <cmath>
#include <stack>

#include "EquiCustomFunction.h"
#include "compile.h"

extern bool isNum(string s);
extern bool isString(string s);

pair<EquiObject*, bool> EquiWorker::run(vector<CodeLine>* code)
{
	int pLine = -1;
	int lineCount = 0;
	int scopeSince = 0;
	breakFlag = false;
	continueFlag = false;

	vector<stack<pair<EquiObject*, bool>>> registers;

	while (lineCount < code->size() && pLine != lineCount)
	{
		pLine = lineCount;
		int step = 1;

		CodeLine ln = (*code)[lineCount];

		while(registers.size() <= ln.reg)
		{
			stack<pair<EquiObject*, bool>> newR;
			registers.push_back(newR);
		}

		if (ln.cmd == EC_SCOPE_UP  && !breakFlag && !continueFlag)
			scopeUp();
		else if (ln.cmd == EC_SCOPE_UP)
			scopeSince += 1;
		else if (ln.cmd == EC_SCOPE_DOWN && !breakFlag && !continueFlag)
			scopeDown();
		else if (ln.cmd == EC_SCOPE_DOWN)
			scopeSince -= 1;
		else if (ln.cmd == EC_ADD && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers to add");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for add");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for add");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();


			pair<EquiObject*, bool> ret(*o1.first + *o2.first, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_SUBTRACT && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers to subtract");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for subtract");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for subtract");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();


			pair<EquiObject*, bool> ret(*o1.first - *o2.first, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_LOAD_CONST && !breakFlag && !continueFlag)
		{
			string cnst = ln.args[0];
			EquiObject* out = NULL;
			if (cnst == "false")
			{
				EquiPrimitive<bool>* fals = new EquiPrimitive<bool>;
				fals->setData(false);
				out = fals;
			}
			else if (cnst == "true")
			{
				EquiPrimitive<bool>* tru = new EquiPrimitive<bool>;
				tru->setData(true);
				out = tru;
			}
			else if (cnst == "()")
			{
				EquiTuple* tup = new EquiTuple;
				out = tup;
			}
			else if (isString(cnst))
			{
				string s = cnst.substr(1, cnst.size() - 2);
				EquiString* str = new EquiString;
				str->setString(s);
				out = str;
			}
			else if (isNum(cnst))
			{
				double dat = stod(cnst);
				EquiPrimitive<double>* d = new EquiPrimitive<double>;
				d->setData(dat);
				out = d;
			}
			else
			{
				throwError("Unrecognized constant value");
			}

			pair<EquiObject*, bool> ret(out, true);
			registers[ln.reg].push(ret);
		}
		else if (ln.cmd == EC_LOAD_TOKEN && !breakFlag && !continueFlag)
		{
			string tok = ln.args[0];
			if (!isToken(tok) && !isType(tok))
			{
				throwError("Undefined reference to token " + tok);
			}

			if (isToken(tok))
			{
				pair<EquiObject*, bool> ret(getToken(tok), false);
				registers[ln.reg].push(ret);
			}
			else
			{
				pair<EquiObject*, bool> ret(getType(tok), false);
				registers[ln.reg].push(ret);
			}
		}
		else if (ln.cmd == EC_MULTIPLY && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers to multiply");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for multiply");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for multiply");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();


			pair<EquiObject*, bool> ret(*o1.first * *o2.first, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_DIVIDE && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers to divide");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for divide");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for divide");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();


			pair<EquiObject*, bool> ret(*o1.first / *o2.first, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_MODULUS && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers to modulus");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for modulus");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for modulus");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();


			pair<EquiObject*, bool> ret(*o1.first % *o2.first, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_LOGICAL_NEGATE && !breakFlag && !continueFlag)
		{
			if (registers.size() < 1)
				throwError("Need at least one elements in register to negate");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for negate");


			pair<EquiObject*, bool> o1 = registers[0].top();
			registers[0].pop();


			pair<EquiObject*, bool> ret(!(*o1.first), true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
		}
		else if (ln.cmd == EC_ALGEBRAIC_NEGATE && !breakFlag && !continueFlag)
		{
			if (registers.size() < 1)
				throwError("Need at least one elements in register to negate");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for negate");


			pair<EquiObject*, bool> o1 = registers[0].top();
			registers[0].pop();


			pair<EquiObject*, bool> ret(-(*o1.first), true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
		}
		else if (ln.cmd == EC_ASSIGN && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers to assign");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for assignment");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for assignment");

			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();

			if (!o1.second)
			{
				(*o1.first) = (*o2.first);
			}
			else if (o1.first->getType() == E_TUPLE_TYPE)
			{
				EquiTuple* tupA = (EquiTuple*)o1.first;
				vector<EquiObject*> oA = tupA->getTuple();
				if (o2.first->getType() == E_TUPLE_TYPE)
				{
					EquiTuple* tupB = (EquiTuple*) o2.first;
					vector<EquiObject*> oB = tupB->getTuple();
					if (oA.size() != oB.size())
					{
						for (int i = 0; i < oA.size(); i++)
						{
							if (oA[i]->getTemp())
								throwError("Cannot assign to a temporary variable");
							((*oA[i]) = (*o2.first));
						}
					}
					else
					{
						for (int i = 0; i < oA.size(); i++)
						{
							if (oA[i]->getTemp())
								throwError("Cannot assign to a temporary variable");
							((*oA[i]) = (*oB[i]));
						}
					}
				}
				else
				{
					for (int i = 0; i < oA.size(); i++)
					{
						if (oA[i]->getTemp())
							throwError("Cannot assign to a temporary variable");
						((*oA[i]) = (*o2.first));
					}
				}
			}
			else
			{
				throwError("Cannot assign to a temporary variable");
			}

			pair<EquiObject*, bool> ret(o1.first, o1.second);
			registers[ln.reg].push(ret);

			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_NOT_EQUAL && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers for comparison");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for comparison");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for comparison");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();

			EquiPrimitive<bool>* res = new EquiPrimitive<bool>;
			res->setData(*o1.first != *o2.first);

			pair<EquiObject*, bool> ret(res, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_EQUAL && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers for comparison");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for comparison");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for comparison");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();

			EquiPrimitive<bool>* res = new EquiPrimitive<bool>;
			res->setData(*o1.first == *o2.first);

			pair<EquiObject*, bool> ret(res, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_GREATER && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers for comparison");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for comparison");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for comparison");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();

			EquiPrimitive<bool>* res = new EquiPrimitive<bool>;
			res->setData(*o1.first > *o2.first);

			pair<EquiObject*, bool> ret(res, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_GREATER_EQ && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers for comparison");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for comparison");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for comparison");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();

			EquiPrimitive<bool>* res = new EquiPrimitive<bool>;
			res->setData(*o1.first >= *o2.first);

			pair<EquiObject*, bool> ret(res, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_LESS && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers for comparison");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for comparison");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for comparison");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();

			EquiPrimitive<bool>* res = new EquiPrimitive<bool>;
			res->setData(*o1.first < *o2.first);

			pair<EquiObject*, bool> ret(res, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_LESS_EQ && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers for comparison");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for comparison");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for comparison");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();

			EquiPrimitive<bool>* res = new EquiPrimitive<bool>;
			res->setData(*o1.first <= *o2.first);

			pair<EquiObject*, bool> ret(res, true);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				delete o1.first;
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_ARRAY_ACCESS && !breakFlag && !continueFlag)
		{
			if (registers.size() < 2)
				throwError("Need at least two elements in registers for array access");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for array access");
			if (registers[1].size() == 0)
				throwError("Missing item in register 1 for array access");


			pair<EquiObject*, bool> o1 = registers[0].top();
			pair<EquiObject*, bool> o2 = registers[1].top();
			registers[0].pop();
			registers[1].pop();

			EquiObject* res;
			string ts = o2.first->to_string();
			if (!isNum(ts))
			{
				res = (*o1.first)[ts];
			}
			else
			{
				res = (*o1.first)[stoi(ts)];
			}

			res->setTemp(false);

			pair<EquiObject*, bool> ret(res, o1.second);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				res->addDependant(o1.first);
			}
			if (o2.second)
			{
				delete o2.first;
			}
		}
		else if (ln.cmd == EC_MEMORY_ACCESS && !breakFlag && !continueFlag)
		{
			if (registers.size() < 1)
				throwError("Need at least one element in registers for memory access");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for memory access");


			pair<EquiObject*, bool> o1 = registers[0].top();
			registers[0].pop();

			EquiObject* res;
			string ts = ln.args[1];
			res = (*o1.first)[ts];

			res->setTemp(false);

			pair<EquiObject*, bool> ret(res, o1.second);
			registers[ln.reg].push(ret);

			if (o1.second)
			{
				res->addDependant(o1.first);
			}
		}
		else if (ln.cmd == EC_CREATE_TUPLE && !breakFlag && !continueFlag)
		{
			int numto = stoi(ln.args[0]);
			vector<EquiObject*> toTup;
			if (registers.size() < numto)
				throwError("Missing registers for tuple creation");

			for(int i = 0; i < numto; i++)
			{
				if (registers[i].size() == 0)
					throwError("Missing item in register for tuple creation");
				toTup.push_back(registers[i].top().first);
				registers[i].pop();
			}

			EquiTuple* tup = new EquiTuple;
			tup->setTuple(toTup);

			pair<EquiObject*, bool> ret(tup, true);
			registers[ln.reg].push(ret);
		}
		else if (ln.cmd == EC_CREATE_FRAME && !breakFlag && !continueFlag)
		{
			EquiFrame* f = new EquiFrame;
			pair<EquiObject*, bool> ret(f, true);
			registers[ln.reg].push(ret);
		}
		else if (ln.cmd == EC_ADD_TO_FRAME && !breakFlag && !continueFlag)
		{
			int r = stoi(ln.args[0]);
			string name = ln.args[1];

			if (registers.size() < ln.reg || registers.size() < r)
				throwError("Missing pointed to register");
			if (registers[ln.reg].size() == 0)
				throwError("Missing frame to add to");
			if (registers[ln.reg].top().first->getType() != "FRAME")
				throwError("Item pointed to is not a frame");

			pair<EquiObject*, bool> o = registers[r].top();
			registers[r].pop();
			EquiFrame* f = (EquiFrame*)registers[ln.reg].top().first;

			if (!o.second)
			{
				o.first = o.first->clone();
			}

			f->emplaceToken(name, o.first);
		}
		else if (ln.cmd == EC_FUNCTION_CALL && !breakFlag && !continueFlag)
		{
			int frameN = stoi(ln.args[1]);
			int inp = stoi(ln.args[2]);

			if (registers.size() < frameN || registers.size() < inp)
				throwError("Registers improperly loaded");
			if (registers[frameN].size() == 0)
				throwError("Missing frame to load");
			if (registers[frameN].top().first->getType() != "FRAME")
				throwError("Item pointed to is not a frame");

			EquiFrame* f = (EquiFrame*)registers[frameN].top().first;
			registers[frameN].pop();
			pair<EquiObject*, bool> input;
			pair<EquiObject*, bool> func = registers[0].top();
			registers[0].pop();

			if (inp != 0)
			{
				input = registers[inp].top();
				registers[inp].pop();
			}
			else
			{
				input.first = new EquiVoid;
				input.second = true;
			}

			pair<EquiObject*, bool> o((*func.first)(input.first, f->apparentTokens()), true);
			registers[ln.reg].push(o);

			if (func.second)
				delete func.first;
			if (input.second)
				delete input.first;
			delete f;
		}
		else if (ln.cmd == EC_MOVE_REG0_TO && !breakFlag && !continueFlag)
		{
			if (registers.size() < 1)
				throwError("Need at least one element in registers for transfer");

			if (registers[0].size() == 0)
				throwError("Missing item in register 0 for transfer");
			pair<EquiObject*, bool> o = registers[0].top();
			registers[0].pop();
			registers[ln.reg].push(o);
		}
		else if (ln.cmd == EC_BREAK_FLAG && !breakFlag && !continueFlag)
		{
			breakFlag = true;
		}
		else if (ln.cmd == EC_CONTINUE_FLAG && !breakFlag && !continueFlag)
		{
			continueFlag = true;
		}
		else if (ln.cmd == EC_RETURN_FLAG && !breakFlag && !continueFlag)
		{
			if (ln.args.size() == 0)
			{
				pair<EquiObject*, bool> o(new EquiVoid, true);
				return o;
			}
			else
			{
				if (registers.size() < 1)
				throwError("Need at least one element in registers for return");

				if (registers[0].size() == 0)
					throwError("Missing item in register 0 for return");

				pair<EquiObject*, bool> o = registers[0].top();
				registers[0].pop();

				return o;
			}
		}
		else if (ln.cmd == EC_RESET_CONTINUE)
		{
			continueFlag = false;
		}
		else if (ln.cmd == EC_RESET_BREAK)
		{
			breakFlag = false;
		}
		else if (ln.cmd == EC_RESET_REGISTERS)
		{
			breakFlag = false;
			continueFlag = false;

			for (int i = 0; i < registers.size(); i++)
			{
				while (registers[i].size() != 0)
				{
					pair<EquiObject*, bool> t = registers[i].top();
					registers[i].pop();
					if (t.second)
						delete t.first;
				}
			}

			registers.clear();
		}
		else if (ln.cmd == EC_JUMP && !breakFlag && !continueFlag)
		{
			int condN = stoi(ln.args[0]);

			if (registers.size() < condN)
				throwError("Registers improperly loaded");
			if (registers[condN].size() == 0)
				throwError("Missing boolean to load");

			pair<EquiObject*, bool> t = registers[condN].top();
			registers[condN].pop();

			EquiPrimitive<bool> fals;
			fals.setData(false);

			if (fals != *t.first)
			{
				step = 0;
				lineCount = stoi(ln.args[1]);
			}
		}
		else if (ln.cmd == EC_JUMP_REL && !breakFlag && !continueFlag)
		{
			int condN = stoi(ln.args[0]);

			if (registers.size() < condN)
				throwError("Registers improperly loaded");
			if (registers[condN].size() == 0)
				throwError("Missing boolean to load");

			pair<EquiObject*, bool> t = registers[condN].top();
			registers[condN].pop();

			EquiPrimitive<bool> fals;
			fals.setData(false);

			if (fals != *t.first)
			{
				step = stoi(ln.args[1]);
			}
		}
		else if (ln.cmd == EC_JUMP_ALWAYS && !breakFlag && !continueFlag)
		{
			step = 0;
			lineCount = stoi(ln.args[0]);
		}
		else if (ln.cmd == EC_JUMP_ALWAYS_REL && !breakFlag && !continueFlag)
		{
			step = stoi(ln.args[0]);
		}
		else if (ln.cmd == EC_SET_ELSE_FLAG && !breakFlag && !continueFlag)
		{
			elseFlag = true;
		}
		else if (ln.cmd == EC_LOAD_ELSE_FLAG && !breakFlag && !continueFlag)
		{
			EquiPrimitive<bool>* load = new EquiPrimitive<bool>;
			load->setData(elseFlag);
			pair<EquiObject*, bool> t(load, true);
			registers[ln.reg].push(t);
		}
		else if (ln.cmd == EC_LOAD_NOT_ELSE_FLAG && !breakFlag && !continueFlag)
		{
			EquiPrimitive<bool>* load = new EquiPrimitive<bool>;
			load->setData(!elseFlag);
			pair<EquiObject*, bool> t(load, true);
			registers[ln.reg].push(t);
		}
		else if (ln.cmd == EC_CLEAR_ELSE_FLAG && !breakFlag && !continueFlag)
		{
			elseFlag = false;
		}
		else if (ln.cmd == EC_INCREMENT && !breakFlag && !continueFlag)
		{

		}
		else if (ln.cmd == EC_DECREMENT && !breakFlag && !continueFlag)
		{

		}
		else if (ln.cmd == EC_SPAWN && !breakFlag && !continueFlag)
		{

		}
		else if (ln.cmd == EC_SPAWN_ARRAY && !breakFlag && !continueFlag)
		{

		}
		else if (ln.cmd == EC_IS_TOKEN && !breakFlag && !continueFlag)
		{

		}
		else if (ln.cmd == EC_DEFINE_FUNCTION && !breakFlag && !continueFlag)
		{

		}
		else if (!breakFlag && !continueFlag)
		{
			throwError("Unrecognized bytecode " + to_string(ln.cmd));
		}

		lineCount += step;
	}

	if (registers.size() >= 1)
		if (registers[0].size() != 0)
		{
			pair<EquiObject*, bool> tp = registers[0].top();
			registers[0].pop();

			for (int i = 0; i < registers.size(); i++)
			{
				while (registers[i].size() != 0)
				{
					pair<EquiObject*, bool> t = registers[i].top();
					registers[i].pop();
					if (t.second)
						delete t.first;
				}
			}

			return tp;
		}

	for (int i = 0; i < registers.size(); i++)
	{
		while (registers[i].size() != 0)
		{
			pair<EquiObject*, bool> t = registers[i].top();
			registers[i].pop();
			if (t.second)
				delete t.first;
		}
	}

	pair<EquiObject*, bool> p(new EquiVoid, false);
	return p;
}

