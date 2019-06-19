#include "pila.h"
#include <stdlib.h>

/* Definición de constantes
 */
const int FACTOR_REDIMENSIONAR = 2;
const int TAM_INICIAL = 2; // TAM_INICIAL no puede ser 0


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};


/* *****************************************************************
 *                         AUXILIARES
 * *****************************************************************/

bool hay_que_incrementar_capacidad(pila_t* pila) {
  return pila->cantidad == pila->capacidad;
}

bool hay_que_disminuir_capacidad(pila_t* pila) {
  /* Hay que disminuir la capacidad de la pila cuando la cantidad es un cuarto
    de la capacidad. La capacidad nunca puede ser menor que la capacidad
    inicial (TAM_INICIAL)
    */
  return pila->cantidad *4 <= pila->capacidad && pila->capacidad / FACTOR_REDIMENSIONAR >= TAM_INICIAL;
}

bool incrementar_capacidad_pila(pila_t* pila) {

  void** datos_nuevos = realloc(pila->datos, pila->capacidad * FACTOR_REDIMENSIONAR * sizeof(void*));

  if(datos_nuevos == NULL) return false;

  pila->datos = datos_nuevos;
  pila->capacidad *= FACTOR_REDIMENSIONAR;
  return true;
}


bool disminuir_capacidad_pila(pila_t* pila) {

  void* datos_nuevos = realloc(pila->datos, pila->capacidad  * sizeof(void*) / FACTOR_REDIMENSIONAR);

  if(datos_nuevos == NULL) return false;

  pila->datos = datos_nuevos;
  pila->capacidad /= FACTOR_REDIMENSIONAR;
  return true;
}


/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void) {

  pila_t* pila = malloc(sizeof(pila_t));

  if(pila == NULL) return NULL;

  pila->datos = malloc(TAM_INICIAL * sizeof(void*));

  if(pila->datos == NULL) {
    free(pila);
    return NULL;
  }

  pila->cantidad = 0;
  pila->capacidad = TAM_INICIAL;

  return pila;
}


void pila_destruir(pila_t *pila) {

  free(pila->datos);
  free(pila);
}


bool pila_esta_vacia(const pila_t *pila) {

  return pila->cantidad == 0;
}


bool pila_apilar(pila_t *pila, void* valor) {

  if(hay_que_incrementar_capacidad(pila)) {

    if(!incrementar_capacidad_pila(pila)) return false;
  }

  pila->datos[pila->cantidad] = valor;
  (pila->cantidad)++;
  return true;
}

void* pila_ver_tope(const pila_t *pila) {

  return pila_esta_vacia(pila) ? NULL : pila->datos[(pila->cantidad) - 1];
}

void* pila_desapilar(pila_t *pila) {

  if(pila_esta_vacia(pila)) return NULL;

  pila->cantidad --;
  void* elemento_tope = pila->datos[pila->cantidad];

  if(hay_que_disminuir_capacidad(pila)) disminuir_capacidad_pila(pila);

  return elemento_tope;
}
