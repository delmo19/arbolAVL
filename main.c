/* MIT License */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo {
    int dato;
    struct Nodo *izquierda;
    struct Nodo *derecha;
    int altura;
} Nodo;

// Prototipos de funciones
int altura(Nodo *n);
int max(int a, int b);
Nodo* crear_nodo(int dato);
Nodo* rotar_derecha(Nodo *y);
Nodo* rotar_izquierda(Nodo *x);
int balance_factor(Nodo *n);
Nodo* insertar(Nodo *nodo, int dato);
Nodo* nodo_minimo(Nodo *nodo);
Nodo* nodo_maximo(Nodo *nodo);
Nodo* eliminar(Nodo *raiz, int dato);
Nodo* buscar(Nodo *raiz, int dato);
void inorden(Nodo *raiz);
void preorden(Nodo *raiz);
void postorden(Nodo *raiz);
void vaciar_arbol(Nodo *raiz);
void imprimir_arbol_vertical(Nodo *raiz, int nivel, char *prefijo, int es_izquierdo);
void imprimir_arbol(Nodo *raiz);

// Implementación de funciones
int altura(Nodo *n) {
    return n ? n->altura : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Nodo* crear_nodo(int dato) {
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    nuevo->dato = dato;
    nuevo->izquierda = NULL;
    nuevo->derecha = NULL;
    nuevo->altura = 1;
    return nuevo;
}

Nodo* rotar_derecha(Nodo *y) {
    Nodo *x = y->izquierda;
    Nodo *T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    y->altura = max(altura(y->izquierda), altura(y->derecha)) + 1;
    x->altura = max(altura(x->izquierda), altura(x->derecha)) + 1;

    return x;
}

Nodo* rotar_izquierda(Nodo *x) {
    Nodo *y = x->derecha;
    Nodo *T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    x->altura = max(altura(x->izquierda), altura(x->derecha)) + 1;
    y->altura = max(altura(y->izquierda), altura(y->derecha)) + 1;

    return y;
}

int balance_factor(Nodo *n) {
    return n ? altura(n->izquierda) - altura(n->derecha) : 0;
}

Nodo* insertar(Nodo *nodo, int dato) {
    if (!nodo) return crear_nodo(dato);

    if (dato < nodo->dato)
        nodo->izquierda = insertar(nodo->izquierda, dato);
    else if (dato > nodo->dato)
        nodo->derecha = insertar(nodo->derecha, dato);
    else
        return nodo;

    nodo->altura = 1 + max(altura(nodo->izquierda), altura(nodo->derecha));

    int balance = balance_factor(nodo);

    // Rotación simple derecha
    if (balance > 1 && dato < nodo->izquierda->dato)
        return rotar_derecha(nodo);

    // Rotación simple izquierda
    if (balance < -1 && dato > nodo->derecha->dato)
        return rotar_izquierda(nodo);

    // Rotación doble izquierda-derecha
    if (balance > 1 && dato > nodo->izquierda->dato) {
        nodo->izquierda = rotar_izquierda(nodo->izquierda);
        return rotar_derecha(nodo);
    }

    // Rotación doble derecha-izquierda
    if (balance < -1 && dato < nodo->derecha->dato) {
        nodo->derecha = rotar_derecha(nodo->derecha);
        return rotar_izquierda(nodo);
    }

    return nodo;
}

Nodo* nodo_minimo(Nodo *nodo) {
    Nodo *actual = nodo;
    while (actual && actual->izquierda)
        actual = actual->izquierda;
    return actual;
}

Nodo* nodo_maximo(Nodo *nodo) {
    Nodo *actual = nodo;
    while (actual && actual->derecha)
        actual = actual->derecha;
    return actual;
}

Nodo* eliminar(Nodo *raiz, int dato) {
    if (!raiz) return raiz;

    if (dato < raiz->dato)
        raiz->izquierda = eliminar(raiz->izquierda, dato);
    else if (dato > raiz->dato)
        raiz->derecha = eliminar(raiz->derecha, dato);
    else {
        // Nodo con un hijo o sin hijos
        if (!raiz->izquierda || !raiz->derecha) {
            Nodo *temp = raiz->izquierda ? raiz->izquierda : raiz->derecha;

            // Sin hijos
            if (!temp) {
                temp = raiz;
                raiz = NULL;
            } 
            // Un hijo
            else {
                *raiz = *temp;
            }
            free(temp);
        } 
        // Dos hijos
        else {
            // Mayor de los menores (máximo en subárbol izquierdo)
            Nodo *temp = nodo_maximo(raiz->izquierda);
            raiz->dato = temp->dato;
            raiz->izquierda = eliminar(raiz->izquierda, temp->dato);
        }
    }

    if (!raiz) return raiz;

    raiz->altura = 1 + max(altura(raiz->izquierda), altura(raiz->derecha));

    int balance = balance_factor(raiz);

    // Rotaciones para balancear
    if (balance > 1) {
        if (balance_factor(raiz->izquierda) >= 0)
            return rotar_derecha(raiz);
        else {
            raiz->izquierda = rotar_izquierda(raiz->izquierda);
            return rotar_derecha(raiz);
        }
    }
    if (balance < -1) {
        if (balance_factor(raiz->derecha) <= 0)
            return rotar_izquierda(raiz);
        else {
            raiz->derecha = rotar_derecha(raiz->derecha);
            return rotar_izquierda(raiz);
        }
    }

    return raiz;
}

Nodo* buscar(Nodo *raiz, int dato) {
    if (!raiz) return NULL;
    if (dato < raiz->dato) return buscar(raiz->izquierda, dato);
    if (dato > raiz->dato) return buscar(raiz->derecha, dato);
    return raiz;
}

void inorden(Nodo *raiz) {
    if (raiz) {
        inorden(raiz->izquierda);
        printf("%d ", raiz->dato);
        inorden(raiz->derecha);
    }
}

void preorden(Nodo *raiz) {
    if (raiz) {
        printf("%d ", raiz->dato);
        preorden(raiz->izquierda);
        preorden(raiz->derecha);
    }
}

void postorden(Nodo *raiz) {
    if (raiz) {
        postorden(raiz->izquierda);
        postorden(raiz->derecha);
        printf("%d ", raiz->dato);
    }
}

void vaciar_arbol(Nodo *raiz) {
    if (raiz) {
        vaciar_arbol(raiz->izquierda);
        vaciar_arbol(raiz->derecha);
        free(raiz);
    }
}

void imprimir_arbol_vertical(Nodo *raiz, int nivel, char *prefijo, int es_izquierdo) {
    if (!raiz) return;

    // Reservar espacio para el nuevo prefijo
    char nuevo_prefijo[256];
    strcpy(nuevo_prefijo, prefijo);
    strcat(nuevo_prefijo, es_izquierdo ? "│   " : "    ");

    // Imprimir subárbol derecho (primero)
    imprimir_arbol_vertical(raiz->derecha, nivel + 1, nuevo_prefijo, 0);

    // Imprimir el nodo actual (con factor de equilibrio)
    printf("%s", prefijo);
    printf("%s", es_izquierdo ? "└── " : "┌── ");
    printf("%d (%d)\n", raiz->dato, balance_factor(raiz)); // Modificado

    // Actualizar prefijo para subárbol izquierdo
    strcpy(nuevo_prefijo, prefijo);
    strcat(nuevo_prefijo, es_izquierdo ? "    " : "│   ");

    // Imprimir subárbol izquierdo
    imprimir_arbol_vertical(raiz->izquierda, nivel + 1, nuevo_prefijo, 1);
}

void imprimir_arbol(Nodo *raiz) {
    if (raiz) {
        printf("Estructura del árbol AVL:\n");
        imprimir_arbol_vertical(raiz, 0, "", 0);
    } else {
        printf("El árbol está vacío.\n");
    }
}

int main(int argc, char *argv[]) {
    Nodo *raiz = NULL;
    int opcion, dato;
    
    // Insertar números pasados como argumentos
    for (int i = 1; i < argc; i++) {
        raiz = insertar(raiz, atoi(argv[i]));
    }

    do {
        printf("\nMenú Árbol AVL:\n");
        printf("1. Insertar elemento\n");
        printf("2. Eliminar elemento\n");
        printf("3. Buscar elemento\n");
        printf("4. Recorridos (Inorden, Preorden, Postorden)\n");
        printf("5. Mostrar altura del árbol\n");
        printf("6. Impresión de conjuntos (Inorden)\n");
        printf("7. Impresión jerárquica\n");
        printf("8. Vaciar árbol\n");
        printf("9. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Ingrese el número a insertar: ");
                scanf("%d", &dato);
                raiz = insertar(raiz, dato);
                break;
            case 2:
                printf("Ingrese el número a eliminar: ");
                scanf("%d", &dato);
                raiz = eliminar(raiz, dato);
                break;
            case 3:
                printf("Ingrese el número a buscar: ");
                scanf("%d", &dato);
                Nodo *encontrado = buscar(raiz, dato);
                printf(encontrado ? "Elemento encontrado\n" : "Elemento no encontrado\n");
                break;
            case 4:
                printf("Inorden: ");
                inorden(raiz);
                printf("\nPreorden: ");
                preorden(raiz);
                printf("\nPostorden: ");
                postorden(raiz);
                printf("\n");
                break;
            case 5:
                printf("Altura del árbol: %d\n", altura(raiz));
                break;
            case 6:
                printf("Elementos en orden (Inorden): ");
                inorden(raiz);
                printf("\n");
                break;
            case 7:
                imprimir_arbol(raiz);
                break;
            case 8:
                vaciar_arbol(raiz);
                raiz = NULL;
                printf("Árbol vaciado\n");
                break;
            case 9:
                vaciar_arbol(raiz);
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción inválida\n");
        }
    } while (opcion != 9);

    return 0;
}
