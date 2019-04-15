/*
 * OPERATING SYSTEMS DESING - 16/17
 *
 * @file 	metadata.h
 * @brief 	Definition of the structures and data types of the file system.
 * @date	01/03/2017
 */


#define ELEM_IN_FILE_SYS 40 // NF1
#define ELEM_IN_DIRECTORY 10 //NF2
#define NAME_MAX_SIZE 32 //NF3
#define FULLPATH_MAX_DIRECTORY 99 // NF4
#define FULLPATH_MAX_FILE 132 // NF4 
#define DEEPTH_MAX 3 // NF5;
#define FILE_MAX_SIZE 2048 // NF6: El tamaño máximo de un fichero será de un bloque.
#define BLOCK_SIZE 2048 // NF7: El tamaño de bloque del sistema será de 2048 bytes
#define FILE_SYS_MAX_SIZE 102400 //NF9 :El sistema de ficheros será usado en discos de 50 KiB a 10 MiB.
#define FILE_SYS_MIN_SIZE 51200 // NF9
#define TYPE_DIRECTORY 1
#define TYPE_FILE 0
#define OPEN 1
#define CLOSE 0



typedef struct INodo {
  char *name; //Nombre del fichero/directorio 
  uint8_t type; // Indica si es fichero/directorio
  uint8_t directBlock; //Bloque asociado
  uint8_t opened; //Indica si está abierto o no
  uint8_t inodes[ELEM_IN_DIRECTORY]; // Inodos contenidos if type = TYPE_DIRECTORY
  uint16_t bytesUsed; //Bytes usados por el fichero
  uint16_t pointer; //Puntero del fichero
  //uint16_t Check; 
} INodo;

typedef struct Superblock {
  long size; //Tamaño del disco
  uint64_t INodoMap; //Mpa de indos
  struct INodo iNodos[ELEM_IN_FILE_SYS]; //Array donde se almacenan los i-nodos
  uint8_t firstFreeInode;
  //uint16_t Check; 
} Superblock;

#define bitmap_getbit(bitmap_, i_) (bitmap_[i_ >> 3] & (1 << (i_ & 0x07)))
static inline void bitmap_setbit(char *bitmap_, int i_, int val_) {
  if (val_)
    bitmap_[(i_ >> 3)] |= (1 << (i_ & 0x07));
  else
    bitmap_[(i_ >> 3)] &= ~(1 << (i_ & 0x07));
}
