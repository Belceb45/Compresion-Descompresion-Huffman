#include "huffman.h"

int main() {
    char nombre_archivo[MAX_CHAR];
    archivos_txt(nombre_archivo);

    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    fseek(archivo, 0, SEEK_END);
    long tamanio = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    unsigned char *texto = malloc(tamanio + 1);
    if (!texto) {
        printf("Error al asignar memoria para el texto.\n");
        fclose(archivo);
        return 1;
    }

    fread(texto, 1, tamanio, archivo);
    texto[tamanio] = '\0';
    fclose(archivo);

    unsigned int tabla_frecuencia[MAX_CHAR];
    inicializar_tabla_frecuencia(tabla_frecuencia);
    llenar_tabla_frecuencia(texto, tabla_frecuencia);
    imprimir_tabla_frecuencia(tabla_frecuencia);

    Lista lista;
    crear_lista(&lista);
    llenar_lista(tabla_frecuencia, &lista);
    imprimir_lista(&lista);

    Nodo *arbol = construir_arbol_huffman(&lista);
    printf("\nArbol de Huffman:\n");
    imprimir_arbol(arbol, 0);

    int columnas = altura_arbol(arbol) + 1;
    char **diccionario = alocar_diccionario(columnas);
    generar_diccionario(diccionario, arbol, "", columnas);
    imprimir_diccionario(diccionario);

    char *codificado = codificar(diccionario, texto);
    printf("\nTexto codificado: %s\n", codificado);

    char *decodificado = decodificar((unsigned char *)codificado, arbol);
    printf("\nTexto decodificado: %s\n", decodificado);

    comprimir("comprimido.txt", texto, codificado);
    printf("\nArchivo comprimido. Descomprimiendo:\n");
    descomprimir("comprimido.txt", arbol);

    // Liberar memoria
    free(texto);
    free(codificado);
    free(decodificado);
    for (int i = 0; i < MAX_CHAR; i++)
        free(diccionario[i]);
    free(diccionario);

    return 0;
}

