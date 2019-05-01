/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	test.c
 * @brief 	Implementation of the client test routines.
 * @date	01/03/2017
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h> 
#include "include/filesystem.h"
#include "filesystem.c"

// Color definitions for asserts
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"

#define N_BLOCKS 25					  // Number of blocks in the device
#define DEV_SIZE N_BLOCKS *BLOCK_SIZE // Device size, in bytes

int main()
{    




















	/// PRUEBA CORRECTA APERTURA, ESCRITURA, LECTURA Y CERRADO DE FICHEROS.
     /*char escribir[]= {"La practica dos es una pracitca en la que construimos un sistema ficheros..."};
     char leido [BLOCK_SIZE];
   
     mkFS(DEV_SIZE);

     
     mkDir("/universidad");
     mkDir("/universidad/dso");
     if(createFile("/universidad/dso/practica.txt")!=0)printf("Error create \n");
     printFS();
     
     int y = openFile("/universidad/dso/practica.txt");
     if(y<=0)printf("Error openFile\n");
    
     int bytesescritos = writeFile(y,escribir,sizeof(escribir));
     printf("Los bytes escritos han sido: %d\n", bytesescritos);

     if(lseekFile(y,0,FS_SEEK_BEGIN)== -1) printf("Error la asignacion del punteroi al inicio\n");

     int bytesleidos = readFile(y,leido,sizeof(escribir));
     printf("Los bytes escritos han sido: %d\n",bytesleidos);
     printf("-%s\n",leido);

     if(closeFile(y)<0)printf("Error cerrando fichero\n");

     if(removeFile("/universidad/dso/practica.txt")== -1) printf("Error al eliminar fichero\n");

     printFS();*/








     /// Prueba de que monta y desmonta bien el sistema 
     /*printFS();
     mkDir("/prueba");
     mkDir("/prueba/error");
     mkDir("/prueba/error1");
     mkDir("/prueba/error2");
     mkDir("/prueba/error3");
     mkDir("/prueba/error3/uno");
     mkDir("/prueba/error3/dos");
     mkDir("/prueba/error3/tres");
     printFS();
     if(unmountFS() == -1) printf("ERROR en umount\n");
     rmDir("/prueba");
     printFS();
    if(mountFS() == -1) printf("ERROR en umount\n");
     printFS();*/


     /*mkDir("/pruebas");
     createFile("/pruebas/fichero.txt");
     createFile("/otro.txt");

     printFS();*/



   /// PRUEBA DEL SISTEMA DE CARPETAS ////
     /*int x = mkDir("/prueba");
     if(x==-2)printf("ERROR\n");
     mkDir("/prueba/error");
     mkDir("/prueba/error1");
     mkDir("/prueba/error2");
     mkDir("/prueba/error3");
     printFS();
     mkDir("/prueba/error1/uno");
     mkDir("/prueba/error1/dos");
     mkDir("/prueba/error1/tres");
     printFS();
     rmDir("/prueba/error1");
     printFS();
     mkDir("/samba");
     printFS();
     mkDir("/prueba/error3/1");
     mkDir("/prueba/error3/2");
     mkDir("/prueba/error3/3");
     mkDir("/prueba/error3/4");
     printFS();
     rmDir("/prueba/error3");
     printFS();
     rmDir("/prueba");
     printFS();*/









	/*int ret;

	///////

	ret = mkFS(DEV_SIZE);
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mkFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	ret = mountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST mountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	ret = createFile("/test.txt");
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST createFile ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	ret = unmountFS();
	if (ret != 0)
	{
		fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_RED, "FAILED\n", ANSI_COLOR_RESET);
		return -1;
	}
	fprintf(stdout, "%s%s%s%s%s", ANSI_COLOR_BLUE, "TEST unmountFS ", ANSI_COLOR_GREEN, "SUCCESS\n", ANSI_COLOR_RESET);

	///////

	return 0;*/
}
