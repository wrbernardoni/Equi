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
#define EC_LOGICAL_NEGATE 9
#define EC_ALGEBRAIC_NEGATE 10
#define EC_ASSIGN 11
#define EC_NOT_EQUAL 12
#define EC_EQUAL 13
#define EC_GREATER 14
#define EC_GREATER_EQ 15
#define EC_LESS 16
#define EC_LESS_EQ 17
#define EC_ARRAY_ACCESS 18
#define EC_MEMORY_ACCESS 19
#define EC_CREATE_TUPLE 20
#define EC_CREATE_FRAME 21
#define EC_ADD_TO_FRAME 22
#define EC_FUNCTION_CALL 23 //@0, frame_address, input_address (0 if none)
#define EC_MOVE_REG0_TO 24
#define EC_BREAK_FLAG 25
#define EC_CONTINUE_FLAG 26
#define EC_RETURN_FLAG 27 // Has argument of 0 if there is an item to return in reg0

#endif
