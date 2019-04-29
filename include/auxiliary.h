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
// inodo asociado a un nombre 
int namei ( char *fileName );
// libera un inodo 
int ifree ( int inodo_id) ;

void tokenaizor (char * cadena);
bool check (void);