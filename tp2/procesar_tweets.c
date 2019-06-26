#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include "cms.h"
#include "strutil.h"
#include "heap.h"
#include "hash.h"

#define separador ','
#define tam 100

typedef struct nodo_tweet {
    char* tweet;
    int frecuencia;
} nodo_tweet_t;

void procesar_tweets(cms_t* cms, hash_t* hash, char** vector) {
  int i = 0;
  while(vector[i]) {
    cms_guardar(cms, vector[i]);
    hash_guardar(hash, vector[i], true)
  }
}

bool arr_pertenece(char** arr, size_t tam, char* buscado) {
  for(size_t i = 0; i < tam, i++) {
    if(strcmp(arr[i], buscado) == 0) return true;
  }
  return false;
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
  hash_t* visitados = hash_crear(NULL);


  for(int i = 0; i < n; i++) {

    getline(&linea, &capacidad, stdin);
    char** vector_de_tweets = split(linea, separador);
    procesar_tweets(cms, visitados, vector_de_tweets);
  }

  heap_t* heap = heap_crear(cmp);
  hash_iter_t* iter_visitados = hash_iter_crear(visitados);
  while(!hash_iter_al_final(iter_visitados)) {
    if(i < k) {
      heap_encolar(heap, hash_iter_ver_actual(iter_visitados));
    } else if(cmp(heap_ver_max(heap), hash_iter_ver_actual(iter_visitados) < 0 ) ) {
      heap_desencolar(heap);
      heap_encolar(heap, hash_iter_ver_actual(iter_visitados));
    }
    i++;
    hash_iter_avanzar(iter_visitados);
  }
  // en el heap quedan los k tt


  // hay que destruir todo lo que se creo
  return 0;
}
