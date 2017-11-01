#ifndef PRACTICA1_LEXICALANALYZER_H
#define PRACTICA1_LEXICALANALYZER_H
#define SIZE_LA 16 // 2^4, multipo del tamaño del bloque de sistema de entrada

/*Tipo de dato que devuelve el analizador lexico*/
struct lexemaStruct {
    char *lexema;
    int compLex;
};
typedef struct lexemaStruct lexemaOutput;

/* Función que se encarga de inicializar el analizador léxico y reservar todos los recusos necesarios */
void initLexicalAnalyzer(char *filePath);

/* Función que devuelve el siguiente lexema reconocido del código fuente*/
lexemaOutput nextLexicalComponent();

/* Función que se encarga de finalizar el analizador léxico y liberar todos los recusos reservados */
void destroyLexicalAnalyzer();

#endif //PRACTICA1_LEXICALANALYZER_H
