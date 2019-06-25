#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include "cms.h"
#include "strutil.h"

#define separador ','
#define tam 100

void procesar_tweets(cms_t* cms, char** vector) {
  int i = 0;
  while(vector[i]) {
    cms_guardar(cms, vector[i]);
  }
}

int main(int argc, char *argv[]) {

  if(argc != 3) {
    fprintf(stderr, "%s\n", "Error: Cantidad erronea de parametros");
    return -1;
  }
  int n = atoi(argv[1]);
  // int k = atoi(argv[2]);

  cms_t* cms = cms_crear(tam);
  if(!cms) {
    fprintf(stderr, "%s\n", "Error: Memoria insuficiente");
    return -1;
  }

  // size_t leidos;
  size_t capacidad = 0;
  char* linea = NULL;

  for(int i = 0; i < n; i++) {

    getline(&linea, &capacidad, stdin);
    char** vector_de_tweets = split(linea, separador);
    procesar_tweets(cms, vector_de_tweets);
  }

  // buscar_los_k_tt()




  return 0;
}
