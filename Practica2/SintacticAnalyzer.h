#ifndef PRACTICA1_SINTACTICLANALYZER_H
#define PRACTICA1_SINTACTICLANALYZER_H



/* Función que se encarga de inicializar el analizador sintáctico y reservar todos los recusos necesarios */
void initSintacticAnalyzer(char *filepath);

/* Función que inicua el análisis sintactico del código fuente*/
void startSintacticAnalisis();

/* Función que se encarga de finalizar el analizador sintáctico y liberar todos los recusos reservados */
void destroySintacticAnalyzer();

#endif //PRACTICA1_SINTACTICLANALYZER_H
