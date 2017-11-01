#ifndef PRACTICA1_LEXICALANALYZER_H
#define PRACTICA1_LEXICALANALYZER_H
#define SIZE_LA 4

struct lexemaStruct{
    char* lexema;
    int compLex;
};
typedef struct lexemaStruct lexemaOutput;


void initLexicalAnalyzer(char * filePath) ;
lexemaOutput nextLexicalComponent() ;
void destroyLexicalAnalyzer();
#endif //PRACTICA1_LEXICALANALYZER_H
