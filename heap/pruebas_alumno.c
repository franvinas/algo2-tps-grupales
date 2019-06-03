#include "heap.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>

#define CONSTANTE_PRUEBA_VOLUMEN 50 //50 no es volumen suficiente



/* ******************************************************************
 *                   				AUXILIARES
 * *****************************************************************/

 int cmp(const void* a, const void* b) {
	 const int* x = a;
	 const int* y = b;
	 return *x - *y;
 }

//borren las pruebas comentadas que no usen

// bool cumple_condicion_heap(void** vector, size_t tam, size_t pos) {
// 	bool condicion_padre;
// 	bool condicion_hijo_izq;
// 	bool condicion_hijo_der;
// 	size_t hijo_izq = 2 * pos + 1;
// 	size_t hijo_der = 2 * pos + 2;
// 	if(pos == 0)
// 		condicion_padre = true;
// 	else {
// 		size_t padre = (pos - 1) / 2;
// 		condicion_padre = *(int*) vector[padre] > *(int*) vector[pos];
// 	}
//
// 	if(hijo_izq >= tam) {
// 		condicion_hijo_izq = true;
// 		condicion_hijo_der = true;
// 	} else {
// 		condicion_hijo_izq = *(int*) vector[pos] > *(int*) vector[hijo_izq];
// 		if(hijo_der >= tam)
// 			condicion_hijo_der = true;
// 		else
// 			condicion_hijo_der = *(int*) vector[pos] > *(int*) vector[hijo_der];
// 	}
// 	return condicion_padre && condicion_hijo_der && condicion_hijo_izq;
//
// }
//
// bool es_heap(void** vector, size_t n) {
// 	for(size_t i = 0; i < n; i++) {
// 		if(!cumple_condicion_heap(vector, n, i)) return false;
// 	}
// 	return true;
// }

bool esta_ordenado(int** vector, size_t tam) {
  for(size_t i = 1; i < tam; i++) {
    if(*vector[i] < *vector[i - 1]) return false;
  }
  return true;
}

// void imprimir_vector(int** vector, size_t tam) {
//   for(size_t i = 0; i < tam; i++) {
//     printf("%i ", *vector[i]);
//   }
//   printf("\n");
// }

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
	print_test("heap vacia", heap_esta_vacio(heap));
	print_test("Tope de el heap es NULL" , heap_ver_max(heap) == NULL);
	heap_destruir(heap, NULL);
	print_test("el heap fue destruida",true);
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

void PruebaVolumen(){//estaria bueno recibir el volumen por parametro para poder probar varios volumenes
    heap_t* heap = heap_crear(cmp);
    print_test("Se creo el heap", heap != NULL);


    int vector_prueba_volumen[CONSTANTE_PRUEBA_VOLUMEN];

    for (int i = 0; i < CONSTANTE_PRUEBA_VOLUMEN; ++i) {
        vector_prueba_volumen[i] = i;
        heap_encolar(heap,&vector_prueba_volumen[i]);//no es interesante encolar en orden los elementos
    }


    print_test("El nuevo tope de el heap es " , heap_ver_max(heap) == &vector_prueba_volumen[CONSTANTE_PRUEBA_VOLUMEN - 1]);


    int* valorAdesencolar;
    for (int j = CONSTANTE_PRUEBA_VOLUMEN -1; j >= 0 ; j--) {
        valorAdesencolar = heap_desencolar(heap);
        print_test("Se desencola el valor correcto", *valorAdesencolar == vector_prueba_volumen[j]);
		}

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
		heap_sort( (void**) vector, 6, cmp);


		print_test("La funcion heap_sort ordena el vector correctamente", esta_ordenado(vector, 6));//estaria bueno alguna prueba de heapsort de volumen

}

void pruebas_heap_alumno() {

	printf("INICIO PRUEBAS DESTRUIR HEAP\n");
	PruebaDestruirheapVacia();

	printf("\nINICIO PRUEBAS HEAP CON UN ELEMENTO\n");
	PruebaheapUnElemento();

	printf("\nINICIO PRUEBAS HEAP CON VARIOS ELEMENTOS\n");
	PruebaheapVariosElementos();

	printf("\nINICIO PRUEBAS DE VOLUMEN HEAP\n");
	PruebaVolumen();

	printf("\nINICIO PRUEBAS DE HEAP_SORT\n");
	pruebas_heap_sort();
}
//faltan pruebas con funcion de destruccion propia
