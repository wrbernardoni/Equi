#ifndef EQUI_SYNTAX_TREE_H_
#define EQUI_SYNTAX_TREE_H_

#include <string>
#include <vector>

using namespace std;


#define EQ_TR_ROOT "root"
#define EQ_TR_CODE "code"
#define EQ_TR_COMMA "comma"
#define EQ_TR_EQUALITY "equality"
#define EQ_TR_COMPARISON "comparison"
#define EQ_TR_ADDITIVE "additive"
#define EQ_TR_MULTIPLICATIVE "multiplicative"
#define EQ_TR_UNARY "unary"
#define EQ_TR_DECLARATION "declaration"
#define EQ_TR_CONST "constant"
#define EQ_TR_TOKEN "token"
#define EQ_TR_FUNCTION "function"

class SyntaxTree
{
private:
  string type;
  vector<SyntaxTree*> children;
  vector<string> tokens;
public:
  ~SyntaxTree();
  SyntaxTree(string);
  void print(int);
  void addChild(SyntaxTree*);
  void addToken(string);
  int numChildren();

};

#endif


