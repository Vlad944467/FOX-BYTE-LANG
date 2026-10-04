#ifndef OPCODES_H
#define OPCODES_H
enum opcode {
    OP_MOV,
    OP_ADD,
    OP_SUB,
    OP_INT_PRINT,
    OP_PRINT_C,
    OP_ADD_NUM,
    OP_SUBT_NUM,
    OP_CLEAR,
    OP_MUL_NUM,
    OP_DIV_NUM,
    OP_STR,
    OP_PRINT_STR,
    OP_FMOV,
    OP_PRINTFL,
    OP_MOVC,
    OP_PRINTC,
    OP_STRLEN,
    OP_FADD,
    OP_INPUT_INT,
    OP_INPUT_STR,
    OP_DEC,
    OP_INC,
    OP_COPY,
    OP_SAVEV,
    OP_LOADV,
    OP_INPUTF,
    OP_INPUTC,
    OP_IF,
    OP_CM
};
#endif
