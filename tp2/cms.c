#include "cms.h"

struct cms {
  size_t* tabla;
  size_t capacidad;
};

// Djb2 hash function (Funcion de Hash sacada de internet)
size_t funcion_hash(const char *str, size_t capacidad) {
    size_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % capacidad;
}

/****************************************************************
 *                        PRIMITIVAS
 ****************************************************************/

cms_t* cms_crear(size_t tam) {
  cms_t* cms = malloc(sizeof(cms_t));
  if(!cms) return NULL;

  cms->tabla = malloc(tam * sizeof(size_t));
  if(!cms->tabla) {
    free(cms);
    return NULL;
  }

  cms->capacidad = tam;
  return cms;
}

void cms_guardar(cms_t* cms, const char* clave) {
  size_t pos = funcion_hash(clave, cms->capacidad);
  cms->tabla[pos]++;
}

size_t cms_obtener(const cms_t *cms, const char *clave) {
  size_t pos = funcion_hash(clave, cms->capacidad);
  return cms->tabla[pos];
}

void cms_destruir(cms_t* cms) {
  free(cms->tabla);
  free(cms);
}
