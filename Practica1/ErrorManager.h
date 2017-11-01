#ifndef PRACTICA1_ERRORMANAGER_H
#define PRACTICA1_ERRORMANAGER_H

void printError(int errorCode, int line, char *lexema);

#define MISSING_ARGUMENTS (-107)
#define NOT_FINAL_STATE (-1)
#define NOT_KEYWORDS_FILE_FOUNDED (-101)
#define NOT_FILE_FOUNDED (-102)
#define NOT_CLOSED_COMMENT (-103)
#define NOT_CLOSED_STRING (-104)
#define ILLEGAL_ESCAPED_CHARACTER (-105)
#define ILLEGAL_CHARACTER (-106)
#define ILLEGAL_LEXEMA_SIZE (-108)

void showError(int errorCode, int numLinea);

#endif //PRACTICA1_ERRORMANAGER_H
