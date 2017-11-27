#ifndef PRACTICA3_DEFINITIONS_H
#define PRACTICA3_DEFINITIONS_H

struct initFunctionStruct{
  char *name;       
  double (*fnct) (double);//function pointer
};


struct initConstantStruct{
    char *name;
    double constantValue;
};

extern struct initFunctionStruct functions[];
extern struct initConstantStruct constants[];


void loadFunctions (struct initFunctionStruct* functions);
void loadConstants (struct initConstantStruct* constants);


#endif //PRACTICA3_DEFINITIONS_H
