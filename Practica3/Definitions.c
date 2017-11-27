
#include "Definitions.h"
#include "consolaMatematica.tab.h"
#include <math.h>


//default values for symbol table
struct initFunctionStruct functions[] = {
    {"atan", atan},
    {"cos",  cos},
    {"exp",  exp},
    {"ln",   log},
    {"sin",  sin},
    {"sqrt", sqrt},
    {0,      0},
};

struct initConstantStruct constants[] = {
    {"pi", 3.14159265359},
    {"e",  2.71828182846},
    {0,    0},
};

//puts the functions in the symbol table
void loadFunctions(struct initFunctionStruct *functions) {
    int i;
    for (i = 0; functions[i].name != 0; i ++) {
        symbolInput *input = insert(functions[i].name, FNCT);
        input->value.fnctptr = functions[i].fnct;
        input->initialized = true;
    }
}

//puts the constants in the symbol table
void loadConstants(struct initConstantStruct *constants) {
    int i;
    for (i = 0; constants[i].name != 0; i ++) {
        symbolInput *input = insert(functions[i].name, - 1);
        input->value.var = constants[i].constantValue;
        input->initialized=true;
    }
}