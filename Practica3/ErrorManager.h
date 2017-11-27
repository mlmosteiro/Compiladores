#ifndef PRACTICA3_ERRORMANAGER_H
#define PRACTICA3_ERRORMANAGER_H

#define MISSING_ARGUMENTS 			(-101)
#define NOT_KEYWORDS_FILE_FOUNDED 	(-102)
#define NOT_FILE_FOUNDED 			(-103)
#define NOT_INITIALIZED_VARIABLE	(-104)
#define OVERWITE_VARIABLE 			(-105)
#define ILLEGAL_ESCAPED_CHARACTER 	(-106)
#define ILLEGAL_CHARACTER 			(-107)
#define NOT_FUNCTION	 			(-108)
#define DIVISION_BY_ZERO			(-109)
#define VALUE_OF_FUNCTION			(-110)




/* Función que imprime un mensaje asociado al código de error
 * que se pasa como parametro.
 */
void showError(int errorCode, int numLinea);

#endif //PRACTICA3_ERRORMANAGER_H
