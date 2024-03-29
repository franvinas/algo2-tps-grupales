#include "heap.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RAND_TOPE 1000

/* ******************************************************************
 *                   				AUXILIARES
 * *****************************************************************/

 int cmp(const void* a, const void* b) {
	 const int* x = a;
	 const int* y = b;
	 return *x - *y;
 }

bool esta_ordenado(int** vector, size_t tam) {
  for(size_t i = 1; i < tam; i++) {
    if(*vector[i] < *vector[i - 1]) return false;
  }
  return true;
}

void imprimir_vector(int** vector, size_t tam) {
  for(size_t i = 0; i < tam; i++) {
    printf("%i ", *vector[i]);
  }
  printf("\n");
}

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void PruebaDestruirheapVacia(){
	heap_t* heap = heap_crear(cmp);
	print_test("Se creo el heap", heap != NULL);
	print_test("heap esta vacia", heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
	print_test("El heap fue destruido",true);
}

void PruebaheapUnElemento(){
	heap_t* heap = heap_crear(cmp);
	print_test("Se creo el heap", heap != NULL);

	int valor = 5;
	int* pvalor = &valor;

	print_test("Encolo un elemento" , heap_encolar(heap,pvalor));
	print_test("heap ya no esta vacia", !heap_esta_vacio(heap));
	print_test("Tope de el heap es lo apuntado por pvalor" , heap_ver_max(heap) == pvalor);
	print_test("Desencolo un elemento" , heap_desencolar(heap));
	print_test("Heap vacio", heap_esta_vacio(heap));
	print_test("Tope del heap es NULL" , heap_ver_max(heap) == NULL);
	heap_destruir(heap, NULL);
	print_test("El heap fue destruido",true);
}


void PruebaheapVariosElementos(){
	heap_t* heap = heap_crear(cmp);
	print_test("Se creo el heap", heap != NULL);

	int valor1 = 10;
	int valor2 = 4;
	int valor3 = 61;
	int valor4 = 8;

	print_test("Encolo un elemento" , heap_encolar(heap, &valor1));
	print_test("El maximo del heap es el que se encolo" , *(int*) heap_ver_max(heap) == valor1);

	print_test("Encolo un elemento" , heap_encolar(heap, &valor2));
	print_test("El maximo del heap es el correcto" , *(int*) heap_ver_max(heap) == valor1);

	print_test("Encolo un elemento" , heap_encolar(heap, &valor3));
	print_test("El maximo del heap es el correcto" , *(int*) heap_ver_max(heap) == valor3);

	print_test("Encolo un elemento" , heap_encolar(heap,&valor4));

	print_test("Desencolo el elemento correcto" , valor3 == *(int*) heap_desencolar(heap));

	print_test("El maximo del heap es el correcto" , *(int*) heap_ver_max(heap) == valor1);

	print_test("Encolo un elemento" , heap_encolar(heap, &valor1));
	print_test("Encolo un elemento" , heap_encolar(heap, &valor1));
	print_test("Encolo un elemento" , heap_encolar(heap, &valor1));
	print_test("Encolo un elemento" , heap_encolar(heap, &valor3));
	print_test("Encolo un elemento" , heap_encolar(heap, &valor1));
	print_test("Encolo un elemento" , heap_encolar(heap, &valor1));
	print_test("Encolo un elemento" , heap_encolar(heap, &valor1));
	print_test("El maximo del heap es el correcto" , *(int*) heap_ver_max(heap) == valor3);
	print_test("Encolo un elemento" , heap_encolar(heap, &valor1));
	print_test("El maximo del heap es el correcto" , *(int*) heap_ver_max(heap) == valor3);

	print_test("Desencolo el elemento correcto" , valor3 == *(int*) heap_desencolar(heap));

	heap_destruir(heap, NULL);
	print_test("el heap fue destruida",true);


}

void PruebaVolumen(int tam){//estaria bueno recibir el volumen por parametro para poder probar varios volumenes
    heap_t* heap = heap_crear(cmp);
    print_test("Se creo el heap y encolo muchos elementos", heap != NULL);

    time_t t;
    srand((unsigned) time(&t));

    int max = 0;
    int vector_prueba_volumen[tam];

    for (int i = 0; i < tam; ++i) {
        vector_prueba_volumen[i] = rand() % RAND_TOPE;
        heap_encolar(heap,&vector_prueba_volumen[i]);//no es interesante encolar en orden los elementos
        max = vector_prueba_volumen[i] > max ? vector_prueba_volumen[i] : max;
    }


    print_test("El nuevo tope de el heap es " ,*(int*) heap_ver_max(heap) == max);

    bool desencolar_ok = true;
    int actual;
    int anterior = RAND_TOPE;
    for (int j = tam -1; j >= 0 ; j--) {
        actual = *(int*) heap_desencolar(heap);
        if(actual > anterior) desencolar_ok = false;
		}

    print_test("Se desencola correctamente", desencolar_ok);
    print_test("el heap esta vacia", heap_esta_vacio(heap));
    heap_destruir(heap, NULL);
    print_test("el heap fue destruida",true);
}

void pruebas_heap_sort() {

		int valor1 = 9;
		int valor2 = 7;
		int valor3 = 5;
		int valor4 = 17;
		int valor5 = 19;
		int valor6 = 11;

		int* vector[6];
		vector[0] = &valor1;
		vector[1] = &valor2;
		vector[2] = &valor3;
		vector[3] = &valor4;
		vector[4] = &valor5;
		vector[5] = &valor6;
    printf("Vector original: ");
    imprimir_vector(vector, 6);
		heap_sort( (void**) vector, 6, cmp);
    printf("Vector ordenado: ");
    imprimir_vector(vector, 6);

		print_test("La funcion heap_sort ordena el vector correctamente", esta_ordenado(vector, 6));//estaria bueno alguna prueba de heapsort de volumen
}

void pruebas_funcion_destruccion() {
  heap_t* heap = heap_crear(cmp);

  int* valor1 = malloc(sizeof(int));
  int* valor2 = malloc(sizeof(int));
  int* valor3 = malloc(sizeof(int));
  int* valor4 = malloc(sizeof(int));
  *valor1 = 14;
  *valor2 = 52;
  *valor3 = 93;
  *valor4 = 70;

  printf("Se encolan elementos pedidos con malloc\n");
  heap_encolar(heap, valor1);
  heap_encolar(heap, valor2);
  heap_encolar(heap, valor3);
  heap_encolar(heap, valor4);

  printf("Se destruye el heap con la funcion de destruccion free\n");
  heap_destruir(heap, free);
}

void pruebas_heap_alumno() {

	printf("INICIO PRUEBAS DESTRUIR HEAP\n");
	PruebaDestruirheapVacia();

	printf("\nINICIO PRUEBAS HEAP CON UN ELEMENTO\n");
	PruebaheapUnElemento();

	printf("\nINICIO PRUEBAS HEAP CON VARIOS ELEMENTOS\n");
  PruebaheapVariosElementos();

  printf("\nINICIO PRUEBAS DE VOLUMEN HEAP\n");
  PruebaVolumen(500);
  PruebaVolumen(5000);

	printf("\nINICIO PRUEBAS DE HEAP_SORT\n");
	pruebas_heap_sort();

  printf("\nINICIO DE PRUEBAS CON FUNCION DE DESTRUCCION PROPIA\n");
  pruebas_funcion_destruccion();
}
//faltan pruebas con funcion de destruccion propia
