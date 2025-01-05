#include "huffman.h"

void inicializar_tabla_frecuencia(unsigned int tabla[])
{
    for (int i = 0; i < MAX_CHAR; i++)
        tabla[i] = 0;
}

void llenar_tabla_frecuencia(unsigned char texto[], unsigned int tabla[])
{
    int i = 0;
    while (texto[i] != '\0')
    {
        tabla[texto[i]]++;
        i++;
    }
}

void imprimir_tabla_frecuencia(unsigned int tabla[])
{
    printf("\nTabla de frecuencia:\n");
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (tabla[i] > 0)
            printf("%c: %u\n", i, tabla[i]);
    }
}

void crear_lista(Lista *lista)
{
    lista->inicio = NULL;
    lista->tamanio = 0;
}

void insertar_ordenado(Lista *lista, Nodo *nodo)
{
    if (lista->inicio == NULL || nodo->frecuencia < lista->inicio->frecuencia)
    {
        nodo->siguiente = lista->inicio;
        lista->inicio = nodo;
    }
    else
    {
        Nodo *actual = lista->inicio;
        while (actual->siguiente && actual->siguiente->frecuencia <= nodo->frecuencia)
            actual = actual->siguiente;
        nodo->siguiente = actual->siguiente;
        actual->siguiente = nodo;
    }
    lista->tamanio++;
}

void llenar_lista(unsigned int tabla[], Lista *lista)
{
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (tabla[i] > 0)
        {
            Nodo *nuevo = malloc(sizeof(Nodo));
            if (nuevo)
            {
                nuevo->caracter = i;
                nuevo->frecuencia = tabla[i];
                nuevo->izquierda = nuevo->derecha = nuevo->siguiente = NULL;
                insertar_ordenado(lista, nuevo);
            }
            else
            {
                printf("Error al asignar memoria en llenar_lista\n");
                exit(1);
            }
        }
    }
}

void imprimir_lista(Lista *lista)
{
    printf("\nLista ordenada (tamaño: %d):\n", lista->tamanio);
    Nodo *actual = lista->inicio;
    while (actual)
    {
        printf("%c: %u\n", actual->caracter, actual->frecuencia);
        actual = actual->siguiente;
    }
}

Nodo *extraer_minimo(Lista *lista)
{
    if (lista->inicio)
    {
        Nodo *minimo = lista->inicio;
        lista->inicio = minimo->siguiente;
        minimo->siguiente = NULL;
        lista->tamanio--;
        return minimo;
    }
    return NULL;
}

Nodo *construir_arbol_huffman(Lista *lista)
{
    while (lista->tamanio > 1)
    {
        Nodo *izq = extraer_minimo(lista);
        Nodo *der = extraer_minimo(lista);
        Nodo *nuevo = malloc(sizeof(Nodo));
        if (nuevo)
        {
            nuevo->caracter = '+';
            nuevo->frecuencia = izq->frecuencia + der->frecuencia;
            nuevo->izquierda = izq;
            nuevo->derecha = der;
            nuevo->siguiente = NULL;
            insertar_ordenado(lista, nuevo);
        }
        else
        {
            printf("Error al asignar memoria en construir_arbol_huffman\n");
            exit(1);
        }
    }
    return lista->inicio;
}

void imprimir_arbol(Nodo *raiz, int profundidad)
{
    if (raiz)
    {
        for (int i = 0; i < profundidad; i++)
            printf("  ");
        if (raiz->izquierda == NULL && raiz->derecha == NULL)
            printf("Hoja: %c (Frecuencia: %u)\n", raiz->caracter, raiz->frecuencia);
        else
            printf("Nodo interno: + (Frecuencia: %u)\n", raiz->frecuencia);
        imprimir_arbol(raiz->izquierda, profundidad + 1);
        imprimir_arbol(raiz->derecha, profundidad + 1);
    }
}

int altura_arbol(Nodo *raiz)
{
    if (raiz == NULL)
        return -1;
    int izq = altura_arbol(raiz->izquierda);
    int der = altura_arbol(raiz->derecha);
    return (izq > der ? izq : der) + 1;
}

char **alocar_diccionario(int columnas)
{
    char **diccionario = malloc(sizeof(char *) * MAX_CHAR);
    for (int i = 0; i < MAX_CHAR; i++)
        diccionario[i] = calloc(columnas, sizeof(char));
    return diccionario;
}

void generar_diccionario(char **diccionario, Nodo *raiz, char *camino, int columnas)
{
    if (raiz->izquierda == NULL && raiz->derecha == NULL)
    {
        strcpy(diccionario[raiz->caracter], camino);
    }
    else
    {
        char izquierda[columnas], derecha[columnas];
        strcpy(izquierda, camino);
        strcpy(derecha, camino);
        strcat(izquierda, "0");
        strcat(derecha, "1");
        generar_diccionario(diccionario, raiz->izquierda, izquierda, columnas);
        generar_diccionario(diccionario, raiz->derecha, derecha, columnas);
    }
}

void imprimir_diccionario(char **diccionario)
{
    printf("\nDiccionario:\n");
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (strlen(diccionario[i]) > 0)
            printf("%c: %s\n", i, diccionario[i]);
    }
}

int calcular_longitud_texto(char **diccionario, unsigned char *texto)
{
    int i = 0, longitud = 0;
    while (texto[i] != '\0')
    {
        longitud += strlen(diccionario[texto[i]]);
        i++;
    }
    return longitud + 1;
}

char *codificar(char **diccionario, unsigned char *texto)
{
    int longitud = calcular_longitud_texto(diccionario, texto);
    char *codigo = calloc(longitud, sizeof(char));
    int i = 0;
    while (texto[i] != '\0')
    {
        strcat(codigo, diccionario[texto[i]]);
        i++;
    }
    return codigo;
}

char *decodificar(unsigned char texto[], Nodo *raiz)
{
    int i = 0;
    Nodo *actual = raiz;
    char temp[2];
    char *decodificado = calloc(strlen((char *)texto), sizeof(char));

    while (texto[i] != '\0')
    {
        if (texto[i] == '0')
            actual = actual->izquierda;
        else
            actual = actual->derecha;

        if (actual->izquierda == NULL && actual->derecha == NULL)
        {
            temp[0] = actual->caracter;
            temp[1] = '\0';
            strcat(decodificado, temp);
            actual = raiz;
        }
        i++;
    }
    return decodificado;
}

void comprimir(const char *nombre_archivo, unsigned char *texto, char *codigo)
{
    FILE *archivo = fopen(nombre_archivo, "wb");
    if (archivo)
    {
        int i = 0, j = 7;
        unsigned char mascara, byte = 0;

        while (codigo[i] != '\0')
        {
            mascara = 1;
            if (codigo[i] == '1')
            {
                mascara = mascara << j;
                byte = byte | mascara;
            }
            j--;

            if (j < 0)
            {
                fwrite(&byte, sizeof(unsigned char), 1, archivo);
                byte = 0;
                j = 7;
            }
            i++;
        }
        if (j != 7)
            fwrite(&byte, sizeof(unsigned char), 1, archivo);
        fclose(archivo);
        printf("Archivo comprimido guardado como '%s'\n", nombre_archivo);
    }
    else
    {
        printf("Error al abrir/crear archivo en comprimir\n");
    }
}

void descomprimir(const char *nombre_archivo, Nodo *raiz)
{
    const char *nombre_salida = "descomprimido.txt"; // Nombre fijo del archivo de salida
    FILE *archivo = fopen(nombre_archivo, "rb");
    FILE *salida = fopen("descomprimido.txt", "w");

    if (archivo && salida)
    {
        Nodo *actual = raiz;
        unsigned char byte;
        int i;

        printf("Descomprimiendo archivo...\n");
        while (fread(&byte, sizeof(unsigned char), 1, archivo))
        {
            for (i = 7; i >= 0; i--)
            {
                if (byte & (1 << i))
                    actual = actual->derecha;
                else
                    actual = actual->izquierda;

                // Si es una hoja (nodo con un carácter)
                if (actual->izquierda == NULL && actual->derecha == NULL)
                {
                    fputc(actual->caracter, salida); // Escribe en el archivo de salida
                    actual = raiz;                   // Reinicia en la raíz
                }
            }
        }

        printf("Archivo descomprimido correctamente en: %s\n", nombre_salida);

        fclose(archivo);
        fclose(salida);
    }
    else
    {
        if (!archivo)
            printf("Error al abrir archivo de entrada: %s\n", nombre_archivo);
        if (!salida)
            printf("Error al crear archivo de salida: %s\n", nombre_salida);
    }
}

void archivos_txt(char nombre[MAX_CHAR])
{
    FILE *fp;
    char path[MAX_CHAR];
    char txt_files[MAX_TREE_HT][MAX_CHAR];
    int countTXT = 0;

// Comando específico según el sistema operativo
#if defined(_WIN32) || defined(_WIN64)
    fp = popen("dir /b /a-d | findstr /i \".txt .pdf .wg\"", "r");
#elif defined(__linux__) || defined(__APPLE__) && defined(__MACH__)
    fp = popen("ls | grep -E '\\.(txt|wg|pdf)$'", "r");
#else
    printf("Sistema operativo no soportado para listar archivos.\n");
    exit(1);
#endif

    // Verificación de error al ejecutar el comando
    if (fp == NULL)
    {
        perror("Error al ejecutar el comando");
        exit(1);
    }

    // Leer y almacenar los nombres de archivos de imagen en un arreglo
    while (fgets(path, sizeof(path), fp) != NULL && countTXT < MAX_TREE_HT)
    {
        path[strcspn(path, "\n")] = 0; // Elimina el salto de línea
        if (es_txt(path))
        {
            strncpy(txt_files[countTXT], path, MAX_CHAR);
            countTXT++;
        }
    }
    pclose(fp);

    // Muestra la lista de archivos de imagen al usuario
    if (countTXT == 0)
    {
        printf("No se encontraron archivos de texto en el directorio.\n");
        exit(0);
    }

    printf("Archivos disponibles:\n");
    for (int i = 0; i < countTXT; i++)
    {
        printf("%d. %s\n", i + 1, txt_files[i]);
    }

    // Solicita al usuario que elija una imagen
    int choice;
    printf("\nElige el archivo que deseas usar: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > countTXT)
    {
        printf("ERROR.\n");
        exit(1);
    }

    // Guarda el nombre del archivo seleccionado en el arreglo `selected_image`
    strncpy(nombre, txt_files[choice - 1], MAX_CHAR);
}

// Función para verificar si el archivo es una imagen
int es_txt(const char *nombre_archivo)
{
    const char *extensiones[] = {".txt", ".wg"};
    for (int i = 0; i < sizeof(extensiones) / sizeof(extensiones[0]); i++)
    {
        if (strstr(nombre_archivo, extensiones[i]) != NULL)
        {
            return 1;
        }
    }
    return 0;
}
