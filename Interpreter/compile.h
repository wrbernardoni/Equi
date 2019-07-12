#ifndef EQUI_COMPILE_CMDS_H_
#define EQUI_COMPILE_CMDS_H_

/*
struct CodeLine
{
	int cmd;
	int reg;
	vector<string> args;
};
*/

// Registers are stacks, when used they pop

#define EC_SCOPE_UP 0
#define EC_SCOPE_DOWN 1
#define EC_ADD 2
#define EC_SUBTRACT 3
#define EC_MULTIPLY 4
#define EC_DIVIDE 5
#define EC_MODULUS 6
#define EC_LOAD_CONST 7
#define EC_LOAD_TOKEN 8

#endif
