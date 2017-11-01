#ifndef PRACTICA1_SYMBOLSTABLE_H
#define PRACTICA1_SYMBOLSTABLE_H
struct symbolStruct {
    char *lexema;
    int componenteLexico;
    int numLinea;
};

typedef struct symbolStruct symbolImput;

/* Funcion que incializa la tabla de símbolos e importa las palabras
 * claves del archivo especificado.
 *
 * Es importante que el archivo contenga el delimitador indicado en
 * DELIITER_KEYWORDS al inicio y al final del bloque de palabras para
 * poder leerlo correctamente.
 *
 * Ademas, debe haber una palabra reservada con su componente lexico
 * por linea del archivo.
 * */
void initSymbolsTable(char* keywords);

/* Función que inserta un lexema en la tabla de simbolos, asociandole
 * un componente lexico y el numero de linea en el que se encontró.
 * */
void insert(char *lexema, int componenteLexico, int numLinea);

/* Función que devuelve la entrada de la tabla de símbolos correspondiente
 * al lexema que se para por parametro. Si el lexema no está en la tabla
 * de simbolos, devuelve NULL.
 */
symbolImput search(char *lexema);

/* Funcion que podria modificar el contenido de una entrada de la tabla
 * de símbolos dada. NO IMPLEMENTADA PARA ESTA PRACTICA.
 */
void modify();

/* Función que libera los recursos correspondientes a la tabla de símbolos
 */
void destroySymbolsTable();

/* Función que imprime, por orden alfabético, el contenido de la tabla de
 * simbolos en la forma [lexema - componente lexico]
 */
void printSymbolsTable();

#endif //PRACTICA1_SYMBOLSTABLE_H
