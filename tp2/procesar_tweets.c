#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include "cms.h"
#include "strutil.h"
#include "heap.h"
#include "hash.h"

#define SEPARADOR ','

typedef struct nodo_tweet {
    char* tweet;
    int frecuencia;
} nodo_tweet_t;

int funcion_cmp_heap(const void* tt1, const void* tt2){

  const nodo_tweet_t* a = tt1;
  const nodo_tweet_t* b = tt2;

  if (a->frecuencia != b->frecuencia)
    return (a->frecuencia - b->frecuencia)*(-1);
  else
    return strcmp(a->tweet,b->tweet);
}

nodo_tweet_t* nodo_tweet_crear(const char* tweet, int frecuencia) {
  nodo_tweet_t* nodo = malloc(sizeof(nodo_tweet_t));
  if(!nodo) return NULL;
  nodo->tweet = malloc(sizeof(tweet));
  strcpy(nodo->tweet, tweet);
  if(!nodo->tweet) {
    free(nodo);
    return NULL;
  }
  nodo->frecuencia = frecuencia;
  return nodo;
}

void nodo_tweet_destruir(void* nodo) {
  nodo_tweet_t* nodo_a_destruir = nodo;
  free(nodo_a_destruir->tweet);
  free(nodo_a_destruir);
}

void imprimir_top_k(heap_t* heap, int k, int nro_linea) {
  nodo_tweet_t* top_tweets[k];
  int i = 0;
  while(!heap_esta_vacio(heap)) {
    top_tweets[i] = (nodo_tweet_t*) heap_desencolar(heap);
    i++;
  }
  printf("--- %i\n", nro_linea);
  for(int j = 0; j < i; j++) {
    printf("%i %s\n",top_tweets[i - j - 1]->frecuencia, top_tweets[i - j - 1]->tweet);
    nodo_tweet_destruir(top_tweets[i - j - 1]);
  }
}

void procesar_tweets(cms_t* cms, hash_t* hash, char** vector) {
  int i = 0;
  while(vector[i]) {
    if(strlen(vector[i]) > 0) {
      cms_guardar(cms, vector[i]);
      hash_guardar(hash, vector[i]);
    }
    i++;
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

  cms_t* cms = cms_crear(n * 100);
  if(!cms) {
    fprintf(stderr, "%s\n", "Error: Memoria insuficiente");
    return -1;
  }

  int cant_impresiones = 1;
  bool seguir = true;
  while(seguir) {

    size_t leidos;
    size_t capacidad = 0;
    char* linea = NULL;
    hash_t* visitados = hash_crear();

    for(int i = 0; i < n; i++) {
      leidos = getline(&linea, &capacidad, stdin);
      if(leidos > 1) {
        linea[leidos - 1] = '\0';
        char** vector_de_tweets = split(linea, SEPARADOR);
        procesar_tweets(cms, visitados, vector_de_tweets);
      } else {
        seguir = false;
        break;
      }
    }
    if(feof(stdin)) seguir = false;

    heap_t* heap = heap_crear(funcion_cmp_heap);
    hash_iter_t* iter_visitados = hash_iter_crear(visitados);
    int i = 0;
    while(!hash_iter_al_final(iter_visitados)) {
      const char* tweet = hash_iter_ver_actual(iter_visitados);
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
      i++;
      hash_iter_avanzar(iter_visitados);
    }

    imprimir_top_k(heap, k, cant_impresiones);
    heap_destruir(heap, nodo_tweet_destruir);
    hash_destruir(visitados);
    cant_impresiones++;
  }

  return 0;
}
