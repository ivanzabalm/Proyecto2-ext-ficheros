#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

int num_car;

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombreantiguo, char *nombrenuevo);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int main()
{
	 char *comando[LONGITUD_COMANDO];
	 char *orden;
	 char *argumento1[LONGITUD_COMANDO];
	 char *argumento2[LONGITUD_COMANDO];
	 
	 int i,j;
	 unsigned long int m;
     EXT_SIMPLE_SUPERBLOCK ext_superblock;
     EXT_BYTE_MAPS ext_bytemaps;
     EXT_BLQ_INODOS ext_blq_inodos;
     EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
     EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
     EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
     int entradadir;
     int grabardatos;
     FILE *fent;
     
     // Lectura del fichero completo de una sola vez
	 
     // ...
     
     fent = fopen("particion.bin","r+b");
     fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    
     
     memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
     memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
     memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
     
     // Bucle de tratamiento de comandos
	 
     for (;;){
		 do {
		 printf (">> ");
		 fflush(stdin);
		 memset(comando,' ', LONGITUD_COMANDO); // Rellenamos la cadena de caracteres vacios para eliminar contenido basura
		 fgets(comando, LONGITUD_COMANDO, stdin);
		 
		 } while(ComprobarComando(comando,orden,argumento1,argumento2) !=0);	 		 
	      
		 // Resize de la cadena orden (para realizar efectivamente la comparación con strcmp)
		 
		 orden = (char *) malloc(num_car*sizeof(char));
		 strncpy(orden, comando, num_car);
		 
		 // Selector de ordenes
		
		 if (strcmp(orden,"dir")==0) {
			 printf("funcion dir\n");
			 
            //Directorio(&directorio,&ext_blq_inodos);
			
            }
		 if (strcmp(orden,"info")==0) {
			 printf("funcion info\n");
			 
            //LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
			
            }
		 if (strcmp(orden,"bytemaps")==0) {
			 printf("funcion bytemaps\n");
			 
            //Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
			
            }
		 if (strcmp(orden,"rename")==0) {
			 printf("funcion rename\n");
			 
            //Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo);
			
            }
		 if (strcmp(orden,"imprimir")==0) {
			 printf("funcion imprimir\n");
			
            //BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombre);
			//Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre);
			
            }
		 if (strcmp(orden,"remove")==0) {
			 printf("funcion remove\n");
			 
            //Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, char *nombre, FILE *fich);
			
            }
		 if (strcmp(orden,"copy")==0) {
			 printf("funcion copy\n");
			 
            //Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock, EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
			
            }
		 if (strcmp(orden,"salir")==0) {
            exit(-1);
            }
		 
		 // TODO	 
		 // Escritura de metadatos en comandos rename, remove, copy     
		 
         /* 
         Grabarinodosydirectorio(&directorio,&ext_blq_inodos,fent);
         GrabarByteMaps(&ext_bytemaps,fent);
         GrabarSuperBloque(&ext_superblock,fent);
         if (grabardatos)
           GrabarDatos(&memdatos,fent);
         grabardatos = 0;
		 */
		 
         //Si el comando es salir se habrán escrito todos los metadatos
         //faltan los datos y cerrar
		 /*
         if (strcmp(orden,"salir")==0){
            GrabarDatos(&memdatos,fent);
            fclose(fent);
            return 0;
         }
		 */
		 
		 free(orden);
     }
}

int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2){
	
	int i = 0;	
	
	num_car = 0;
	
	// Contador de caracteres de la orden
	
	while(strcomando[i] != ' '){
		if((strcomando[i] != '\n') && (strcomando[i] != '\0')){
			num_car++;
		}
		
		i++;
	}
	
	// Comprobacion de comando (existencia de la orden y el tamaño de los strings concuerden) 
	
	if((strncmp(strcomando,"dir", 3) == 0) && num_car == 3){	
		return 0;
	}
	
	else if(((strncmp(strcomando,"info", 4) == 0) || (strncmp(strcomando,"copy", 4) == 0)) && num_car == 4){
		return 0;
	}
	
	else if((strncmp(strcomando,"salir", 5) == 0) && num_car == 5){
		return 0;
	}
	
	else if(((strncmp(strcomando,"rename", 6) == 0) || (strncmp(strcomando,"remove", 6) == 0)) && num_car == 6){
		return 0;
	}
	
	else if(((strncmp(strcomando,"bytemaps", 8) == 0) || (strncmp(strcomando,"imprimir", 8) == 0)) && num_car == 8){
		return 0;
	}
	
	else{
		printf("ERROR: Comando ilegal [bytemaps,copy,dir,info,imprimir,rename,remove,salir]\n");
		return 1;
	}
	
	// TODO 
	// Argumentos
	
}

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
	
	// TODO
	
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
	
	// TODO
	
}
