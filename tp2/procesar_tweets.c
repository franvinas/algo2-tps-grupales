#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include "cms.h"
#include "strutil.h"
#include "heap.h"
#include "hash.h"

#define SEPARADOR ','
#define TAM 100

typedef struct nodo_tweet {
    char* tweet;
    int frecuencia;
} nodo_tweet_t;

int funcion_cmp_heap(const void* tt1, const void* tt2){

  const nodo_tweet_t* a = tt1;
  const nodo_tweet_t* b = tt2;

  if (a->frecuencia != b->frecuencia)
    return a->frecuencia - b->frecuencia;
  else
    return strcmp(a->tweet,b->tweet);
}

nodo_tweet_t* nodo_tweet_crear(const char* tweet, int frecuencia) {
  nodo_tweet_t* nodo = malloc(sizeof(nodo_tweet_t));
  if(!nodo) return NULL;
  nodo->tweet = malloc(sizeof(tweet));
  if(!nodo->tweet) {
    free(nodo);
    return NULL;
  }
  nodo->frecuencia = frecuencia;
  return nodo;
}

void nodo_tweet_destruir(nodo_tweet_t* nodo) {
  free(nodo->tweet);
  free(nodo);
}

void procesar_tweets(cms_t* cms, hash_t* hash, char** vector) {
  int i = 0;
  while(vector[i]) {
    cms_guardar(cms, vector[i]);
    hash_guardar(hash, vector[i]);
  }
}

bool arr_pertenece(char** arr, size_t tam, char* buscado) {
  for(size_t i = 0; i < tam; i++) {
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
  int k = atoi(argv[2]);

  cms_t* cms = cms_crear(TAM);
  if(!cms) {
    fprintf(stderr, "%s\n", "Error: Memoria insuficiente");
    return -1;
  }

  // size_t leidos;
  size_t capacidad = 0;
  char* linea = NULL;
  hash_t* visitados = hash_crear();


  for(int i = 0; i < n; i++) {

    getline(&linea, &capacidad, stdin);
    char** vector_de_tweets = split(linea, SEPARADOR);
    procesar_tweets(cms, visitados, vector_de_tweets);
  }

  heap_t* heap = heap_crear(funcion_cmp_heap);
  hash_iter_t* iter_visitados = hash_iter_crear(visitados);
  int i = 0;
  while(!hash_iter_al_final(iter_visitados)) {
    const char* tweet = hash_iter_ver_actual(iter_visitados);
    if(!hash_pertenece(visitados, tweet)) {

      nodo_tweet_t* nodo = nodo_tweet_crear(tweet, cms_obtener(cms, tweet));
      if(i < k) {
        heap_encolar(heap, nodo);
      } else if(funcion_cmp_heap(heap_ver_max(heap), hash_iter_ver_actual(iter_visitados) ) < 0 ) {
        nodo_tweet_t* nodo_desencolado = heap_desencolar(heap);
        nodo_tweet_destruir(nodo_desencolado);
        heap_encolar(heap, nodo);
      } else {
        nodo_tweet_destruir(nodo);
      }
    }
    i++;
    hash_iter_avanzar(iter_visitados);
  }
  // en el heap quedan los k tt


  // hay que destruir todo lo que se creo
  return 0;
}
