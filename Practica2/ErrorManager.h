#ifndef PRACTICA1_ERRORMANAGER_H
#define PRACTICA1_ERRORMANAGER_H

#define NOT_FINAL_STATE 			(-100)
#define MISSING_ARGUMENTS 			(-101)
#define NOT_KEYWORDS_FILE_FOUNDED 	(-102)
#define NOT_FILE_FOUNDED 			(-103)
#define NOT_CLOSED_COMMENT 			(-104)
#define NOT_CLOSED_STRING 			(-105)
#define ILLEGAL_ESCAPED_CHARACTER 	(-106)
#define ILLEGAL_CHARACTER 			(-107)
#define ILLEGAL_LEXEMA_SIZE 		(-108)


/* Función que imprime un mensaje asociado al código de error
 * que se pasa como parametro.
 */
void showError(int errorCode, int numLinea);

#endif //PRACTICA1_ERRORMANAGER_H
