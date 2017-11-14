#ifndef PRACTICA1_LEXICALANALYZER_H
#define PRACTICA1_LEXICALANALYZER_H


/* Función que se encarga de inicializar el analizador léxico y reservar todos los recusos necesarios */
void initLexicalAnalyzer(char *filePath);

/* Función que devuelve el siguiente lexema reconocido del código fuente*/
void nextLexicalComponent();

/* Función que se encarga de finalizar el analizador léxico y liberar todos los recusos reservados */
void destroyLexicalAnalyzer();

#endif //PRACTICA1_LEXICALANALYZER_H
