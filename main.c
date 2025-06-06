/* MIT License */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo {
    int dato;
    int fe;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;

Nodo* crear_nodo(int dato) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    nuevo->dato = dato;
    nuevo->fe = 0;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    return nuevo;
}

int altura(Nodo* raiz) {
    if (raiz == NULL) return 0;
    int alt_izq = altura(raiz->izq);
    int alt_der = altura(raiz->der);
    return 1 + (alt_izq > alt_der ? alt_izq : alt_der);
}

void actualizar_fe(Nodo* raiz) {
    if (raiz != NULL) {
        raiz->fe = altura(raiz->der) - altura(raiz->izq);
    }
}

Nodo* rotar_izq(Nodo* raiz) {
    Nodo* nueva_raiz = raiz->der;
    raiz->der = nueva_raiz->izq;
    nueva_raiz->izq = raiz;
    actualizar_fe(raiz);
    actualizar_fe(nueva_raiz);
    return nueva_raiz;
}

Nodo* rotar_der(Nodo* raiz) {
    Nodo* nueva_raiz = raiz->izq;
    raiz->izq = nueva_raiz->der;
    nueva_raiz->der = raiz;
    actualizar_fe(raiz);
    actualizar_fe(nueva_raiz);
    return nueva_raiz;
}

Nodo* rotar_der_izq(Nodo* raiz) {
    raiz->der = rotar_der(raiz->der);
    return rotar_izq(raiz);
}

Nodo* rotar_izq_der(Nodo* raiz) {
    raiz->izq = rotar_izq(raiz->izq);
    return rotar_der(raiz);
}

Nodo* balancear(Nodo* raiz) {
    if (raiz == NULL) return NULL;
    
    actualizar_fe(raiz);
    
    if (raiz->fe == -2) {
        if (raiz->izq->fe <= 0) {
            return rotar_der(raiz);
        } else {
            return rotar_izq_der(raiz);
        }
    } else if (raiz->fe == 2) {
        if (raiz->der->fe >= 0) {
            return rotar_izq(raiz);
        } else {
            return rotar_der_izq(raiz);
        }
    }
    return raiz;
}

Nodo* insertar(Nodo* raiz, int dato) {
    if (raiz == NULL) return crear_nodo(dato);
    
    if (dato < raiz->dato) {
        raiz->izq = insertar(raiz->izq, dato);
    } else if (dato > raiz->dato) {
        raiz->der = insertar(raiz->der, dato);
    } else {
        return raiz;
    }
    
    raiz = balancear(raiz);
    return raiz;
}

Nodo* encontrar_min(Nodo* raiz) {
    if (raiz == NULL) return NULL;
    if (raiz->izq == NULL) return raiz;
    return encontrar_min(raiz->izq);
}

Nodo* eliminar(Nodo* raiz, int dato) {
    if (raiz == NULL) return NULL;
    
    if (dato < raiz->dato) {
        raiz->izq = eliminar(raiz->izq, dato);
    } else if (dato > raiz->dato) {
        raiz->der = eliminar(raiz->der, dato);
    } else {
        if (raiz->izq == NULL || raiz->der == NULL) {
            Nodo* temp = raiz->izq ? raiz->izq : raiz->der;
            if (temp == NULL) {
                free(raiz);
                return NULL;
            } else {
                *raiz = *temp;
                free(temp);
            }
        } else {
            Nodo* sucesor = encontrar_min(raiz->der);
            raiz->dato = sucesor->dato;
            raiz->der = eliminar(raiz->der, sucesor->dato);
        }
    }
    
    return balancear(raiz);
}

void imprimir_conjuntos(Nodo* raiz) {
    if (raiz == NULL) return;
    
    printf("%d", raiz->dato);
    
    if (raiz->izq != NULL || raiz->der != NULL) {
        printf("(");
        if (raiz->izq != NULL) {
            imprimir_conjuntos(raiz->izq);
        }
        if (raiz->der != NULL) {
            printf(",");
            imprimir_conjuntos(raiz->der);
        }
        printf(")");
    }
}

void imprimir_nivel(Nodo* raiz, int nivel) {
    if (raiz == NULL) return;
    if (nivel == 1) {
        printf("%d\n", raiz->dato);
    } else if (nivel > 1) {
        imprimir_nivel(raiz->izq, nivel - 1);
        imprimir_nivel(raiz->der, nivel - 1);
    }
}

void imprimir_jerarquica(Nodo* raiz) {
    if (raiz == NULL) return;
    
    int h = altura(raiz);
    for (int i = 1; i <= h; i++) {
        imprimir_nivel(raiz, i);
    }
}

void preorden(Nodo* raiz) {
    if (raiz == NULL) return;
    printf("%d ", raiz->dato);
    preorden(raiz->izq);
    preorden(raiz->der);
}

void inorden(Nodo* raiz) {
    if (raiz == NULL) return;
    inorden(raiz->izq);
    printf("%d ", raiz->dato);
    inorden(raiz->der);
}

void postorden(Nodo* raiz) {
    if (raiz == NULL) return;
    postorden(raiz->izq);
    postorden(raiz->der);
    printf("%d ", raiz->dato);
}

Nodo* buscar(Nodo* raiz, int dato) {
    if (raiz == NULL) return NULL;
    if (dato == raiz->dato) return raiz;
    if (dato < raiz->dato) return buscar(raiz->izq, dato);
    return buscar(raiz->der, dato);
}

void vaciar_arbol(Nodo* raiz) {
    if (raiz != NULL) {
        vaciar_arbol(raiz->izq);
        vaciar_arbol(raiz->der);
        free(raiz);
    }
}

int main(int argc, char *argv[]) {
    Nodo *raiz = NULL;
    Nodo *temp = NULL;
    int opcion, dato, h;
    
    for (int i = 1; i < argc; i++) {
        raiz = insertar(raiz, atoi(argv[i]));
    }

    do {
        printf("\nMENU\n");
        printf("1. Insertar.\n");
        printf("2. Eliminar.\n");
        printf("3. Imprimir.\n");
        printf("4. Recorridos.\n");
        printf("5. Altura.\n");
        printf("6. Busqueda.\n");
        printf("7. Salir\n\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        
        switch(opcion) {
            case 1:
                printf("Dato a insertar: ");
                scanf("%d", &dato);
                raiz = insertar(raiz, dato);
                break;
                
            case 2:
                printf("Dato a eliminar: ");
                scanf("%d", &dato);
                raiz = eliminar(raiz, dato);
                break;
                
            case 3:
                printf("Impresion de conjuntos: ");
                imprimir_conjuntos(raiz);
                printf("\n\nImpresion de arbol jerarquico:\n");
                imprimir_jerarquica(raiz);
                printf("\n");
                break;
                
            case 4:
                printf("Preorden: ");
                preorden(raiz);
                printf("\nInorden: ");
                inorden(raiz);
                printf("\nPostorden: ");
                postorden(raiz);
                printf("\n");
                break;
                
            case 5:
                h = altura(raiz);
                printf("Altura del arbol: %d\n", h);
                break;
                
            case 6:
                printf("Dato a buscar: ");
                scanf("%d", &dato);
                temp = buscar(raiz, dato);
                if (temp) {
                    printf("Nodo encontrado: %d\n", temp->dato);
                } else {
                    printf("Nodo no encontrado\n");
                }
                break;
                
            case 7:
                vaciar_arbol(raiz);
                raiz = NULL;
                break;
                
            default:
                printf("Opcion invalida\n");
        }
    } while(opcion != 7);
    
    return 0;
}
