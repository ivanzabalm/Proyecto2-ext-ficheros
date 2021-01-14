#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

int num_car;
int inodos_count;
char arg1[LONGITUD_COMANDO];
char arg2[LONGITUD_COMANDO];

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Identificador(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *arg);
void Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
void Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
             EXT_DATOS *memdatos);
void Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,FILE *fich);
void  Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos,FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int main(){
    
    char *comando[LONGITUD_COMANDO];
    char *orden = NULL;
     
    //int i,j;
    //unsigned long int m;
    EXT_SIMPLE_SUPERBLOCK ext_superblock;
    EXT_BYTE_MAPS ext_bytemaps;
    EXT_BLQ_INODOS ext_blq_inodos;
    EXT_ENTRADA_DIR directorio[SIZE_BLOQUE];
    EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
    EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
    //int entradadir;
    //int grabardatos;
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
     
    int i = 0;
    while(i<MAX_INODOS){
        if(ext_blq_inodos.blq_inodos[i].size_fichero != 0){
            
            inodos_count++;
        }
        
        i++;
    }
    
    printf("\n%d\n",inodos_count);
    
    for (;;){
        do{
        printf (">> ");
        fflush(stdin);
        memset(comando,' ', LONGITUD_COMANDO);    // Rellenamos la cadena de caracteres vacios para eliminar su contenido basura
        fgets(comando, LONGITUD_COMANDO, stdin);
         
        } while(ComprobarComando(comando,orden) !=0);
        
        // Tamaño exacto de la cadena segun la orden (para realizar efectivamente la comparación con strcmp)
         
        orden = (char *) malloc(num_car*sizeof(char));
        strncpy(orden, comando, num_car);
         
        // Selector de ordenes
        
        if(strcmp(orden,"dir")==0){
            Directorio(directorio, &ext_blq_inodos);
        }
        
        if(strcmp(orden,"info")==0){
            LeeSuperBloque(&ext_superblock);
        }
        
        if(strcmp(orden,"bytemaps")==0){
            Printbytemaps(&ext_bytemaps);
        }
        
        if(strcmp(orden,"rename")==0){
             
            Renombrar(directorio,&ext_blq_inodos);
            
        }
        
        if(strcmp(orden,"imprimir")==0){
            
            Imprimir(directorio, &ext_blq_inodos, memdatos);
            
        }
        
        if(strcmp(orden,"remove")==0){
             
            Borrar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, fent);
            
        }
        
        if(strcmp(orden,"copy")==0){
            printf("funcion copy\n");
             
            Copiar(directorio, &ext_blq_inodos, &ext_bytemaps, &ext_superblock, memdatos,fent);
            
        }
        
        if(strcmp(orden,"salir")==0){
            exit(-1);
        }
         
        free(orden);
    }
}

int ComprobarComando(char *strcomando, char *orden){
    
    int i = 0;
    int x = 0;
    int pos = 0;
    int space = 0;
    int boolArg = 1;
    
    for(int s = 0; s<100; s++){
        arg1[s] = '\0';
        arg2[s] = '\0';
    }
    
    
    num_car = 0;
    
    // Contador de caracteres de la orden (primera palabra)
    
    while(strcomando[i] != ' '){
        if((strcomando[i] != '\n') && (strcomando[i] != '\0')){
            num_car++;
        }else if(strcomando[i] == '\n'){
            boolArg = 0;}
        
        i++;
    }
    // Argumentos
    
    printf("\n");
    if(boolArg == 1){
        
        while(strcomando[x] != '\n'){
        
            if(strcomando[x] == ' '){space++;pos = 0;}else{
        
            
            switch(space){
                
                case 1: arg1[pos] = strcomando[x]; pos++;break;
                case 2: arg2[pos] = strcomando[x]; pos++;break;
                
            }
            }
        
            x++;
        }
    }
    
    // Comprobacion de comando (existencia de la orden (primera palabra) y el tamaño de los strings concuerden)
    
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
    
    printf("Bloque %d Bytes\n", psup->s_block_size);
    printf("inodos particion = %d\n", psup->s_inodes_count);
    printf("inodos libres = %d\n", psup->s_free_inodes_count);
    printf("Bloques particion %d\n", psup->s_blocks_count);
    printf("Bloques libres = %d\n", psup->s_free_blocks_count);
    printf("Primer bloque de datos = %d\n", psup->s_first_data_block);
    
}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
    
    printf("Inodos :");
    
    for(int i=0; i<MAX_INODOS; i++){
        printf("%u ",ext_bytemaps->bmap_inodos[i]);
    }
    
    printf("\nBloques [0-25] :");
    
    for(int i=0; i<MAX_BLOQUES_PARTICION; i++){
        printf("%u ",ext_bytemaps->bmap_bloques[i]);
    }
    
    printf("\n");
}

void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){
        
    // Recorrer los ficheros existentes (exceptuando la entrada especial del directorio raiz),
    // imprimiendo su nombre, tamaño, inodo y bloques que ocupa
    
    for(int i=1; i<inodos_count+1; i++){
        printf("%s ",directorio[i].dir_nfich);
        printf("    tamaño:%d", inodos->blq_inodos[directorio[i].dir_inodo].size_fichero);
        printf("    inodo:%d",directorio[i].dir_inodo);
        printf(" bloques: ");
        
        for(int j=0; j<MAX_NUMS_BLOQUE_INODO; j++){
            if( inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]!= 65535){
                printf("%d ",inodos->blq_inodos[directorio[i].dir_inodo].i_nbloque[j]);
            }
        }
        
        printf("\n");
    }
    printf("\n");

}

int Identificador(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,char *arg){

    //usamos esta funcion para saber si el argumento introducido es el nombre de algun directorio
    //de ser asi envia su direccion, por el contrario envia -1
    int boolean = 0;
    int pos = -1;
    int let = 0;
    int char1 = 0;
    int char2 = 0;
    
    for(int i=0; i<inodos_count+1; i++){
        
        boolean = 1;

        for(int x = 0; directorio[i].dir_nfich[x] != '\0'; x++){
                        
            char1 = directorio[i].dir_nfich[x];
            char2 = arg[x];
            
            char1 = char1 - char2;
            
            if(char1 == 0){
                let = i;
                
            }else{boolean = 0;}

            
        }
        if(boolean == 1){pos = let;}
    }
    
    return pos;
}
void Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){
    
    int pos = 0;
    int pos2 = 0;

    if(arg1[0] != '\0' && arg2[0] != '\0'){
                    
        pos = Identificador(directorio,inodos,arg1);
        
        pos2 = Identificador(directorio,inodos,arg2);
        
        //comprobamos que exista dichos ficheros
        if(pos == -1){
            printf("No se ha encontrado ningún fichero con ese nombre.\n");
        }
        if(pos2 != -1){
            printf("El nuevo nombre ya pertenece a un fichero.\n");
        }
        
        if(pos != -1 && pos2 == -1){
            
            printf("Rename ");
            
            for(int i = 0; directorio[pos].dir_nfich[i] != '\0'; i++){
                
                printf("%c",directorio[pos].dir_nfich[i]);
                
            }
            
            printf(" a ");
            
            for(int i = 0; directorio[pos].dir_nfich[i] != '\0' || arg2[i] != '\0'; i++){
                
                //sustituimos el nombre del fichero indicado por el argumento introducido
                directorio[pos].dir_nfich[i] = arg2[i];
                printf("%c",directorio[pos].dir_nfich[i]);
                
            }
            
            printf("\n");
            
        }
        
    }else{
        
        printf("Sintáxis del comando errónea, rename nombre_fichero nombre_nuevo.\n");
        
    }
    printf("\n");
    
}

void Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,EXT_DATOS *memdatos){
    
    
    int dir = Identificador(directorio, inodos, arg1);
    
    //comprobamos que exista dicho fichero
    if(dir == -1){printf("Error, fichero no encontrado.");}else{
    
        for(int j=0; j<MAX_NUMS_BLOQUE_INODO; j++){
            
            //recorremos cada dato de cada directorio mientras exista para ir imprimiendolos
            int bloq = inodos->blq_inodos[directorio[dir].dir_inodo].i_nbloque[j];
            bloq = bloq -4;
            
            if(bloq != 65531){
                for(int i = 0; memdatos[bloq].dato[i] != '\0'; i++){
                    printf("%c",memdatos[bloq].dato[i]);
                }
            }

        }

    }
    printf("\n");
}

void Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,FILE *fich){
    
    int dir = Identificador(directorio, inodos, arg1);
        
    //comprobamos que exista dicho fichero
    if(dir ==-1){
        
        printf("\nno existe dicho fichero.\n");
        
    }else{
                
        //si existe recorremos todos los directorios desde la direccion de dicho fichero sustituyendolos por sl siguiente
        //de esta forma eliminamos el fichero indicado
        for(int i = dir; i <inodos_count+1; i++){
            
            directorio[i] = directorio[i+1];
            ext_bytemaps->bmap_inodos[i] = ext_bytemaps->bmap_inodos[i+1];
            ext_bytemaps->bmap_bloques[i] = ext_bytemaps->bmap_bloques[i+1];
            
        }
        inodos_count--;
    }
}

void  Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
             EXT_DATOS *memdatos,FILE *fich){
    
    int bloq_an = 0;
    int origen = Identificador(directorio, inodos, arg1);
    int destino = Identificador(directorio, inodos, arg2);
    int boolean = 1;
    
    if(origen == -1){
        boolean = 0;
        printf("\nEl fichero origen no existe.\n");
    }
    if(destino != -1){
        boolean = 0;
        printf("\nEl fichero destino ya existe.\n");
    }
    
    if(boolean == 1){
        
        //buscamos los inodos y bloques libres para crear nuestro nuevo fichero
        inodos_count++;
        destino = inodos_count;
        for(int i = 0; i<17; i++){
            directorio[inodos_count].dir_nfich[i] = arg2[i];
        }
        
        for(int i = 0; i<24 ; i++){
            
            if(inodos->blq_inodos[i].size_fichero == 0){
                
                directorio[destino].dir_inodo = i;
                inodos->blq_inodos[i].size_fichero = inodos->blq_inodos[origen].size_fichero;
                i=25;
                
            }

        }
        ext_bytemaps->bmap_inodos[destino] = ext_bytemaps->bmap_inodos[origen];
        ext_bytemaps->bmap_bloques[destino] = ext_bytemaps->bmap_bloques[origen];
        inodos->blq_inodos[directorio[destino].dir_inodo].size_fichero = inodos->blq_inodos[directorio[origen].dir_inodo].size_fichero;
        
        for(int j=0; j<MAX_NUMS_BLOQUE_INODO; j++){
            
            //recorremos cada dato de cada directorio mientras exista para ir imprimiendolos
            int bloq = inodos->blq_inodos[directorio[origen].dir_inodo].i_nbloque[j];
            bloq = bloq -4;
            
            
            if(bloq != 65531){
                
                
                for(int i = 0; memdatos[i].dato[0] != '\0'; i++){
                    bloq_an = i;
                }bloq_an++;
                 
                int boolean2 = 0;
                for(int x=0; x<MAX_BLOQUES_DATOS; x++){
                    
                    if(memdatos[x].dato[0] == 0 && boolean2 == 0){
                        //buscamos un bloque libre para rellenarlo con los datos copiados
                        memdatos[x] = memdatos[bloq];
                        inodos->blq_inodos[directorio[destino].dir_inodo].i_nbloque[j] = x;
                        boolean2 = 1;
                    }
                    
                }
        
            }
             
        }
            
    }
    
}
