/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	auxiliary.h
 * @brief 	Headers for the auxiliary functions required by filesystem.c.
 * @date	01/03/2017
 */
#include <stdbool.h>
// inodo libre 
int ialloc(void);
int iallocBit(void);
// inodo asociado a un nombre 
int namei ( char *fileName );
// libera un inodo 
int ifree ( int inodo_id) ;

int tokenizador (char * cadena, int modo);

/*bool check (void);*/

void printFS(void);

char * getDirect(char * path);
char * getName(char * path);

void rmChild(int padre, int hijo);
void rmAllChilds(int nodo);