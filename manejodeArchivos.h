#ifndef MANEJODEARCHIVOS_H
#define MANEJODEARCHIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  // para mkdir
#include <sys/stat.h>
#include <fcntl.h>  // para open
#include <unistd.h> // para rmkdir
#include <dirent.h> // para directorios


int crearJugadores();

int crearMazo();

int imprimirMazo();

#endif
