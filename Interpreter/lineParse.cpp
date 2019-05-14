	#include "global.h"
	#include "parse.h"
	#include <cmath>
	#include <iostream>

	// Helper functions
	#define STATE ln[state]
	#define EXPECT(a) (STATE == a)

	// RECURSIVE CFG RULES
	SyntaxTree* code(vector<string>, int&);
	SyntaxTree* block(vector<string>, int&);
	SyntaxTree* logicalBlock(vector<string>, int&);
	SyntaxTree* loopBlock(vector<string>, int&);
	SyntaxTree* doWhileLoop(vector<string>, int&);
	SyntaxTree* whileLoop(vector<string>, int&);
	SyntaxTree* forLoop(vector<string>, int&);
	SyntaxTree* functDec(vector<string>, int&);
	SyntaxTree* line(vector<string>, int&);
	SyntaxTree* expression(vector<string>, int&);
	SyntaxTree* commas(vector<string>, int&);
	SyntaxTree* equality(vector<string>, int&);
	SyntaxTree* comparison(vector<string>, int&);
	SyntaxTree* additive(vector<string>, int&);
	SyntaxTree* multiplicative(vector<string>, int&);
	SyntaxTree* unary(vector<string>, int&);
	SyntaxTree* assignment(vector<string>, int&);
	SyntaxTree* declaration(vector<string>, int&);
	SyntaxTree* funct(vector<string>, int&);
	SyntaxTree* primary(vector<string>, int&);
	SyntaxTree* eq_array(vector<string>, int&);
	SyntaxTree* special(vector<string>, int&);
	SyntaxTree* memAccess(vector<string> ln, int& state);
	SyntaxTree* asOperator(vector<string> ln, int& state);
	

	bool isNum(string s)
	{
	  if (s == "")
	    return false;

	  char* end = 0;
	  double val = strtod(s.c_str(), &end); 
	  return end != s.c_str() && val != HUGE_VAL;
	}

	bool isString(string s)
	{
	  if (s.size() == 0)
	    return false;
	  
	  return s[0] == '\"' && s[s.size() - 1] == '\"';
	}

	bool isToken(string s){
	  return (s != "," && s != ";" && s != "!=" &&
	    s != "==" && s != ">" && s != "<" && s != ">=" &&
	    s != "<=" && s != "+" && s != "-" && s != "/" &&
	    s != "*" && s != "%" && s != "!" && s != "=" &&
	    s != "(" && s != ")" && s != "false" && s != "true" &&
	    s != "{" && s != "}" && s != "[" && s != "]" && s != "." &&
	    s != "&" && s != "as" &&
	    s != "break" && s != "continue" && s != "return" &&
	    s != "" && !isNum(s) && !isString(s));
	}

	extern void throwError(string s);

	#define SAFECHECK(a,b) (b < a.size() ? a[b] : "")

	//#define DEB 1
	#ifdef DEB
	#define DEBUG(a) cout << "In " << a << " looking at: " << SAFECHECK(ln,state) << endl;
	#define DPRINT(a) cout << a << endl;
	#else
	#define DEBUG(a) ;
	#define DPRINT(a) ;
	#endif

	SyntaxTree* code(vector<string> ln, int& state)
	{
	  DEBUG("code")
	  SyntaxTree* cd = new SyntaxTree(EQ_TR_CODE);
	  SyntaxTree* t = NULL;
	  do
	  {
	    int ps = state;
	    t = block(ln, ps);
	    if (t == NULL)
	    {
	      ps = state;
	      t = line(ln, ps);
	    }

	    if (t != NULL)
	    {
	      DPRINT("Line added")
	      cd->addChild(t);
	      state = ps;
	    }
	  } while (t != NULL);
	  


	  return cd;


	  if (cd->numChildren() > 0)
	    return cd;
	  else
	  {
	    delete cd;
	    return NULL;
	  }
	}

	SyntaxTree* block(vector<string> ln, int& state)
	{
	  DEBUG("block")
	  int ps = state;
	  SyntaxTree* blc = logicalBlock(ln, ps);
	  if (blc != NULL)
	  {
	    state = ps;
	    return blc;
	  }

          ps = state;
	  blc = loopBlock(ln, ps);
          if (blc != NULL)
          {
            state = ps;
            return blc;
          }

          ps = state;
          blc = functDec(ln, ps);
          if (blc != NULL)
          {
            state = ps;
            return blc;
          }

          return NULL;
	}

	SyntaxTree* logicalBlock(vector<string> ln, int& state)
	{
	  DEBUG("logicalBlock")
	  SyntaxTree* log = new SyntaxTree(EQ_TR_LOGICAL_BLOCK);

	  bool reqExpr = false;
	  if(SAFECHECK(ln, state) == "if")
	  {
	    log->addToken("if");
	    state++;
	    DPRINT("Eating if");
	    reqExpr = true;
	  }
	  else if (SAFECHECK(ln, state) == "else")
	  {
	    DPRINT("Eating else");
	    log->addToken("else");
	    state++;
	    if (SAFECHECK(ln, state) == "if")
	    {
	      DPRINT("Eating if");
	      log->addToken("if");
	      state++;
	      reqExpr = true;
	    }
	    else
	    {

	    }
	  }
	  else
	  {
	    delete log;
	    return NULL;
	  }

	  if (reqExpr)
	  {
	    if (SAFECHECK(ln, state) == "(")
	    {
	      DPRINT("Eating (");
	      state++;
	      SyntaxTree* expr = expression(ln, state);
	      if (expr != NULL && SAFECHECK(ln, state) == ")")
	      {
		DPRINT("Eating )");
		log->addChild(expr);
		state++;
	      }
	      else if (expr != NULL)
	      {
		DPRINT("Expecting ), failed to find");
		// ERROR expecting )
		delete expr;
		delete log;

		throwError("Expecting ), failed to find");
		return NULL;
	      }
	    }
	    else
	    {
	      DPRINT("Expecting logical expression")
	      delete log;
	      return NULL;
	    }
	  }

	  if (SAFECHECK(ln, state) == "{")
	  {
	    DPRINT("Eating {");
	    state++;
	    SyntaxTree* cde = code(ln, state);
	    if (SAFECHECK(ln, state) == "}")
	    {
	      DPRINT("Eating }");
	      state++;
	      if (SAFECHECK(ln, state) == ";")
	      {
		DPRINT("Eating ;");
		state++;
	      }
	      if (cde != NULL)
	      {
		log->addChild(cde);
	      }
	      else
	      {
		cde = new SyntaxTree(EQ_TR_CODE);
		log->addChild(cde);
	      }

	      return log;
	    }
	    else
	    {
	      DPRINT("Missing trailing }");
	      if (cde != NULL)
	      {
		delete cde;
	      }

	      // ERROR expecting }
	      throwError("Expecting }, failed to find");
	    }
	  }
	  else
	  {
	    int ps = state;
	    SyntaxTree* lin = line(ln, ps);
	    if (lin != NULL)
	    {
	      state = ps;
	      log->addChild(lin);
	      return log;
	    }
	    else
	    {
	      lin = block(ln, state);
	      if (lin != NULL)
	      {
		log->addChild(lin);
		return log;
	      }
	      else
	      {
		if (SAFECHECK(ln, state) == ";")
		{
		  lin = new SyntaxTree(EQ_TR_CODE);
		  log->addChild(lin);
		  return log;
		}
		else
		{
		  DPRINT("No expression after logical block")
		  // ERROR expecting expression after logical block
		  delete lin;
		  throwError("No expression after logical block");
		}
	      }
	    }
	  }

	  delete log;
	  return NULL;
	}

	SyntaxTree* loopBlock(vector<string> ln, int& state)
	{
	  DEBUG("loop")
	  int ps = state;
	  SyntaxTree* lp = doWhileLoop(ln, ps);
	  if (lp != NULL)
	  {
	    state = ps;
	    return lp;
	  }

	  ps = state;
	  lp = whileLoop(ln, ps);
	  if (lp != NULL)
	  {
	    state = ps;
	    return lp;
	  }

	  return forLoop(ln, state);
	}

	SyntaxTree* doWhileLoop(vector<string> ln, int& state)
	{
	  DEBUG("doWhile")
	  if (SAFECHECK(ln, state) == "do")
	  {
	    DPRINT("eating do");
	    state++;
	    SyntaxTree* doWhl = new SyntaxTree(EQ_TR_DO_WHILE);
	    if (SAFECHECK(ln, state) == "{")
	    {
	      DPRINT("eating {");
	      state++;
	      
	      SyntaxTree* cd = code(ln, state);
	      if (cd == NULL)
	      {
		cd = new SyntaxTree(EQ_TR_CODE);
	      }
	      doWhl->addChild(cd);

	      if (SAFECHECK(ln, state) != "}")
	      {
		DPRINT("Expecting }");
		// ERROR missing end } on do while;
		delete doWhl;
		throwError("Missing end } on do while");
		return NULL;
	      }
	      DPRINT("eating }")
	      state++;
	    }
	    else
	    {
	      int ps = state;
	      SyntaxTree* interior = block(ln, ps);
	      if (interior == NULL)
	      {
		ps = state;
		interior = line(ln, ps);

		if (interior == NULL)
		{
		  interior = new SyntaxTree(EQ_TR_CODE);
		}
	      }
	      state = ps;
	      doWhl->addChild(interior);
	    }

	    if (SAFECHECK(ln, state) == "while")
	    {
	      DPRINT("eating while");
	      state++;
	      if (SAFECHECK(ln, state) == "(")
	      {
		DPRINT("eating (");
		state++;
		SyntaxTree* condition = expression(ln, state);
		if (condition == NULL)
		{
		  condition = new SyntaxTree(EQ_TR_CODE);
		}

		doWhl->addChild(condition);
		if (SAFECHECK(ln, state) == ")" && SAFECHECK(ln, state+1) == ";")
		{
		  DPRINT("eating ) ;");
		  state += 2;
		  return doWhl;
		}
		delete doWhl;
		return NULL;
	      }
	      else
	      {
		DPRINT("Error, missing ( after while")
		//ERROR missing ( after while
		delete doWhl;
		throwError("Missing ( after while");
		return NULL;
	      }
	    }
	    else
	    {
	      DPRINT("Error, expecting while after do");
	      //ERROR expecting while after do
	      delete doWhl;
	      throwError("Expecting while after do");
	      return NULL;
	    }

	    delete doWhl;
	  }
	  return NULL;
	}

	SyntaxTree* whileLoop(vector<string> ln, int& state)
	{
	  DEBUG("while")
	  if (SAFECHECK(ln, state) == "while" && SAFECHECK(ln, state + 1) == "(")
	  {
	    DPRINT("eating while (");
	    state += 2;
	    SyntaxTree* whl = new SyntaxTree(EQ_TR_WHILE);

	    SyntaxTree* expr = expression(ln, state);
	    if (expr == NULL)
	    {
	      expr = new SyntaxTree(EQ_TR_CODE);
	    }
	    whl->addChild(expr);

	    if (SAFECHECK(ln, state) != ")")
	    {
	      DPRINT("ERROR missing )")
	      // ERROR, missing )
	      delete whl;
	      throwError("Missing )");
	      return NULL;
	    }

	    DPRINT("eating )");
	    state++;

	    if (SAFECHECK(ln, state) == "{")
	    {
	      DPRINT("eating {");
	      state++;
	      
	      SyntaxTree* cd = code(ln, state);
	      if (cd == NULL)
	      {
		cd = new SyntaxTree(EQ_TR_CODE);
	      }
	      whl->addChild(cd);

	      if (SAFECHECK(ln, state) != "}")
	      {
		DPRINT("Expecting }");
		// ERROR missing end } on while;
		delete whl;
		throwError("Missing end }");
		return NULL;
	      }
	      DPRINT("eating }")
	      state++;
	    }
	    else
	    {
	      int ps = state;
	      SyntaxTree* interior = block(ln, ps);
	      if (interior == NULL)
	      {
		ps = state;
		interior = line(ln, ps);

		if (interior == NULL)
		{
		  interior = new SyntaxTree(EQ_TR_CODE);
		}
	      }
	      state = ps;
	      whl->addChild(interior);
	    }

	    if (SAFECHECK(ln, state) == ";")
	    {
	      DPRINT("eating ;")
	      state++;
	    }

	    return whl;

	    delete whl;
	  }
	  return NULL;
	}

	SyntaxTree* forLoop(vector<string> ln, int& state)
	{
	  DEBUG("for")
	  if (SAFECHECK(ln, state) == "for" && SAFECHECK(ln, state + 1) == "(")
	  {
	    DPRINT("eating for (");
	    state += 2;
	    SyntaxTree* whl = new SyntaxTree(EQ_TR_FOR);

	    SyntaxTree* expr = expression(ln, state);
	    if (expr == NULL)
	    {
	      expr = new SyntaxTree(EQ_TR_CODE);
	    }
	    whl->addChild(expr);

	    if (SAFECHECK(ln, state) != ";")
	    {
	      //ERROR, expecting ;
	      DPRINT("ERROR, expecting ;");
	      delete whl;
	      throwError("Expecting ; in for loop clauses");
	      return NULL;
	    }
	    state++;

	    expr = expression(ln, state);
	    if (expr == NULL)
	    {
	      expr = new SyntaxTree(EQ_TR_CODE);
	    }
	    whl->addChild(expr);

	    if (SAFECHECK(ln, state) != ";")
	    {
	      //ERROR, expecting ;
	      DPRINT("ERROR, expecting ;");
	      delete whl;
	      throwError("Expecting ; in for loop clauses");
	      return NULL;
	    }
	    state++;

	    expr = expression(ln, state);
	    if (expr == NULL)
	    {
	      expr = new SyntaxTree(EQ_TR_CODE);
	    }
	    whl->addChild(expr);

	    if (SAFECHECK(ln, state) != ")")
	    {
	      DPRINT("ERROR missing )")
	      // ERROR, missing )
	      delete whl;
	      throwError("Missing ) in for loop clause");
	      return NULL;
	    }

	    DPRINT("eating )");
	    state++;

	    if (SAFECHECK(ln, state) == "{")
	    {
	      DPRINT("eating {");
	      state++;
	      
	      SyntaxTree* cd = code(ln, state);
	      if (cd == NULL)
	      {
		cd = new SyntaxTree(EQ_TR_CODE);
	      }
	      whl->addChild(cd);

	      if (SAFECHECK(ln, state) != "}")
	      {
		DPRINT("Expecting }");
		// ERROR missing end } on while;
		delete whl;
		throwError("Missing }");
		return NULL;
	      }
	      DPRINT("eating }")
	      state++;
	    }
	    else
	    {
	      int ps = state;
	      SyntaxTree* interior = block(ln, ps);
	      if (interior == NULL)
	      {
		ps = state;
		interior = line(ln, ps);

		if (interior == NULL)
		{
		  interior = new SyntaxTree(EQ_TR_CODE);
		}
	      }
	      state = ps;
	      whl->addChild(interior);
	    }

	    if (SAFECHECK(ln, state) == ";")
	    {
	      DPRINT("eating ;")
	      state++;
	    }

	    return whl;

	    delete whl;
	  }
	  return NULL;
	}

        SyntaxTree* functDec(vector<string> ln, int& state)
        {
          DEBUG("functDec")
          SyntaxTree* func = new SyntaxTree(EQ_TR_FUNCTION_DEC);

          if (SAFECHECK(ln, state) == "function")
          {
            state++;
            DPRINT("Eating function");
            if (isToken(SAFECHECK(ln, state)))
            {
              func->addToken(SAFECHECK(ln, state));
              DPRINT("Eating token " << SAFECHECK(ln, state))
              state++;
              if (SAFECHECK(ln, state) == ":" && SAFECHECK(ln, state + 1) == "(")
              {
                DPRINT("Eating : (")
                state+=2;

                while(SAFECHECK(ln, state) == "(" || isToken(SAFECHECK(ln, state)))
                {
                  string in = "";
                  if (SAFECHECK(ln, state) == "(" && SAFECHECK(ln, state + 1) == ")")
                  {
                    in = "()";
                    state+=2;
                  }
                  else if (isToken(SAFECHECK(ln, state)))
                  {
                    in = SAFECHECK(ln, state);
                    state++;
                    if (SAFECHECK(ln, state) == "[" && SAFECHECK(ln, state + 1) == "]")
                    {
                      state += 2;
                      in = in + " []";
                    }
                  }
                  else
                  {
                    delete func;
                    throwError("Expecting type in function input");
                    return NULL;
                  }

                  if (SAFECHECK(ln, state) == "[" && SAFECHECK(ln, state + 1) == "]")
                  {
                    state += 2;
                    in = in + " []";
                  }

                  if (!isToken(SAFECHECK(ln, state)))
                  {
                    delete func;
                    throwError("Expecting token name for input");
                    return NULL;
                  }

                  in = in + " " + SAFECHECK(ln, state);
                  state++;

                  func->addToken(in);

                  if (SAFECHECK(ln, state) == ",")
                    state++;
                }

                if (SAFECHECK(ln, state) != ")" || SAFECHECK(ln, state + 1) != "->")
                {
                  delete func;
                  throwError("Expecting ) or -> in function declaration " + SAFECHECK(ln, state) + " " + SAFECHECK(ln, state + 1) + " instead");
                  return NULL;
                }

                func->addToken("->");
                state += 2;
                
                string out = "";
                if (SAFECHECK(ln, state) == "(" && SAFECHECK(ln, state + 1) == ")")
                {
                  out = "()";
                  state+=2;
                }
                else if (isToken(SAFECHECK(ln, state)))
                {
                  out = SAFECHECK(ln, state);
                  state++;
                }
                else
                {
                  delete func;
                  throwError("Expecting return type on function");
                  return NULL;
                }

                if (SAFECHECK(ln, state) == "[" && SAFECHECK(ln, state + 1) == "]")
                {
                  out = out + " []";
                  state+=2;
                }

                func->addToken(out);
                if (SAFECHECK(ln, state) != "{")
                {
                  delete func;
                  throwError("Expecting code on function declaration");
                  return NULL;
                }
                state++;
                int ps = state;
                SyntaxTree* cd = code(ln, ps);
                if (cd != NULL)
                {
                  state = ps;
                }
                else
                {
                  cd = new SyntaxTree(EQ_TR_CODE);
                }
                func->addChild(cd);
                if (SAFECHECK(ln, state) != "}")
                {
                  delete func;
                  throwError("Expecting {");
                  return NULL;
                }
                state++;
                return func;
              }
            }
          }

          delete func;
          return NULL;
        }

	SyntaxTree* line(vector<string> ln, int& state)
	{
	  DEBUG("line")
	  int ps = state;
	  SyntaxTree* line = expression(ln, ps);
	  if (SAFECHECK(ln,ps) == ";")
	  {
	    ps++;
	    state = ps;
	    return line;
	  }
	  else
	  {
	    DPRINT("no ; found")
	    //ERROR no ; found
	    return NULL;
	  }
	}

	SyntaxTree* expression(vector<string> ln, int& state)
	{
	  DEBUG("expression")
	  return commas(ln, state);
	}

	SyntaxTree* commas(vector<string> ln, int& state)
	{
	  DEBUG("commas")
	  int ps = state;
	  SyntaxTree* comm = NULL;
	  SyntaxTree* t = asOperator(ln, ps);
	  if (t != NULL)
	  {
	    state = ps;
	    while (SAFECHECK(ln,ps) == ",")
	    {
	      ps++;
	      DPRINT("Eating comma")
	      SyntaxTree* c = asOperator(ln, ps);
	      if (c != NULL)
	      {
		state = ps;
		if (comm == NULL)
		{
		  comm = new SyntaxTree(EQ_TR_COMMA);
		  comm->addChild(t);
		  comm->addChild(c);
		}
		else
		{
		  comm->addChild(c);
		}
	      }
	      else
	      {
		//ERROR trailing ,
		throwError("Trailing ,");
	      }
	    }

	    if (comm != NULL)
	      return comm;
	    else
	      return t;
	  } 

	  return NULL;
	}

	SyntaxTree* equality(vector<string> ln, int& state)
	{
	  DEBUG("equality")
	  int ps = state;
	  SyntaxTree* eq = new SyntaxTree(EQ_TR_EQUALITY);
	  SyntaxTree* t = comparison(ln, ps);
	  if (t != NULL)
	  {
	    state = ps;
	    if (SAFECHECK(ln,ps) == "==" || SAFECHECK(ln,ps) == "!=")
	    {
	      if (eq->numChildren() == 0)
		eq->addChild(t);

	      if (ln[ps] == "==")
	      {
		eq->addToken("==");
	      }
	      else
	      {
		eq->addToken("!=");
	      }
	      ps++;
	      DPRINT("Eating equality")
	      SyntaxTree* c = comparison(ln, ps);
	      if (c != NULL)
	      {
		state = ps;
		eq->addChild(c);
	      }
	      else
	      {
		//ERROR trailing == or !=
		throwError("Trailing == or !=");
	      }
	    }

	    if (eq->numChildren() != 0)
	      return eq;
	    else
	    {
	      delete eq;
	      return t;
	    }
	  } 

	  return NULL;
	}

	SyntaxTree* comparison(vector<string> ln, int& state)
	{
	  DEBUG("comparison")
	  int ps = state;
	  SyntaxTree* comp = new SyntaxTree(EQ_TR_COMPARISON);
	  SyntaxTree* t = additive(ln, ps);
	  if (t != NULL)
	  {
	    state = ps;
	    if (SAFECHECK(ln,ps) == ">" || SAFECHECK(ln,ps) == ">=" || SAFECHECK(ln,ps) == "<=" || SAFECHECK(ln,ps) == "<")
	    {
	      if (comp->numChildren() == 0)
		comp->addChild(t);

	      if (ln[ps] == ">")
	      {
		comp->addToken(">");
	      }
	      else if (ln[ps] == ">=")
	      {
		comp->addToken(">=");
	      }
	      else if (ln[ps] == "<=")
	      {
		comp->addToken("<=");
	      }
	      else
	      {
		comp->addToken("<");
	      }
	      ps++;
	      DPRINT("Eating comparison")
	      SyntaxTree* c = additive(ln, ps);
	      if (c != NULL)
	      {
		state = ps;
		comp->addChild(c);
	      }
	      else
	      {
		//ERROR trailing == or !=
		throwError("Trailing comparison operator");
	      }
	    }

	    if (comp->numChildren() != 0)
	      return comp;
	    else
	    {
	      delete comp;
	      return t;
	    }
	  } 


	  return NULL;
	}

	SyntaxTree* additive(vector<string> ln, int& state)
	{
	  DEBUG("additive")
	  int ps = state;
	  SyntaxTree* add = new SyntaxTree(EQ_TR_ADDITIVE);
	  SyntaxTree* t = multiplicative(ln, ps);
	  if (t != NULL)
	  {
	    state = ps;
	    if (SAFECHECK(ln,ps) == "+" || SAFECHECK(ln,ps) == "-")
	    {
	      if (add->numChildren() == 0)
		add->addChild(t);

	      if (ln[ps] == "+")
	      {
		add->addToken("+");
	      }
	      else
	      {
		add->addToken("-");
	      }
	      ps++;
	      DPRINT("Eating additive")
	      SyntaxTree* c = additive(ln, ps);
	      if (c != NULL)
	      {
		state = ps;
		add->addChild(c);
	      }
	      else
	      {
		//ERROR trailing == or !=
		throwError("Trailing additive operator");
	      }
	    }

	    if (add->numChildren() != 0)
	      return add;
	    else
	    {
	      delete add;
	      return t;
	    }
	  } 

	  return NULL;
	}

	SyntaxTree* multiplicative(vector<string> ln, int& state)
	{
	  DEBUG("multiplicative")
	  int ps = state;
	  SyntaxTree* mult = new SyntaxTree(EQ_TR_MULTIPLICATIVE);
	  SyntaxTree* t = unary(ln, ps);
	  if (t != NULL)
	  {
	    state = ps;
	    if (SAFECHECK(ln,ps) == "*" || SAFECHECK(ln,ps) == "/" || SAFECHECK(ln,ps) == "%")
	    {
	      if (mult->numChildren() == 0)
		mult->addChild(t);

	      if (ln[ps] == "*")
	      {
		mult->addToken("*");
	      }
	      else if (ln[ps] == "/")
	      {
		mult->addToken("/");
	      }
	      else
	      {
		mult->addToken("%");
	      }
	      ps++;
	      DPRINT("Eating multiplicative")
	      SyntaxTree* c = multiplicative(ln, ps);
	      if (c != NULL)
	      {
		state = ps;
		mult->addChild(c);
	      }
	      else
	      {
		//ERROR trailing == or !=
		throwError("Trailing multiplicative operator");
	      }
	    }

	    if (mult->numChildren() != 0)
	      return mult;
	    else
	    {
	      delete mult;
	      return t;
	    }
	  } 

	  return NULL;
	}

	SyntaxTree* unary(vector<string> ln, int& state)
	{
	  DEBUG("unary")
	  SyntaxTree* un = new SyntaxTree(EQ_TR_UNARY);
	  int ps = state;
	  if (SAFECHECK(ln,ps) == "!" || SAFECHECK(ln,ps) == "-")
	  {
	    bool logicalNegation = SAFECHECK(ln,ps) == "!";
	    ps++;
	    DPRINT("Eating unary")
	    SyntaxTree* c = unary(ln, ps);
	    if (c != NULL)
	    {
	      if (logicalNegation)
	      {
		un->addToken("!");
	      }
	      else
		un->addToken("-");
	      state = ps;
	      un->addChild(c);
	      return un;
	    }
	    return NULL;
	  }
	  else
	  {
	    delete un;
	    SyntaxTree* dec = assignment(ln, ps);
	    if (dec != NULL)
	    {
	      state = ps;
	      return dec;
	    }
	    else
	    {
	      ps = state;
	      dec = declaration(ln, ps);
	      if (dec != NULL)
	      {
		state = ps;
		return dec;
	      }

	      ps = state;
	      SyntaxTree* func = funct(ln, ps);
	      if (func != NULL)
	      {
		state = ps;
		return func;
	      }
	      else
	      {
		ps = state;
		SyntaxTree* prim = memAccess(ln, ps);
		if (prim != NULL)
		{
		  state = ps;
		  return prim;
		}
		else
		{
		  ps = state;
		  prim = eq_array(ln, ps);
		  if (prim != NULL)
		  {
		    state = ps;
		    return prim;
		  }

		  ps = state;
		  prim = primary(ln, ps);
		  if (prim != NULL)
		  {
		    state = ps;
		    return prim;
		  }
		}
	      }
	    }
	  }
	  return NULL;
	}

	SyntaxTree* assignment(vector<string> ln, int& state)
	{
	  DEBUG("assignment")
	  SyntaxTree* assign = new SyntaxTree(EQ_TR_ASSIGNMENT);

	  int ps = state;
	  SyntaxTree* lhs = declaration(ln, ps);
	  if (lhs == NULL)
	  {
	    ps = state;
	    lhs = memAccess(ln, ps);
	    if (lhs == NULL)
	    {
	      ps = state;
	      lhs = eq_array(ln, ps);
	      if (lhs == NULL)
	      {
		ps = state;
		lhs = primary(ln, ps);
		if (lhs == NULL)
		{
		  delete assign;
		  return NULL;
		}
	      }
	    }
	  }

	  state = ps;
	  assign->addChild(lhs);

	  if (SAFECHECK(ln, state) != "=")
	  {
	    delete assign;
	    return NULL;
	  }

	  state++;

	  SyntaxTree* rhs = expression(ln, state);

	  if (rhs == NULL)
	  {
	    delete assign;
	    throwError("Expecting expression after = sign");
	    return NULL;
	  }

	  assign->addChild(rhs);

	  return assign;
	}

	SyntaxTree* declaration(vector<string> ln, int& state)
	{ 
	  DEBUG("declaration")
	  SyntaxTree* dec = new SyntaxTree(EQ_TR_DECLARATION);
	  if (isToken(SAFECHECK(ln, state)) || SAFECHECK(ln, state) == "(")
	  {
	    if (isToken(SAFECHECK(ln, state)))
	    {
	      DPRINT("Eating token " << SAFECHECK(ln, state))
	      dec->addToken(SAFECHECK(ln, state));
	      state++;
	    }
	    else
	    {
	      DPRINT("Eating (");
	      state++;

	      if (SAFECHECK(ln, state) == ")")
	      {
		state++;
		dec->addToken("()");
	      }
	      else
	      {
		delete dec;
		return NULL;
	      }
	    }

	    if (SAFECHECK(ln, state) == "[")
	    {
	      DPRINT("Eating [")
	      state++;

	      if (isNum(SAFECHECK(ln, state)))
	      {
		dec->addToken(ln[state]);
		state++;
	      }
	      else if (isToken(SAFECHECK(ln, state)))
	      {
		dec->addToken("[" + ln[state] + "]");
		state++;
	      }
	      else
	      {
		dec->addToken("0");
	      }

	      if (SAFECHECK(ln, state) == "]")
	      {
		state++;
	      }
	      else
	      {
		delete dec;
		return NULL;
	      }
	    }

	    if (isToken(SAFECHECK(ln, state)))
	    {
	      DPRINT("Eating token " << SAFECHECK(ln, state))
	      dec->addToken(SAFECHECK(ln, state));
	      state++;

	      if (SAFECHECK(ln, state) == "=" && SAFECHECK(ln, state+1) == "&")
	      {
	      	state += 2;
	      	SyntaxTree* rhs = expression(ln, state);
	      	if (rhs != NULL)
	      	{
	      		dec->addChild(rhs);
	      		return dec;
	      	}
	      }
	      else
	      	return dec;
	    }
	  }
	  delete dec;
	  return NULL;
	}

	SyntaxTree* funct(vector<string> ln, int& state)
	{
	  DEBUG("funct")
	  SyntaxTree* fn = new SyntaxTree(EQ_TR_FUNCTION);

	  int ps = state;
	  SyntaxTree* pm = memAccess(ln, ps);
	  if (pm == NULL)
	  {
	    ps = state;
	    pm = eq_array(ln, ps);

	    if (pm == NULL)
	    {
	      ps = state;
	      pm = primary(ln, ps);
	    }
	  }

	  if (pm != NULL)
	  {
	    state = ps;
	    fn->addChild(pm);
	    DPRINT("Eating operand");

	    if (SAFECHECK(ln,state) == "(")
	    {
	      state++;
	      DPRINT("Eating (")
	      SyntaxTree* exp = expression(ln, state);
	      if (SAFECHECK(ln,state) == ")")
	      {
			state++;
			DPRINT("Eating )");
			if (exp != NULL)
			{
				if (exp->getType() == EQ_TR_COMMA)
				{
					bool through = true;
					int start = 0;
					do
					{
						through = true;
						vector<SyntaxTree*> c = exp->getChildren();
						for(int i = start; i < c.size(); i++)
						{
							if (c[i]->getType() == EQ_TR_AS)
							{
								through = false;
								start = i;
								fn->addChild(c[i]);
								exp->drop(i);
								break;
							}
						}
					} while(!through);
				}

				if (exp->getChildren().size() == 1 && exp->getType() == EQ_TR_COMMA)
				{
					SyntaxTree* t = exp->getChildren()[0];
					exp->drop(0);
					delete exp;
					exp = t;
				}
				else if (exp->getChildren().size() == 0 && exp->getType() == EQ_TR_COMMA)
				{
					delete exp;
					exp = NULL;
				}

				if (exp != NULL)
			  		fn->addChild(exp);
			}
			return fn;
	      }
	    }
	  }
	  delete fn;
	  return NULL;
	}

	SyntaxTree* asOperator(vector<string> ln, int& state)
	{
		DEBUG("as")
		SyntaxTree* lh = equality(ln, state);
		if (lh == NULL || SAFECHECK(ln, state) != "as" || !isToken(SAFECHECK(ln, state + 1)))
		{
			return lh;
		}

		SyntaxTree* fn = new SyntaxTree(EQ_TR_AS);

		fn->addChild(lh);
		fn->addToken(SAFECHECK(ln, state + 1));
		state += 2;

		return fn;

	}

	SyntaxTree* primary(vector<string> ln, int& state)
	{
	  DEBUG("primary")
	  int ps = state;
	  SyntaxTree* spec = special(ln, ps);
	  if (spec != NULL)
	  {
	    state = ps;
	    return spec;
	  }

	  ps = state;

	  if (isNum(SAFECHECK(ln,state)) || isString(SAFECHECK(ln,state)) || SAFECHECK(ln,state) == "true" || SAFECHECK(ln,state) == "false" || SAFECHECK(ln,state) == "null" || (SAFECHECK(ln, state) == "(" && SAFECHECK(ln, state + 1) == ")"))
	  {
	    SyntaxTree* cons = new SyntaxTree(EQ_TR_CONST);
            if (isNum(SAFECHECK(ln, state)) && (SAFECHECK(ln, state + 1) == ".") && isNum(SAFECHECK(ln, state + 2)))
            {
              string n = SAFECHECK(ln,state);
              n = n + SAFECHECK(ln, state + 1);
              n = n + SAFECHECK(ln, state + 2);
              DPRINT("Eating constant " << n);
              cons->addToken(n);
              state += 3;
            }
            else
            {
              if (SAFECHECK(ln, state) == "(")
              {
                DPRINT("Eating constant ()");
                cons->addToken("()");
                state += 2;
              }
              else
              {
	        DPRINT("Eating constant " << SAFECHECK(ln,state))
	        cons->addToken(SAFECHECK(ln,state));
                state++;
              }
            }
            return cons;
	  }
	  else if (isToken(SAFECHECK(ln,state)))
	  {
	    SyntaxTree* tok = new SyntaxTree(EQ_TR_TOKEN);
	    DPRINT("Eating token " << ln[state])
	    tok->addToken(ln[state]);
	    state++;
	    return tok;
	  }
	  else if (SAFECHECK(ln,state) == "(")
	  {
	    DPRINT("Eating (")
	    state++;
	    SyntaxTree* expr = expression(ln, state);
	    if (expr != NULL)
	    {
	      if (SAFECHECK(ln,state) == ")")
	      {
		state++;
		DPRINT("Eating )")
		return expr;
	      }
	      delete expr;
	    }
	  }
	  
	  return NULL;
	}

	SyntaxTree* eq_array(vector<string> ln, int& state)
	{
	  DEBUG("array");
	  vector<SyntaxTree*> linAr;

	  SyntaxTree* tok = primary(ln, state);

	  while (tok != NULL)
	  {
	    linAr.push_back(tok);

	    if (!(SAFECHECK(ln, state) == "["))
	    {
	      tok = NULL;
	    }
	    else
	    {
	      DPRINT("Eating [")
	      state++;
	      tok = expression(ln, state);

	      if (!(SAFECHECK(ln, state) == "]"))
	      {
		throwError("Expecting ]");
		delete tok;

		for (int i = 0; i < linAr.size(); i++)
		{
		  delete linAr[i];
		}

		return NULL;
	      }
	      state++;
	      DPRINT("Eating ]")
	    }
	  }

	  if (linAr.size() == 0)
	  {
	    return NULL;
	  }
	  else if (linAr.size() == 1)
	  {
	    delete linAr[0];
	    return NULL;
	  }

	  SyntaxTree* par = NULL;
	  for (int i = 1; i < linAr.size(); i++)
	  {
	    SyntaxTree* ar = new SyntaxTree(EQ_TR_ARRAY);
	    if (par == NULL)
	    {
	      ar->addChild(linAr[0]);
	    }
	    else
	    {
	      ar->addChild(par);
	    }

	    ar->addChild(linAr[i]);
	    par = ar;
	  }

	  return par;
	}

	SyntaxTree* memAccess(vector<string> ln, int& state)
	{
	  DEBUG("memAccess");
	  vector<string> linAr;

	  int ps = state;
	  ps = state;
	  SyntaxTree* tok = eq_array(ln, ps);
	  if (tok == NULL)
	  {
	    ps = state;
	    tok = primary(ln, ps);

	    if (tok == NULL)
	    {
	      return NULL;
	    }
	  }

	  state = ps;
	  

	  while (SAFECHECK(ln, state) == ".")
	  {
	    DPRINT("Eating .")
	    state++;
	    if (isToken(SAFECHECK(ln,state)))
	    {
	      DPRINT("Eating token " << ln[state])
	      linAr.push_back(ln[state]);
	      state++;
	    }
	    else
	    {
	      delete tok;
	      throwError("Expecting token after .");
	    }
	  }

	  if (linAr.size() == 0)
	  {
	    delete tok;
	    return NULL;
	  }

	  DPRINT("Constructing memAccess tree")

  SyntaxTree* par = tok;
  for (int i = 0; i < linAr.size(); i++)
  {
    SyntaxTree* ar = new SyntaxTree(EQ_TR_MEMACCESS);
    ar->addChild(par);
    ar->addToken(linAr[i]);
    par = ar;
  }
  return par;
}

SyntaxTree* special(vector<string> ln, int& state)
{
  DEBUG("special");

  if (SAFECHECK(ln, state) == "break")
  {
    DPRINT("Eating break");
    state++;
    SyntaxTree* expr = new SyntaxTree(EQ_TR_SPECIAL);
    expr->addToken("break");
    return expr;
  }
  else if (SAFECHECK(ln, state) == "continue")
  {
    DPRINT("Eating continue");
    state++;
    SyntaxTree* expr = new SyntaxTree(EQ_TR_SPECIAL);
    expr->addToken("continue");
    return expr;
  }
  else if (SAFECHECK(ln, state) == "return")
  {
  	DPRINT("Eating return");
  	state++;
  	int ps = state;
  	SyntaxTree* expr = new SyntaxTree(EQ_TR_SPECIAL);
  	expr->addToken("return");
  	SyntaxTree* o = expression(ln, ps);
  	if (o != NULL)
  	{
  		state = ps;
  		expr->addChild(o);
  	}

  	return expr;
  }

  return NULL;
}

// Runner
SyntaxTree* lineParse(vector<string> ln)
{
  int ps = 0;
  DPRINT("Constructing tree")
  SyntaxTree* t = code(ln, ps);
  DPRINT("Tree constructed")
  return t;
}

