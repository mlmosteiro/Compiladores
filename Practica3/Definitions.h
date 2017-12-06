#ifndef PRACTICA3_DEFINITIONS_H
#define PRACTICA3_DEFINITIONS_H

// Tipo de dato para las funciones por defecto
struct initFunctionStruct{
  char *name;       
  double (*fnct) (double);//function pointer
};

// Tipo de dato para las constantes por defecto
struct initConstantStruct{
    char *name;
    double constantValue;
};

extern struct initFunctionStruct functions[];
extern struct initConstantStruct constants[];

/* Funcion que carga las funciones por defecto en la tabla
 * de símbolos*/
void loadFunctions (struct initFunctionStruct* functions);

/* Funcion que carga las constantes por defecto en la tabla
 * de simbolos*/
void loadConstants (struct initConstantStruct* constants);


#endif //PRACTICA3_DEFINITIONS_H
