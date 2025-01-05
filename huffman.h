#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256
#define MAX_TREE_HT 100

typedef struct nodo {
    unsigned char caracter;
    unsigned int frecuencia;
    struct nodo *izquierda, *derecha, *siguiente;
} Nodo;

typedef struct {
    Nodo *inicio;
    int tamanio;
} Lista;

// Funciones para la tabla de frecuencia
void inicializar_tabla_frecuencia(unsigned int tabla[]);
void llenar_tabla_frecuencia(unsigned char texto[], unsigned int tabla[]);
void imprimir_tabla_frecuencia(unsigned int tabla[]);

// Funciones para la lista enlazada
void crear_lista(Lista *lista);
void insertar_ordenado(Lista *lista, Nodo *nodo);
void llenar_lista(unsigned int tabla[], Lista *lista);
void imprimir_lista(Lista *lista);

// Funciones para el árbol de Huffman
Nodo *extraer_minimo(Lista *lista);
Nodo *construir_arbol_huffman(Lista *lista);
void imprimir_arbol(Nodo *raiz, int profundidad);

// Funciones para el diccionario
int altura_arbol(Nodo *raiz);
char **alocar_diccionario(int columnas);
void generar_diccionario(char **diccionario, Nodo *raiz, char *camino, int columnas);
void imprimir_diccionario(char **diccionario);

// Funciones de codificación y decodificación
char *codificar(char **diccionario, unsigned char *texto);
char *decodificar(unsigned char texto[], Nodo *raiz);

// Funciones de compresión y descompresión
void comprimir(const char *nombre_archivo, unsigned char *texto, char *codigo);
void descomprimir(const char *nombre_archivo, Nodo *raiz);

// Funciones auxiliares
int calcular_longitud_texto(char **diccionario, unsigned char *texto);
void archivos_txt(char nombre[MAX_CHAR]);
int es_txt(const char *nombre_archivo);

#endif

