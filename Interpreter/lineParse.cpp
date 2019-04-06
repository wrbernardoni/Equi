#include "global.h"
#include "parse.h"
#include <cmath>
#include <iostream>

// Helper functions
#define STATE ln[state]
#define EXPECT(a) (STATE == a)

// RECURSIVE CFG RULES
SyntaxTree* code(vector<string>, int&);
SyntaxTree* line(vector<string>, int&);
SyntaxTree* expression(vector<string>, int&);
SyntaxTree* commas(vector<string>, int&);
SyntaxTree* equality(vector<string>, int&);
SyntaxTree* comparison(vector<string>, int&);
SyntaxTree* additive(vector<string>, int&);
SyntaxTree* multiplicative(vector<string>, int&);
SyntaxTree* unary(vector<string>, int&);
SyntaxTree* declaration(vector<string>, int&);
SyntaxTree* funct(vector<string>, int&);
SyntaxTree* primary(vector<string>, int&);

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
    s != "" && !isNum(s) && !isString(s));
}

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
    t = line(ln, ps);
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
  SyntaxTree* t = equality(ln, ps);
  if (t != NULL)
  {
    state = ps;
    while (SAFECHECK(ln,ps) == ",")
    {
      ps++;
      DPRINT("Eating comma")
      SyntaxTree* c = equality(ln, ps);
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
    SyntaxTree* dec = declaration(ln, ps);
    if (dec != NULL)
    {
      state = ps;
      return dec;
    }
    else
    {
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
        SyntaxTree* prim = primary(ln, ps);
        if (prim != NULL)
        {
          state = ps;
          return prim;
        }
      }
    }
  }
  return NULL;
}

SyntaxTree* declaration(vector<string> ln, int& state)
{ 
  DEBUG("declaration")
  SyntaxTree* dec = new SyntaxTree(EQ_TR_DECLARATION);
  if (isToken(SAFECHECK(ln, state)))
  {
    DPRINT("Eating token " << SAFECHECK(ln, state))
    dec->addToken(SAFECHECK(ln, state));
    state++;
    if (isToken(SAFECHECK(ln, state)))
    {
      DPRINT("Eating token " << SAFECHECK(ln, state))
      dec->addToken(SAFECHECK(ln, state));
      state++;

      if (SAFECHECK(ln, state) == "=")
      {
        state++;
        SyntaxTree* expr = expression(ln, state);
        if (expr != NULL)
        {
          dec->addChild(expr);
          return dec;
        }
      }
      else
      {
        return dec;
      }
    }
    else if (SAFECHECK(ln, state) == "=")
    {
      state++;
      SyntaxTree* expr = expression(ln, state);
      if (expr != NULL)
      {
        dec->addChild(expr);
        return dec;
      } 
    }
  }
  delete dec;
  return NULL;
}

SyntaxTree* funct(vector<string> ln, int& state)
{
  DEBUG("funct")
  SyntaxTree* fn = new SyntaxTree(EQ_TR_FUNCTION);
  if (isToken(SAFECHECK(ln,state)))
  {
    fn->addToken(ln[state]);
    state++;
    DPRINT("Eating token");
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
          fn->addChild(exp);
        return fn;
      }
    }
  }
  delete fn;
  return NULL;
}

SyntaxTree* primary(vector<string> ln, int& state)
{
  DEBUG("primary")
  if (isNum(SAFECHECK(ln,state)) || isString(SAFECHECK(ln,state)) || SAFECHECK(ln,state) == "true" || SAFECHECK(ln,state) == "false" || SAFECHECK(ln,state) == "null")
  {
    SyntaxTree* cons = new SyntaxTree(EQ_TR_CONST);
    DPRINT("Eating constant " << SAFECHECK(ln,state))
    cons->addToken(SAFECHECK(ln,state));
    state++;
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

// Runner
SyntaxTree* lineParse(vector<string> ln)
{
  int ps = 0;
  DPRINT("Constructing tree")
  SyntaxTree* t = code(ln, ps);
  DPRINT("Tree constructed")
  return t;
}

