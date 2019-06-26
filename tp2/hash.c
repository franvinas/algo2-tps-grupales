#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum nodo_estados{ //podrian definir los estados como enums
    LIBRE = 0,
    OCUPADO = 1,
    BORRADO = 2
};

#define CONSTANTE_CAPACIDAD_INICIAL 11
#define PROPORCION_REDIMENSION 50
#define CONSTANTE_DE_REDIMENSION 2


//estado guarda el estado actual del nodo el cual puede ser LIBRE,OCUPADO O BORRADO (Se va modificando cuando es necesario)
typedef struct nodo_hash {
    char* clave;
    int estado;
} nodo_hash_t;


//datos es un arreglo de struct nodo
//capacidad guarda la cantidad total de posiciones que tiene el arreglo datos
//cantidad_ocupados guarda la cantidad de nodos en estado OCUPADO
//cantidad_borrados guarda la cantidad de nodos en estado  BORRADO
struct hash {
    nodo_hash_t* datos;
    size_t capacidad;
    size_t cantidad_ocupados;
    size_t cantidad_borrados;
};


//hash es un puntero a la estructura que se quiere iterar
//pos guarda la posicion en cual se encontro la clave
struct hash_iter {
    const hash_t* hash;
    size_t pos;
};



// FUNCIONES AUXILIARES

//Reserva espacio en memoria para la clave, asigna la clave y el dato y cambia el estado del nodo a "ocupado"
//Post: Retorna true en caso de poder reservar memoria para la clave.
bool nodo_hash_guardar(nodo_hash_t* nodo_hash, const char *clave) {

    nodo_hash->clave = malloc((strlen(clave) + 1) * sizeof(char));

    if(!nodo_hash->clave)
        return false;
    strcpy(nodo_hash->clave, clave);
    nodo_hash->estado = OCUPADO;
    return true;
}

// Se crea un nodo de estado vacio para cada posicion del vector de datos
// Post: devuelve false si no se pudieron crear todos los nodos
bool setear_nodos(hash_t* hash) {//devuelta no es que se crea, es que se setean los valores a NULL,NULL y LIBRE, la estructura ya esta creada
    for(int i = 0; i < hash->capacidad; i++) {
        hash->datos[i].clave = NULL;
        hash->datos[i].estado = LIBRE;
    }
    return true;
}

//Post: Inicializa el hash asignando la funcion destruir que le llega por parametro , la capacidad total y reserva espacio en memoria para los datos
void crear_tabla_hash(hash_t* hash,hash_destruir_dato_t destruir_dato,size_t capacidad_total){//aca si seria un crear_tabla_hash porque hacen un malloc de la tabla
    hash->cantidad_borrados = 0;
    hash->cantidad_ocupados = 0;
    hash->capacidad = capacidad_total;
    hash->datos = malloc(capacidad_total * sizeof(nodo_hash_t));
}


// Djb2 hash function (Funcion de Hash sacada de internet)
size_t funcion_hash(const char *str, size_t capacidad) {
    size_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % capacidad;
}


//Post: Dado un hash verifica si es necesario redimensionar
bool debe_redimensionar_ampliar(const hash_t* hash){//tiene sentido una funcion aux para solo una linea de codigo? (la dejamos porqe nos da claridad en el codigo)
    return ((hash->cantidad_ocupados + hash->cantidad_borrados) * 100) / hash->capacidad >= PROPORCION_REDIMENSION;
}


//Pre: Recibe un hash inicializado y una clave la cual puede pertenecer al hash o no,
//Post: Dada una clave te dice su posicion (en caso de existir) y en caso de no existir te devuelve la posicion donde tenes que insertarla.
size_t posicion_clave(const char* clave , const hash_t* hash){

    bool puede_estar = true;
    size_t posicion_inicial = funcion_hash(clave,hash->capacidad);
    size_t posicion = posicion_inicial;

    while(hash->datos[posicion].estado != LIBRE) {//se podria usar un for(int pos=0;hash->datos[pos].estado != LIBRE;pos++ )

        if (hash->datos[posicion].estado == OCUPADO && strcmp(hash->datos[posicion].clave,clave) == 0)
            return posicion;


        if (!puede_estar && hash->datos[posicion].estado == BORRADO) //Este es el caso donde la clave no estaba entonces inserto en el primero que encuentro borrado
            return posicion;

        //si hacen (pos+1)%tamaño siempre les va a devolver un valor dentro del tamaño de la tabla. Aun cuando lleguen al final les va a devolver 0 despues 1 y asi
        posicion = (posicion+1) % hash->capacidad;
        if (posicion == posicion_inicial) // Significa que di toda la vuelta y no lo encontre (tengo que insertar en un borrado)
            puede_estar = false;
    }
    return posicion;
}


//Post: Libera la clave y dato asociado al nodo. Devuelve el valor del par (clave,valor). Cambia el estado del nodo a BORRADO.
void* nodo_borrar(hash_t* hash, nodo_hash_t* nodo){
    free(nodo->clave);

    void* dato_a_borrar = nodo->dato;

    //borrar comentarios

    nodo->estado = BORRADO;

    return dato_a_borrar;
}


//Post: Redimensiona el hash con la nueva capacidad. Copia todas las (clave,valor) que habia en el hash y las vuelve a insertar en su nueva posicion
bool redimensionar_hash(hash_t* hash, size_t nueva_capacidad){

    size_t capacidad_actual = hash->capacidad;
    nodo_hash_t* datos_actual = hash->datos;
    crear_tabla_hash(hash,hash->destruir_dato,nueva_capacidad);

    if (nueva_capacidad > 0 && !hash->datos) {
        return false;
    }

    setear_nodos(hash);

    for (int i = 0; i < capacidad_actual; i++){
        if (datos_actual[i].estado == OCUPADO){
            hash_guardar(hash, datos_actual[i].clave);
            free(datos_actual[i].clave);
        }
    }

    free(datos_actual);

    return true;

}


// Devuelve capacidad del hash si no hay mas posiciones ocupadas despues de la actual
size_t buscar_proxima_posicion_ocupada(const hash_t* hash, size_t posicion) {

    for (size_t i = posicion; i < hash->capacidad ; ++i)
    {
        if (hash->datos[i].estado == OCUPADO)
            return i;
    }
    return hash->capacidad;
}


/*****************************************************************
                            PRIMITIVAS
******************************************************************/


//Post: Crea un hash asignando la funcion de destruccion enviada por parametro , capacidad y setea ocupados y borrados a 0.
//      Ademas inicializa todos los nodos del vector dato a libre para que ninguna posicion sea NULL
hash_t *hash_crear(void){
    hash_t* hash = malloc(sizeof(hash_t));
    if (hash == NULL) {
        return NULL;
    }

    crear_tabla_hash(hash,destruir_dato,CONSTANTE_CAPACIDAD_INICIAL);

    if (hash->datos == NULL){
        free(hash);
        return NULL;
    }

    setear_nodos(hash);

    return hash;
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave){


    size_t posicion = posicion_clave(clave,hash); //Te devuelve la posicion para insertar el nodo
    bool ya_existia = hash->datos[posicion].estado == OCUPADO; // Si ya existia no hay que modificar la cantidad; hay que modificar los miembros del nodo nada mas

    if(ya_existia) {
        return true;
    }

    if (hash->datos[posicion].estado == BORRADO)//nunca va a pasar esto si lo hacen despues de nodo_hash_guardar
        hash->cantidad_borrados--;


    if(!nodo_hash_guardar(&hash->datos[posicion], clave)) return false;


    hash->cantidad_ocupados++;

    if (debe_redimensionar_ampliar(hash)){ //redimensionar en caso necesario
        return redimensionar_hash(hash, hash->capacidad * CONSTANTE_DE_REDIMENSION);//que pasa si falla?
    }


    return true;
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave){//no redimensionan hacia abajo
    size_t  pos = posicion_clave(clave,hash);

    if (hash->datos[pos].estado != OCUPADO) {
        return NULL;
    }

    hash->cantidad_ocupados--;
    hash->cantidad_borrados++;

    return nodo_borrar(hash,&hash->datos[pos]);
}


/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
//Post: devuelve true en caso de encontrar la clave , false en caso contrario
bool hash_pertenece(const hash_t *hash, const char *clave){
    size_t  pos = posicion_clave(clave,hash);
    return hash->datos[pos].estado == OCUPADO && !strcmp(hash->datos[pos].clave, clave);
}


/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
//Post: Devuelve la cantidad de ocupados en el hash
size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad_ocupados;
}

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash){
    for (int i = 0; i < hash->capacidad; ++i) {
        if (hash->datos[i].estado == OCUPADO){
            hash_borrar(hash,hash->datos[i].clave);
        }
    }

    free(hash->datos);
    free(hash);
}
//
// /* Iterador del hash */
//
// //Post: Crea iterador apuntando a la primer posicion ocupada que encuentre del hash (apunta a capacidad en caso de no encontrar ninguna ocupada)
// hash_iter_t *hash_iter_crear(const hash_t *hash){
//
//     hash_iter_t* hash_iter = malloc(sizeof(hash_iter_t));
//     if(!hash_iter) return NULL;
//
//     size_t posicion = buscar_proxima_posicion_ocupada(hash, 0);
//
//     hash_iter->hash = hash;
//     hash_iter->pos = posicion;
//     return hash_iter;
// }
//
// //Post: Avanza el iterador a la proxima posicion ocupada que encuentre
// bool hash_iter_avanzar(hash_iter_t *iter){
//
//     if(hash_iter_al_final(iter)) return false;
//
//     iter->pos = buscar_proxima_posicion_ocupada(iter->hash, iter->pos+1);
//
//     return true;
// }
//
// //Post: Devuelve clave actual, esa clave no se puede modificar ni liberar.
// const char *hash_iter_ver_actual(const hash_iter_t *iter){
//     if(hash_iter_al_final(iter)) return NULL;
//     return iter->hash->datos[iter->pos].clave;
// }
//
// // Comprueba si terminó la iteración
// bool hash_iter_al_final(const hash_iter_t *iter){
//     return buscar_proxima_posicion_ocupada(iter->hash, iter->pos) == iter->hash->capacidad;
// }
//
// // Post: Destruye iterador
// void hash_iter_destruir(hash_iter_t* iter) {
//     free(iter);
// }
