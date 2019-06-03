#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

#define TAM_INICIAL 11
#define FACTOR_REDIMENSION 2

struct heap {
  void** vector;
  size_t cantidad;
  size_t capacidad;
  cmp_func_t cmp;
};


/***********************************************************************
*                         FUNCIONES AUXILIARES
************************************************************************/

void swap(void** vector, size_t pos1, size_t pos2) {

  void* aux = vector[pos1];
  vector[pos1] = vector[pos2];
  vector[pos2] = aux;
}

size_t obtener_padre(size_t pos) {
  return (pos - 1) / 2;
}

size_t obtener_hijo_izq(size_t pos) {
  return 2 * pos + 1;
}

size_t obtener_hijo_der(size_t pos) {
  return 2 * pos + 2;
}

void upheap(void** vector, size_t pos, cmp_func_t cmp) {
  if(pos == 0) return;
  size_t padre = obtener_padre(pos);
  if(cmp(vector[padre], vector[pos]) < 0) {
    swap(vector, padre, pos);
    upheap(vector, padre, cmp);
  }
}

// Pre: existen los dos hijos
size_t buscar_hijo_max(void** vector, size_t hijo1, size_t hijo2, cmp_func_t cmp) {
  return cmp(vector[hijo1], vector[hijo2]) > 0 ? hijo1 : hijo2;
}

void downheap(void** vector, size_t tam, size_t pos, cmp_func_t cmp) {
  size_t hijo_izq = obtener_hijo_izq(pos);
  size_t hijo_der = obtener_hijo_der(pos);
  size_t pos_hijo_max;

  if(hijo_izq >= tam) return;
  if(hijo_der >= tam)
    pos_hijo_max = hijo_izq;
  else
    pos_hijo_max = buscar_hijo_max(vector, hijo_izq, hijo_der, cmp);

  if(cmp(vector[pos], vector[pos_hijo_max]) < 0) {
    swap(vector, pos, pos_hijo_max);
    downheap(vector, tam, pos_hijo_max, cmp);
  }
}

bool heap_redimensionar(heap_t* heap, size_t tam_nuevo) {
  void** arreglo_nuevo = realloc(heap->vector, tam_nuevo * sizeof(void*));
  if(!arreglo_nuevo) return false;
  heap->vector = arreglo_nuevo;
  heap->capacidad = tam_nuevo;
  return true;
}

void heapify(void** vector, size_t tam, size_t pos, cmp_func_t cmp) {
  for(size_t i = 0; i <= pos; i++) {
    downheap(vector, tam, pos - i, cmp);
  }
}

/***********************************************************************
*                              PRIMITIVAS
************************************************************************/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
  heapify(elementos, cant, (cant - 1) / 2, cmp);
  size_t tam = cant;
  while (tam > 1) {
    swap(elementos, 0, tam -1);
    downheap(elementos, tam - 1, 0, cmp);
    tam--;
  }
}

heap_t *heap_crear(cmp_func_t cmp) {
  heap_t* heap = malloc(sizeof(heap_t));
  if(!heap) return NULL;

  heap->cmp = cmp;

  heap->vector = malloc(TAM_INICIAL * sizeof(void*));
  if(!heap->vector) {
    free(heap);
    return NULL;
  }

  heap->cantidad = 0;
  heap->capacidad = TAM_INICIAL;
  return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {

  heap_t* heap = heap_crear(cmp);
  if(!heap) return NULL;

  if(n > TAM_INICIAL) {
    if(!heap_redimensionar(heap, n * FACTOR_REDIMENSION)) return NULL;
  }

  for(size_t i = 0; i < n; i++) {
    heap->vector[i] = arreglo[i];
    heap->cantidad ++;//ya saben que la cantidad va a ser n
  }

  heapify(heap->vector, heap->cantidad, heap->cantidad, cmp);
  return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {

  if(destruir_elemento) {

    for(size_t i = 0; i < heap->cantidad; i++)
      destruir_elemento(heap->vector[i]);

  }
  free(heap->vector);
  free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
  return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
  return heap->cantidad == 0;
}

bool heap_encolar(heap_t *heap, void *elem) {
  if(!elem) return false;
  if(heap->cantidad == heap->capacidad)
    if(!heap_redimensionar(heap, heap->capacidad * FACTOR_REDIMENSION)) return false; // Si no pudo redimensionar devuelvo false

  heap->vector[heap->cantidad] = elem;
  upheap(heap->vector, heap->cantidad, heap->cmp);
  heap->cantidad++;
  return true;
}

void *heap_ver_max(const heap_t *heap) {
  return heap_esta_vacio(heap) ? NULL : heap->vector[0];
}

void *heap_desencolar(heap_t *heap) {

  if(heap_esta_vacio(heap)) return NULL;
  void* elem_a_eliminar = heap->vector[0];
  heap->cantidad--;
  swap(heap->vector, 0, heap->cantidad);
  downheap(heap->vector, heap->cantidad, 0, heap->cmp);

  if(heap->capacidad > heap->cantidad * FACTOR_REDIMENSION * 2) //recuerden que no tienen que redimensionar a menos que el tamaño inicial
    heap_redimensionar(heap, heap->capacidad / FACTOR_REDIMENSION);

  return elem_a_eliminar;
}
