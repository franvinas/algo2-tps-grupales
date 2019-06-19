#include "abb.h"
#include "pila.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct abb_nodo{
    char* clave;
    void* dato;
    struct abb_nodo* izq;
    struct abb_nodo* der;
} abb_nodo_t;

struct abb{
    abb_nodo_t* raiz;
    size_t cant;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
};

struct abb_iter{
  pila_t* pila;
};

/******************************************************************************
                            FUNCIONES AUXILIARES
*******************************************************************************/

abb_nodo_t* abb_nodo_buscar_padre(const abb_t* abb, abb_nodo_t* nodo_actual, const char* clave) {

    if(!nodo_actual || abb->cmp(nodo_actual->clave, clave) == 0)
        return NULL;

    if(abb->cmp(nodo_actual->clave, clave) > 0) {
        if(nodo_actual->izq == NULL || abb->cmp(nodo_actual->izq->clave, clave) == 0)
            return nodo_actual;
        return abb_nodo_buscar_padre(abb, nodo_actual->izq, clave);
    }

    if(nodo_actual->der == NULL || abb->cmp(nodo_actual->der->clave, clave) == 0)
        return nodo_actual;
    return abb_nodo_buscar_padre(abb, nodo_actual->der, clave);

}


abb_nodo_t* abb_buscar_hijo(const abb_t* arbol, abb_nodo_t* padre, const char* clave){

    if(!padre) {
      return arbol->raiz;
    }
    if(arbol->cmp(padre->clave, clave) > 0)
        return padre->izq;
    return padre->der;
}

abb_nodo_t* abb_nodo_buscar(const abb_t* arbol, abb_nodo_t* nodo_actual, const char* clave){

  if(!nodo_actual) return NULL;

  abb_nodo_t* padre = abb_nodo_buscar_padre(arbol, arbol->raiz, clave);
  return abb_buscar_hijo(arbol, padre, clave);
}

abb_nodo_t* nodo_crear(const char* clave, void* dato) {
    abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
    if(!nodo) return NULL;
    nodo->clave = malloc(sizeof(clave) + 1);
    if(!nodo->clave) {
        free(nodo);
        return NULL;
    }
    strcpy(nodo->clave, clave);
    nodo->dato = dato;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}


void* nodo_borrar(abb_nodo_t* nodo){
    free(nodo->clave);
    void* dato_a_borrar = nodo->dato;
    free(nodo);
    return dato_a_borrar;
}

bool tiene_ambos_hijos(abb_nodo_t* nodo){
    return nodo->der != NULL && nodo->izq != NULL;
}

bool es_hoja(abb_nodo_t* nodo){
    return nodo->der == NULL && nodo->izq == NULL;
}

bool solo_hijo_derecho(abb_nodo_t* nodo){
    return nodo->der != NULL && nodo->izq == NULL;
}

bool solo_hijo_izquierdo(abb_nodo_t* nodo){
    return nodo->der == NULL && nodo->izq != NULL;
}


void actualizar_padre(abb_t* arbol, abb_nodo_t* nodo_a_borrar, abb_nodo_t* nodo_padre,abb_nodo_t* nodo_reemplazo) {
    if (nodo_padre != NULL){
        if (nodo_padre->izq == nodo_a_borrar)
            nodo_padre->izq = nodo_reemplazo;
        else
            nodo_padre->der = nodo_reemplazo;
    }
    else{
        arbol->raiz = nodo_reemplazo;
    }
}

abb_nodo_t* obtener_mas_izquierdo(abb_nodo_t* nodo_a_borrar){
    if(nodo_a_borrar->izq != NULL)
        return obtener_mas_izquierdo(nodo_a_borrar->izq);

    return nodo_a_borrar;
}

bool abb_in_order_r(abb_nodo_t* nodo_actual, bool visitar(const char *, void *, void *), void *extra) {
  if(nodo_actual->izq)
    if(!abb_in_order_r(nodo_actual->izq, visitar, extra)) return false;
  if(!visitar(nodo_actual->clave, nodo_actual->dato, extra)) return false;
  if(nodo_actual->der)
    if(!abb_in_order_r(nodo_actual->der, visitar, extra)) return false;

  return true;
}

void apilar_todos_izq(abb_nodo_t* nodo_actual, abb_iter_t* iter) {
    while(nodo_actual)  {
        pila_apilar(iter->pila, nodo_actual);
        nodo_actual = nodo_actual->izq;
    }
}

void abb_destruir_r(abb_t* arbol, abb_nodo_t* nodo_a_borrar){
    if (nodo_a_borrar->izq)
        abb_destruir_r(arbol,nodo_a_borrar->izq);
    if (nodo_a_borrar->der)
        abb_destruir_r(arbol,nodo_a_borrar->der);
    void* dato = nodo_borrar(nodo_a_borrar);
    if (arbol->destruir_dato)
        arbol->destruir_dato(dato);
}


/******************************************************************************
                                PRIMITIVAS
*******************************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if (abb == NULL) {
        return NULL;
    }

    abb->raiz = NULL;
    abb->cant = 0;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;

    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    abb_nodo_t* padre = abb_nodo_buscar_padre(arbol, arbol->raiz, clave);
    abb_nodo_t* nodo = abb_buscar_hijo(arbol, padre, clave);

    if(nodo) {
      if(arbol->destruir_dato)
          arbol->destruir_dato(nodo->dato);
      nodo->dato = dato;
      return true;
    }
    nodo = nodo_crear(clave, dato);
    if(!nodo) return false;


    if(arbol->raiz == NULL || !padre) {
      arbol->raiz = nodo;
      arbol->cant++;
      return true;
    }

    if(arbol->cmp(padre->clave, clave) > 0) {
        padre->izq = nodo;
    } else {
      padre->der = nodo;
    }
    arbol->cant++;
    return true;

}

void *abb_borrar(abb_t *arbol, const char *clave){
    abb_nodo_t* nodo_padre = abb_nodo_buscar_padre(arbol, arbol->raiz, clave);
    abb_nodo_t* nodo_a_borrar = abb_buscar_hijo(arbol,nodo_padre,clave);
    if(!nodo_a_borrar) return NULL; //Por si se manda una clave que no pertenecia

    arbol->cant--;

    if (es_hoja(nodo_a_borrar)) {
        actualizar_padre(arbol, nodo_a_borrar,nodo_padre,NULL);
    }
    else if (solo_hijo_derecho(nodo_a_borrar)){
        actualizar_padre(arbol, nodo_a_borrar,nodo_padre,nodo_a_borrar->der);
    }
    else if (solo_hijo_izquierdo(nodo_a_borrar)){
        actualizar_padre(arbol, nodo_a_borrar,nodo_padre,nodo_a_borrar->izq);
    }
    else
    {
        void* dato_a_borrar = nodo_a_borrar->dato;
        abb_nodo_t* nodo_mas_izquierdo = obtener_mas_izquierdo(nodo_a_borrar->der);
        abb_nodo_t* padre_del_mas_izquierdo = abb_nodo_buscar_padre(arbol, arbol->raiz, nodo_mas_izquierdo->clave);
        strcpy(nodo_a_borrar->clave, nodo_mas_izquierdo->clave);
        nodo_a_borrar->dato = nodo_mas_izquierdo->dato;
        actualizar_padre(arbol, nodo_mas_izquierdo,padre_del_mas_izquierdo,nodo_mas_izquierdo->der);
        nodo_borrar(nodo_mas_izquierdo);

        return dato_a_borrar;
    }

    return nodo_borrar(nodo_a_borrar);
}


void *abb_obtener(const abb_t *arbol, const char *clave){

    abb_nodo_t* nodo = abb_nodo_buscar(arbol, arbol->raiz, clave);
    return nodo ? nodo->dato : NULL;

}


bool abb_pertenece(const abb_t *arbol, const char *clave){
    return abb_nodo_buscar(arbol, arbol->raiz, clave) != NULL;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

void abb_destruir(abb_t *arbol) {//haganlo a mano sin iterador (fijense recursivamente como lo pueden hacer)

    if (arbol->raiz)
      abb_destruir_r(arbol,arbol->raiz);

    free(arbol);
}


void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
  if(!arbol->raiz) return;
  abb_in_order_r(arbol->raiz, visitar, extra);
}


/******************************************************************************
                            PRIMITIVAS DEL ITERADOR
*******************************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
  abb_iter_t* iter = malloc(sizeof(abb_iter_t));
  if(!iter) return NULL;

  iter->pila = pila_crear();
  if(!iter->pila) {
    free(iter);
    return NULL;
  }

  abb_nodo_t* nodo_actual = arbol->raiz;
  apilar_todos_izq(nodo_actual, iter);

  return iter;
}



bool abb_iter_in_avanzar(abb_iter_t *iter){
  abb_nodo_t* desapilado = pila_desapilar(iter->pila);
  if(!desapilado) return false;

  abb_nodo_t* nodo_actual = desapilado->der;
  apilar_todos_izq(nodo_actual, iter);
  return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
  abb_nodo_t* nodo_tope = pila_ver_tope(iter->pila);
  return nodo_tope ? nodo_tope->clave : NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
  return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
  pila_destruir(iter->pila);
  free(iter);
}
