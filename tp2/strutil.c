#define _GNU_SOURCE
#include "strutil.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// FUNCIONES AUXILIARES

size_t contador_de_subcadenas(const char* str, char sep) {
  size_t contador = 1; // Siempre hay como minimo una cadena vacia
  size_t largo = strlen(str);

  for(size_t i = 0; i < largo; i++) {
    if(str[i] == sep)
      contador++;
  }
  return contador;
}


// Devuelve la cantidad de caracteres totales que hay en un vector de
// strings + la cantidad de separadores que deberia haber
size_t cantidad_caracteres(char** strv) {

  if(!strv) return 0;

  size_t contador = 0;
  size_t i = 0;
  size_t cant_separadores;

  while(strv[i]) {
    contador += strlen(strv[i]);
    i++;
  }

  if( i <= 1)
    cant_separadores = 0;
  else
    cant_separadores = i - 1;

  return contador + cant_separadores;
}


/********************************************************************
                            PRIMITIVAS
*********************************************************************/

char *substr(const char *str, size_t n) {

  if(n > strlen(str))
    n = strlen(str);

  char* cadena = malloc(n * sizeof(char) + 1);
  if(!cadena) return NULL;

  strncpy(cadena, str, n);

  cadena[n] = '\0';

  return cadena;
}

char **split(const char *str, char sep) {

  size_t largo = strlen(str);
  size_t cant_subcadenas = contador_de_subcadenas(str, sep);
  char** strv = malloc((cant_subcadenas + 1) * sizeof(char*)); // +1 para que el ultimo sea NULL
  size_t indice_subcadena = 0;
  size_t inicio = 0;

  for(size_t i = 0; i <= largo; i++) {
    if(str[i] == sep || i == largo) {
      strv[indice_subcadena] = substr(str + inicio, i - inicio);
      inicio = i + 1;
      indice_subcadena++;
    }
  }
  strv[indice_subcadena] = NULL;

  return strv;
}

char *join(char **strv, char sep) {

  size_t tam = cantidad_caracteres(strv);
  char* cadena = malloc((tam + 1) * sizeof(char));
  char separador[2];
  separador[0] = sep;
  separador[1] = '\0';

  if(!strv || !strv[0]) {
    strcpy(cadena, "");
    return cadena;
  }

  char* final = stpcpy(cadena, strv[0]);
  for(size_t i = 1; strv[i]; i++) {
    final = stpcpy(final, separador);
    final = stpcpy(final, strv[i]);
  }

  return cadena;
}

void free_strv(char** strv) {

  size_t i = 0;

  while(strv[i]) {
    free(strv[i]);
    i++;
  }
  free(strv);

}
