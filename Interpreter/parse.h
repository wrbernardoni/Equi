#ifndef EQUI_PARSE_H_
#define EQUI_PARSE_H_

#include <string>
#include <vector>
#include <istream>

using namespace std;


vector<string> tokenize(string);
void syntaxTree(vector<string>);


int interpret(string);
vector<string> getLine(istream*, int&);

#endif

