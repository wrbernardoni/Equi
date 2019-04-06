#ifndef EQUI_GLOBAL_H_
#define EQUI_GLOBAL_H_

#define P_VERB(a,b) if(verbose >= b) cout << a;

extern int verbose;
#define TOKEN_PRINT_VERB 1
#define ALL_STEP_VERB 2
#define TOKEN_LIST_VERB 100
#define SYNTAX_TREE_LIST 99

#define MIN(a,b) ((a < b) ? a : b)
#define MAX(a,b) ((a > b) ? a : b)

#endif
