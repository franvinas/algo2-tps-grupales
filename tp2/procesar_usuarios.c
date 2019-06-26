#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include "cms.h"
#include "strutil.h"

#define separador ','

void agregar_tags(matriz, pos, strv) {
  size_t i = 1; // empieza en 1 porque en la pos 0 esta el usuario
  size_t j = 1;
  while(matriz[pos][i]) {
    if(strv[j] == NULL)
  }
}

void procesar_tags(matriz, cantidad_usuarios, strv) {
  size_t i = 0;
  while(i < cantidad_usuarios) {
    if(stcmp(matriz[i][0], strv[0]) == 0) {
      agregar_tags(matriz, i, strv);
      return;
    }
    i++;
  }
  matriz[i] = strv;
}


int main(int argc, char *argv[]) {

  if(argc != 2) {
    fprintf(stderr, "%s\n", "Error: Cantidad erronea de parametros");
    return -1;
  }

  FILE* archivo = fopen(argv[1], "r");
  if(!archivo) {
    fprintf(stderr, "%s", "Error: archivo fuente inaccesible\n");
    return -1;
  }
  // cms_t* cms = cms_crear(100);

  size_t leidos;
  size_t capacidad = 0;
  size_t tam_arr = 2;
  char* linea = NULL;
  char*** arr_de_lineas = malloc(tam_arr * sizeof(char*));
  char** strv;

  leidos = getline(&linea, &capacidad, archivo);
  int i = 0;
  while(!feof(archivo)) {
    linea[leidos -1] = '\0';
    strv = split(linea, separador); // es un arreglo de arreglos que tinenen en la primera posicion al usuario y en las siguientes a los tags
    // procesar_tags();
    arr_de_lineas[i] = strv;
    i++;
    leidos = getline(&linea, &capacidad, archivo);
    if(i == tam_arr) {
      tam_arr *= 2;
      arr_de_lineas = realloc(arr_de_lineas, tam_arr);
    }
  }
  arr_de_lineas[i] = NULL;

  printf("%s\n", arr_de_lineas[2][0]);

  // char** usuarios = arr_de_lineas[0];
  //
  // printf("%s\n", usuarios[0]);
  // printf("%s\n", usuarios[1]);






  return 0;
}
