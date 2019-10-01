#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  // para mkdir
#include <sys/stat.h>
#include <fcntl.h>  // para open
#include <unistd.h> // para rmkdir y creat
#include <dirent.h> // para directorios

#include "manejodeArchivos.h"

#define MAX 100


int crearJugadores(){
  int i, ret;
  char *buffer;

  buffer = (char*)malloc(MAX*sizeof(char));
  for (i=0;i<4;i++){
    snprintf(buffer,10, "Jugador %d", i+1);
    ret = mkdir(buffer,S_IRWXU);
  }
  free(buffer);
  return ret;
}

int crearMazo(){
  int ret,i,num;
  char *buffer, *color, **colores;

  buffer = (char*)malloc(MAX*sizeof(char));


  colores = (char**)malloc(4*sizeof(char*));
  color = (char*)malloc(4*sizeof(char));
  strcpy(color,"rojo");
  colores[0] = color;
  color = (char*)malloc(4*sizeof(char));
  strcpy(color,"azul");
  colores[1] = color;
  color = (char*)malloc(5*sizeof(char));
  strcpy(color,"verde");
  colores[2] = color;
  color = (char*)malloc(8*sizeof(char));
  strcpy(color,"amarillo");
  colores[3] = color;

  ret= mkdir("Mazo",S_IRWXU);

  // creamos las cartas de color rojo, azul, verde y amarillo.
  for(i=0;i<4;i++){
    //primero los 0'
    snprintf(buffer,12 + sizeof(colores[i]), "Mazo/0 %s.txt", colores[i]);
    creat(buffer,S_IRWXU);
    //ahora los numeros
    for(num=1; num<10;num++){
      snprintf(buffer,14 + sizeof(colores[i]), "Mazo/%d %s_1.txt",num,colores[i]);
      creat(buffer,S_IRWXU);
      snprintf(buffer,14 + sizeof(colores[i]), "Mazo/%d %s_2.txt",num,colores[i]);
      creat(buffer,S_IRWXU);
    }
    //luego las cartas especiales:
    //+2
    snprintf(buffer,15 + sizeof(colores[i]), "Mazo/+2 %s_1.txt",colores[i]);
    creat(buffer,S_IRWXU);
    snprintf(buffer,15 + sizeof(colores[i]), "Mazo/+2 %s_2.txt",colores[i]);
    creat(buffer,S_IRWXU);
    //reverse
    snprintf(buffer,20 + sizeof(colores[i]), "Mazo/reverse %s_1.txt",colores[i]);
    creat(buffer,S_IRWXU);
    snprintf(buffer,20 + sizeof(colores[i]), "Mazo/reverse %s_2.txt",colores[i]);
    creat(buffer,S_IRWXU);
    //jump
    snprintf(buffer,17 + sizeof(colores[i]), "Mazo/jump %s_1.txt",colores[i]);
    creat(buffer,S_IRWXU);
    snprintf(buffer,17 + sizeof(colores[i]), "Mazo/jump %s_2.txt",colores[i]);
    creat(buffer,S_IRWXU);

  }
  // despues las cartas de negro.
  for(i=0;i<4;i++){
    //eleccion  de colores
    snprintf(buffer,36, "Mazo/eleccion de colores negra_%d.txt",i+1);
    creat(buffer,S_IRWXU);
    //+4
    snprintf(buffer,19, "Mazo/+4 negra_%d.txt",i+1);
    creat(buffer,S_IRWXU);
  }

  //liberamos memoria pedida
  free(buffer);
  for(i=0;i<4;i++){
    free(colores[i]);
  }
  free(colores);
  return ret;
}
int imprimirMazo(){
  DIR *dir;
  struct dirent *ent;
  // leemos el direcorio de mazo
  dir = opendir ("Mazo/");
  //verificamos que no exista un error
  if (dir == NULL){
    printf("No puedo abrir el directorio");
    exit(EXIT_FAILURE);
  }
  //leemos archivos en la carpeta Mazo
  while ((ent = readdir (dir)) != NULL){
    // nos devolvera "." y ".."
    if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
      //imprimimos el archivo
      printf ("%s \n", ent->d_name);
    }
  }
  closedir (dir);
  return EXIT_SUCCESS;
}

