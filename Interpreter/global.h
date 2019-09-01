#ifndef EQUI_GLOBAL_H_
#define EQUI_GLOBAL_H_

#include <string>

#define P_VERB(a,b) if(verbose >= b) cout << a;

extern int verbose;
extern int numThreads;
extern bool failsafe;
extern bool fullParse;
extern bool verbCompiled;

#define TOKEN_PRINT_VERB 1
#define ALL_STEP_VERB 2
#define TOKEN_LIST_VERB 100
#define SYNTAX_TREE_LIST 99
#define STEP_LIST 1000

#define MIN(a,b) ((a < b) ? a : b)
#define MAX(a,b) ((a > b) ? a : b)


extern bool isNum(std::string s);

#include "EquiCore.h"

extern EquiCore* globalCore;

#endif
