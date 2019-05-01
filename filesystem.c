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
#include <stdlib.h>


Superblock superblock[1]; // SuperBloque del sistema de ficheros (Para facilitar la funcion memcpy evitando asi casteos entre estructuras y char *)
INodo iNodos[ELEM_IN_FILE_SYS]; //Array donde se almacenan los i-nodos




char fullpath[4][32];


/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */
int mkFS(long deviceSize)
{  
	 //NF9: El sistema de ficheros será usado en discos de 50 KiB a 10 MiB
	if (deviceSize < FILE_SYS_MIN_SIZE || deviceSize > FILE_SYS_MAX_SIZE) return -1;
	superblock[0].size =deviceSize/BLOCK_SIZE; // almacenamos el tamaño del disco en numero de bloques.
	

	// Asignamos memoria para los inodos y los inicializamos a la variable EMPTY en lugar de usar i_map.
	for(int i=1; i<ELEM_IN_FILE_SYS; i++){
		memset(&(iNodos[i]), 0, sizeof(INodo) );
		iNodos[i].type = EMPTY;
		iNodos[i].sub_lib = 0;
	}

    // Creamos el directorio raiz "/".
	iNodos[0].type = TYPE_DIRECTORY;
	iNodos[0].path = "/";

    // "Salvamos" los metadatos al disco
	if(unmountFS()== -1) return -1;

	return 0;
}

/*
 * @brief 	Mounts a file system in the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int mountFS(void)
{
	char buffer[BLOCK_SIZE];
    if(bread(DEVICE_IMAGE, 0, buffer ) == -1)return -1; 
    memcpy(superblock,buffer, sizeof(superblock));
    memcpy(iNodos, &buffer[sizeof(superblock)], sizeof(iNodos));

	return 0;
}

/*
 * @brief 	Unmounts the file system from the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int unmountFS(void)
{
    char buffer[BLOCK_SIZE];
    //((char *)&(superblock))
	memcpy(buffer, superblock, sizeof(superblock));
	memcpy(&buffer[sizeof(superblock)], iNodos, sizeof(iNodos));
	if(bwrite(DEVICE_IMAGE, 0, buffer) ==-1)return -1;
	return 0;
	
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
    if(namei(path)!=-1)return -2;

    // Obtenemos si existe un nodo libre 
    int nodohijo = ialloc();
    if(nodohijo == -1){
    	return -2;
    } 
    int nodopadre = namei(getDirect(path));
    
    if(iNodos[nodopadre].sub_lib>9 || iNodos[nodopadre].type == TYPE_FILE || iNodos[nodopadre].type == EMPTY){
     	return -2;
     }else {
     	/*printf("%d%s\n",padre,fullpath[padre+1]);*/
       iNodos[nodohijo].path = path;
       iNodos[nodohijo].name = getName(path);
       iNodos[nodohijo].type = TYPE_FILE;
       iNodos[nodohijo].opened = CLOSE;
       iNodos[nodohijo].bytesUsed = 0;
       iNodos[nodohijo].puntero = 0;
       iNodos[nodohijo].bloque = 0;
       // ASIGNAR UN BLOQUE DEL BITMAP 
       iNodos[nodopadre].subdirectorios[iNodos[nodopadre].sub_lib]= nodohijo;
       iNodos[nodopadre].sub_lib++;
       
     }
     
	return 0;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *path)
{
	int nodo = namei(path);
	if(nodo==-1){
		return -1;
	}
	int padre = namei(getDirect(path));
    rmChild(padre,nodo);
    if(iNodos[nodo].bloque !=0){bitmap_setbit(superblock[0].bitmap,iNodos[nodo].bloque,0);}
	ifree(nodo);

	return 0;
}

/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *path)
{
     int nodo = namei(path);
     if(nodo== -1){
     	return -1;
     }
     if(iNodos[nodo].type != TYPE_FILE || iNodos[nodo].opened == OPEN){
     	return -2;
     }
     iNodos[nodo].opened = OPEN;
     if(iNodos[nodo].bloque == 0){
     	int numbloque = iallocBit();
     	if(numbloque == -1) return -2;
     	iNodos[nodo].bloque = numbloque;
     }

	return nodo;
}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{
	int nodo = fileDescriptor;
    if(nodo <=0){
    	return -1;
    }
    if(iNodos[nodo].type != TYPE_FILE || iNodos[nodo].opened == CLOSE){
     	return -2;
    }
     iNodos[nodo].opened = CLOSE;


	return 0;
}

/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{  
	int nodo = fileDescriptor;
	if(iNodos[nodo].type != TYPE_FILE || numBytes <=0 || iNodos[nodo].opened != OPEN)return -1;

	if(iNodos[nodo].puntero+numBytes > iNodos[nodo].bytesUsed) numBytes = iNodos[nodo].bytesUsed - iNodos[nodo].puntero;
	if(numBytes == 0) return 0;
    
    char lectura[BLOCK_SIZE];
    bread(DEVICE_IMAGE,iNodos[nodo].bloque,lectura);
    memmove(buffer,lectura + iNodos[nodo].puntero,numBytes);
    return numBytes;

}

/*
 * @brief	Writes a number of bytes from a buffer and into a file.
 * @return	Number of bytes properly written, -1 in case of error.
 */
int writeFile(int fileDescriptor, void *buffer, int numBytes)
{ 
	int nodo = fileDescriptor;
	if(iNodos[nodo].type != TYPE_FILE || numBytes <=0 || (iNodos[nodo].bytesUsed + numBytes)> FILE_MAX_SIZE || iNodos[nodo].opened != OPEN)return -1;

	if(iNodos[nodo].puntero+numBytes > BLOCK_SIZE) numBytes = BLOCK_SIZE - iNodos[nodo].puntero;
	if(numBytes == 0) return 0;

	char escritura[BLOCK_SIZE];
	bread(DEVICE_IMAGE,iNodos[nodo].bloque,escritura);
    memmove(escritura + iNodos[nodo].puntero,buffer,numBytes);
    bwrite(DEVICE_IMAGE,iNodos[nodo].bloque,escritura);

    iNodos[nodo].bytesUsed+= numBytes;
    iNodos[nodo].puntero += numBytes;
    unmountFS();
    return numBytes;

	return 0;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{
	int nodo = fileDescriptor;
	if(nodo<=0)return -1;
	if(iNodos[nodo].opened != OPEN)return -1;
	long nuevapos=  iNodos[nodo].puntero + offset;

	switch(whence){
		case FS_SEEK_CUR:
           if(nuevapos > 0 && nuevapos < iNodos[nodo].bytesUsed) iNodos[nodo].puntero = nuevapos;
        break;
		case FS_SEEK_BEGIN:
         iNodos[nodo].puntero = 0;
		break;
		case FS_SEEK_END:
		  iNodos[nodo].puntero = iNodos[nodo].bytesUsed;
		break;

	}
	return 0;
}

/*
 * @brief	Creates a new directory provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the directory already exists, -2 in case of error.
 */
int mkDir(char *path)
{  
    if (strlen(path)>FULLPATH_MAX_FILE){
    	return -2;
    }
    if(namei(path)!=-1)return -2;
    int nodohijo = ialloc();
    if(nodohijo == -1){
    	return -2;
    }
    
    int nodopadre = namei(getDirect(path));
    
     
    if(iNodos[nodopadre].sub_lib>9 || iNodos[nodopadre].type == TYPE_FILE || iNodos[nodopadre].type == EMPTY){
     	return -2;
     }else {
     	/*printf("%d%s\n",padre,fullpath[padre+1]);*/
       iNodos[nodohijo].path = path;
       iNodos[nodohijo].name = getName(path);
       iNodos[nodohijo].type = TYPE_DIRECTORY;
       iNodos[nodohijo].sub_lib = 0;
       iNodos[nodopadre].subdirectorios[iNodos[nodopadre].sub_lib]= nodohijo;
       iNodos[nodopadre].sub_lib++;
       
     }

	return 0;
}

/*
 * @brief	Deletes a directory, provided it exists in the file system
 * @return	0 if success, -1 if the directory does not exist, -2 in case of error..
 */
int rmDir(char *path)
{
	int nodo = namei(path);
	if(nodo==-1){
		return -1;
	}
	int padre = namei(getDirect(path));
    if(iNodos[nodo].sub_lib!=0){
    	rmAllChilds(nodo);
    }
    rmChild(padre,nodo);
	ifree(nodo);

	return 0;
}

/*
 * @brief	Lists the content of a directory and stores the inodes and names in arrays.
 * @return	The number of items in the directory, -1 if the directory does not exist, -2 in case of error..
 */
int lsDir(char *path, int inodesDir[10], char namesDir[10][33])
{

	int nodo = namei(path);

	if(nodo == -1){
		return -1;
	}
    
    for(int i = 0; i<iNodos[nodo].sub_lib; i++){
    	inodesDir[i]=iNodos[nodo].subdirectorios[i];
    	strcpy(namesDir[i],iNodos[iNodos[nodo].subdirectorios[i]].path);
    	
    }

	return iNodos[nodo].sub_lib;
}




/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/

/*Implementation adicional */

// Encuantra y devulve el primer inodo libre.
int ialloc(void)
{
    for(int i = 0; i<ELEM_IN_FILE_SYS;i++){
    	if(iNodos[i].type == EMPTY){
    		return i;
    	}
    }

    return -1;
}

int iallocBit(void){
	for(int i = 1; i<= superblock[0].size;i++){
		if(bitmap_getbit(superblock[0].bitmap,i)==0){
			bitmap_setbit(superblock[0].bitmap,i,1);
			return i;
		}
	}
	return -1;
}

// Devuelve el entero que representa al inodo con el path indicado 
int namei ( char *fileName )
{
	
	for (int i=0; i<ELEM_IN_FILE_SYS; i++){
		if (iNodos[i].type != EMPTY && strcmp(iNodos[i].path, fileName)==0 )
		return i;
	}
	
	return -1;
}


// Vacia un inodo estableciendo sus atributos a default
int ifree ( int inodo_id)
{
     iNodos[inodo_id].path = NULL;
     iNodos[inodo_id].type = EMPTY;
     iNodos[inodo_id].opened = CLOSE;
     iNodos[inodo_id].bytesUsed = 0;
     iNodos[inodo_id].sub_lib = 0;

     for(int i=0; i< ELEM_IN_DIRECTORY; i++){
     	iNodos[inodo_id].subdirectorios[i]=-1;
     }

    return 0;

     
}








int tokenizador (char * cadena, int modo)
{
    
    int profundidad = 0;
    char aux [132];
    strcpy(aux,cadena);
    char *p = strtok (aux, "/");
    
    while (p != NULL)
    {
    	strcpy(fullpath[profundidad],p);
    	profundidad++;
        p = strtok (NULL, "/");
    }
    
    if(profundidad > DEEPTH_MAX + 1){
    	return -2;
    }

    return profundidad-1;    
}

// Imprime el sistema de ficheros 
void printFS()
{
	 for(int i = 0; i<ELEM_IN_FILE_SYS;i++){
    	if(iNodos[i].type != EMPTY){
    		printf("%d-INODO: %s , tipo %d,",i,iNodos[i].path,iNodos[i].type );
    		printf(" NodosHijo:");
    		for(int j = 0; j< iNodos[i].sub_lib;j++) printf("%d,",iNodos[i].subdirectorios[j]);
    		printf("\n");
    	}
    }
    printf("\n");
}



// devuelve el path del padre del path indicado 
char * getDirect(char * path)
{
	char *dir= malloc(sizeof(char)*FULLPATH_MAX_DIRECTORY);
	char *p;

	p= strrchr(path,'/');
	int pos = (int)(p-path);
	strncpy(dir,path,pos);

	if(strcmp(dir,"")==0)dir = "/";
	return dir;
}

char * getName(char * path){
	char *dir= malloc(sizeof(char)*FULLPATH_MAX_DIRECTORY);
	char *p;

	p= strrchr(path,'/');
	int pos = (int)(p-path);
	int tam = strlen(path);
	strncpy(dir,p+1,tam-pos-1);
	

	
	return dir;
}

// elimina un nodo y la conexion con el nodo padre 
void rmChild(int padre, int hijo){
    int i;
	for(i=0; i<iNodos[padre].sub_lib;i++){
        if(iNodos[padre].subdirectorios[i]== hijo){
        	iNodos[padre].subdirectorios[i]=-1;
        	break;
        }
	}
	for(int j = i; j<iNodos[padre].sub_lib-1;j++){
       iNodos[padre].subdirectorios[j]=iNodos[padre].subdirectorios[j+1];
	}
	iNodos[padre].sub_lib--;
}


// elimina de manera recursiva todos los nodos contenidos en un nodo
void rmAllChilds(int nodo){
	for(int i=0; i<iNodos[nodo].sub_lib;i++){
		if(iNodos[iNodos[nodo].subdirectorios[i]].sub_lib!=0){
			rmAllChilds(iNodos[nodo].subdirectorios[i]);
		}
		ifree(iNodos[nodo].subdirectorios[i]);
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