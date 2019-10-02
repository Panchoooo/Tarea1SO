#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  // para mkdir
#include <sys/stat.h>
#include <fcntl.h>  // para open
#include <unistd.h> // para rmkdir y creat
#include <dirent.h> // para directorios
#include <time.h>
#include "manejodeArchivos.h"

#define MAX 100


int crearJugadores(){
  int i, ret;
  char *buffer;

  buffer = (char*)malloc(MAX*sizeof(char));
  for (i=0;i<4;i++){
    snprintf(buffer,10, "Jugador_%d", i+1);
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
    snprintf(buffer,37, "Mazo/eleccion de colores negra_%d.txt",i+1);
    creat(buffer,S_IRWXU);
    //+4
    snprintf(buffer,20, "Mazo/+4 negra_%d.txt",i+1);
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

 int RobarCarta(char* nombre){
   DIR *dir;
   struct dirent *ent;
   int i,j,z;
   char ** lista,*buffer;
   lista = (char**)malloc(108*sizeof(char*));
   buffer = (char*)malloc(MAX*sizeof(char));

   // leemos el direcorio de mazo
   dir = opendir ("Mazo/");

   //verificamos que no exista un error
   if (dir == NULL){
     printf("No puedo abrir el directorio");
     exit(EXIT_FAILURE);
   }
   //leemos archivos en la carpeta Mazo
   i=0;
   while ((ent = readdir (dir)) != NULL){
     // nos devolvera "." y ".."
     if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
       //imprimimos el archivo
       lista[i]= (char*)malloc(sizeof(char)*strlen(ent->d_name)+1);
       snprintf(lista[i],6+(int)strlen(ent->d_name), "%s",ent->d_name);
       i++;
     }
   }
   closedir (dir);
   j = rand () % i + 0; // numero random enter 0 y 108
   snprintf(buffer,6+(int)strlen(lista[j]), "Mazo/%s",lista[j]); //  removemos la carta de la carpeta Mazo
   remove(buffer);
   snprintf(buffer,11+(int)strlen(lista[j]), "%s/%s",nombre,lista[j]); // Se la añadimos al jugador respectivo
   creat(buffer,S_IRWXU);
   // Liberamos la memoria
   for(z=0;z<i;z++){
     free(lista[z]);
   }
   free(lista);
   free(buffer);
   return EXIT_SUCCESS;
 }

void EnJuego(){
  mkdir("En_Juego",S_IRWXU);
  RobarCarta("En_Juego");
}

int RepartirMazo(){
  srand(time(NULL));
  DIR *dir;
  struct dirent *ent;
  int i,j,turno,z;
  char ** lista,*buffer;
  lista = (char**)malloc(108*sizeof(char*));
  buffer = (char*)malloc(MAX*sizeof(char));

  // leemos el direcorio de mazo
  dir = opendir ("Mazo/");

  //verificamos que no exista un error
  if (dir == NULL){
    printf("No puedo abrir el directorio");
    exit(EXIT_FAILURE);
  }
  //leemos archivos en la carpeta Mazo
  i=0;
  while ((ent = readdir (dir)) != NULL){
    // nos devolvera "." y ".."
    if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
      //imprimimos el archivo
      lista[i]= (char*)malloc(sizeof(char)*strlen(ent->d_name)+1);
      snprintf(lista[i],6+(int)strlen(ent->d_name), "%s",ent->d_name);
      i++;
    }
  }
  closedir (dir);
  // arreglo que se encargara de que el random no escoga numeros repetidos, ya que la carta ya fue utilizada.
  int *Arr;
  Arr = (int*)malloc(sizeof(int)*108);
  z=0;
  while(z<108){
    Arr[z]=0;
    z++;
  }
  for(turno = 1 ; turno<5 ;turno++){
    for(i=0;i<7;i++){
      j = rand () % 107 + 0; // numero random enter 0 y 108
      while(Arr[j]==-1){
        j=rand()% 107+0;
      }
      Arr[j]=-1;
      snprintf(buffer,6+(int)strlen(lista[j]), "Mazo/%s",lista[j]); //  removemos la carta de la carpeta Mazo
      remove(buffer);
      snprintf(buffer,11+(int)strlen(lista[j]), "Jugador_%d/%s",turno,lista[j]); // Se la añadimos al jugador respectivo
      creat(buffer,S_IRWXU);
    }
  }
  // Liberamos la memoria
  for(i=0;i<108;i++){
    free(lista[i]);
  }
  free(lista);
  free(buffer);
  return EXIT_SUCCESS;

}

int JugarCarta(char* nombre){

  DIR *dir;
  struct dirent *ent;
  int i,z;
  char ** lista,*buffer;
  lista = (char**)malloc(108*sizeof(char*));
  buffer = (char*)malloc(MAX*sizeof(char));

  dir = opendir ("En_Juego");

  //verificamos que no exista un error
  if (dir == NULL){
    printf("No puedo abrir el directorio");
    exit(EXIT_FAILURE);
  }
  // leemos la carta actual En Juego
  while ((ent = readdir (dir)) != NULL){
    // nos devolvera "." y ".."
    if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
      //borra la carta actual en "En juego"
      snprintf(buffer,10+(int)strlen(ent->d_name), "En_Juego/%s",ent->d_name);
      printf("| Ultima carta revelada: %s |\n",ent->d_name);
      remove(buffer); //en caso de poder jugar, (avance 2)
    }
  }
  closedir (dir);

  // leemos el direcorio de mazo
  snprintf(buffer,2+(int)strlen(nombre), "%s/",nombre);
  dir = opendir (buffer);

  //verificamos que no exista un error
  if (dir == NULL){
    printf("No puedo abrir el directorio");
    exit(EXIT_FAILURE);
  }
  //leemos archivos en la carpeta Mazo
  i=0;
  printf("Escoge una carta:\n");
  while ((ent = readdir (dir)) != NULL){
    // nos devolvera "." y ".."
    if ( (strcmp(ent->d_name, ".")!=0) && (strcmp(ent->d_name, "..")!=0) ){
      //imprimimos el archivo
      lista[i]= (char*)malloc(sizeof(char)*strlen(ent->d_name)+1);
      snprintf(lista[i],6+(int)strlen(ent->d_name), "%s",ent->d_name);
      printf("%d) %s\n",i+1,lista[i]);
      i++;
    }
  }
  closedir (dir);

  int eleccion;
  scanf("%d", &eleccion);
  snprintf(buffer,2+(int)strlen(lista[eleccion-1])+(int)strlen(nombre), "%s/%s",nombre,lista[eleccion-1]); //  removemos la carta de la carpeta del jugador respectivo
  remove(buffer);

  // Reemplaza por la nueva
  snprintf(buffer,10+(int)strlen(lista[eleccion-1]), "En_Juego/%s",lista[eleccion-1]); // Se la añadimos al jugador respectivo
  creat(buffer,S_IRWXU);

  // Liberamos la memoria
  for(z=0;z<i;z++){
    free(lista[z]);
  }
  free(lista);
  free(buffer);

  return EXIT_SUCCESS;

}
