#include "cms.h"

struct cms {
  int* tabla;
  int capacidad;
};

// Djb2 hash function (Funcion de Hash sacada de internet)
int funcion_hash_1(const char *str, int capacidad) {
    int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % capacidad;
}

/****************************************************************
 *                        PRIMITIVAS
 ****************************************************************/

cms_t* cms_crear(int tam) {
  cms_t* cms = malloc(sizeof(cms_t));
  if(!cms) return NULL;

  cms->tabla = malloc(tam * sizeof(int));
  if(!cms->tabla) {
    free(cms);
    return NULL;
  }

  cms->capacidad = tam;
  return cms;
}

void cms_guardar(cms_t* cms, const char* clave) {
  int pos = funcion_hash_1(clave, cms->capacidad);
  cms->tabla[pos]++;
}

int cms_obtener(const cms_t *cms, const char *clave) {
  int pos = funcion_hash_1(clave, cms->capacidad);
  return cms->tabla[pos];
}

void cms_destruir(cms_t* cms) {
  free(cms->tabla);
  free(cms);
}
