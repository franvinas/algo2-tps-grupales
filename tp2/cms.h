#ifndef _CMS_H
#define _CMS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct cms cms_t;

/* Crea el cms
 */
cms_t* cms_crear(size_t tam);

/* Incrementa en uno la cantidad de apariciones de la clave,
 * Pre: La estructura cms fue inicializada
 * Post: Se incremento el valor
 */
void cms_guardar(cms_t* cms, const char* clave);

/* Devuelve la cantidad de apariciones de la clave (aproximada)
 * Pre: La estructura cms fue inicializada
 */
size_t cms_obtener(const cms_t *cms, const char *clave);

/* Destruye la estructura liberando la memoria pedida
 * Pre: La estructura cms fue inicializada
 * Post: La estructura cms fue destruida
 */
void cms_destruir(cms_t* cms);


#endif // CMS_H
