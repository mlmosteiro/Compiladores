#ifndef PRACTICA1_LEXICALANALYZER_H
#define PRACTICA1_LEXICALANALYZER_H
#define SIZE_LA 16 // 2^4, multipo del tamaño del bloque de sistema de entrada

struct lexemaStruct {
    char *lexema;
    int compLex;
};
typedef struct lexemaStruct lexemaOutput;


void initLexicalAnalyzer(char *filePath);

lexemaOutput nextLexicalComponent();

void destroyLexicalAnalyzer();

#endif //PRACTICA1_LEXICALANALYZER_H
