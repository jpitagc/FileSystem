/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	filesystem.c
 * @brief 	Implementation of the core file system funcionalities and auxiliary functions.
 * @date	01/03/2017
 */

#include "include/filesystem.h" // Headers for the core functionality
#include "include/auxiliary.h"  // Headers for auxiliary functions
#include "include/metadata.h"   // Type and structure declaration of the file system

#include <string.h>
#include <stdint.h>
#include <inttypes.h> 


Superblock superblock;
INodo iNodos[ELEM_IN_FILE_SYS]; //Array donde se almacenan los i-nodos



char * carpetas[DEEPTH_MAX + 1];
uint8_t ultim=-1 ;
/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */
int mkFS(long deviceSize)
{  
	 //NF9: El sistema de ficheros será usado en discos de 50 KiB a 10 MiB
	if (deviceSize < FILE_SYS_MIN_SIZE || deviceSize > FILE_SYS_MAX_SIZE) return -1;
	superblock.size =(deviceSize/BLOCK_SIZE)-1;
	superblock.firstFreeInode = 0;
	for(int i=0; i<ELEM_IN_FILE_SYS; i++){
		memset(&(iNodos[i]), 0, sizeof(INodo) );
		iNodos[i].type = EMPTY;
		iNodos[i].sub_libre = 0;
	}
	return 0;
}

/*
 * @brief 	Mounts a file system in the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int mountFS(void)
{
	return -1;
}

/*
 * @brief 	Unmounts the file system from the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int unmountFS(void)
{
	return -1;
}

/*
 * @brief	Creates a new file, provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the file already exists, -2 in case of error.
 */
int createFile(char *path)
{  
    // Comprobamos que el path no supera el limite permitido 
    if (strlen(path)>FULLPATH_MAX_FILE){
    	return -2;
    }

    // Obtenemos si existe un nodo libre 
    uint8_t hijo = ialloc();
    if(hijo == -1){
    	return -2;
    } 

    // CHECK DEL PATH


    // Obtenemos el inodo padre del que se va a crear
     uint8_t padre = namei(carpetas(ultim-1));
     if(iNodos[padre].sub_libre>9 || iNodos[padre].type = TYPE_FILE){
     	return -2;
     }else {
       iNodos[hijo].name = carpetas(ultim);
       iNodos[hijo].type = TYPE_FILE;
       iNodos[padre].subdirectorios[sub_libre]= hijo;
     }



    

     
	return -2;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *path)
{
	return -2;
}

/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *path)
{
	return -2;
}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{
	return -1;
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{
	return -1;
}

/*
 * @brief	Writes a number of bytes from a buffer and into a file.
 * @return	Number of bytes properly written, -1 in case of error.
 */
int writeFile(int fileDescriptor, void *buffer, int numBytes)
{
	return -1;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{
	return -1;
}

/*
 * @brief	Creates a new directory provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the directory already exists, -2 in case of error.
 */
int mkDir(char *path)
{
	return -2;
}

/*
 * @brief	Deletes a directory, provided it exists in the file system.
 * @return	0 if success, -1 if the directory does not exist, -2 in case of error..
 */
int rmDir(char *path)
{
	return -2;
}

/*
 * @brief	Lists the content of a directory and stores the inodes and names in arrays.
 * @return	The number of items in the directory, -1 if the directory does not exist, -2 in case of error..
 */
int lsDir(char *path, int inodesDir[10], char namesDir[10][33])
{
	return -2;
}




/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

/*Implementation adicional */

int ialloc(void){
    for(int i = 0; i<ELEM_IN_FILE_SYS;i++){
    	if(iNodos[i].type == EMPTY){
    		return i;
    	}
    }

    return -1;
}

int namei ( char *fileName ){
	
	for (int i=0; i<ELEM_IN_FILE_SYS; i++){
		if (strcmp(iNodos[i].name, fileName)==0)
		return i;
	}
	
	return -1;
}










void tokenaizor (char * cadena){
	for(int i = 0; i< DEEPTH_MAX + 1;i++){
		carpetas[i]= NULL;
	}

	for(int i = 0; carpetas[i-1] != NULL && i < DEEPTH_MAX + 1; i++){
        carpetas[i]= strtok(cadena, "/");
        ultim = i;
	}
    
}

/*bool check (void){
	for(int i = 0; i< DEEPTH_MAX && carpetas[i+1] != NULL;i++){
		if(namei(carpetas[i])== -1){
			return false;
		}
	}
	return true;
}*/