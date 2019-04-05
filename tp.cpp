#include "tp.h"
/*
Inicializa un nodo de lista doblemente enlazada.
*/
void Nodo::create() {
    this->caracter = malloc(sizeof(char));
    this->anterior = NULL;
    this->siguiente = NULL;
}

/*
Destruye el nodo de lista doblemente enlazada.
*/
void Nodo::destroy() {
    free(this->caracter);
}

/*
Inicializa una lista doblemente enlazada.
*/
void ListaDoble::create() {
    this->primero = NULL;
}
/*
Destruye la lista doblemente enlazada.
*/
void ListaDoble::destroy() {
	char* actual = this->primero;
	char* siguiente = NULL;
	while (actual != NULL) {
		siguiente = actual->siguiente;
		delete[] actual;
		actual = siguiente;
	}
}

bool ListaDoble::insertar_primero(char caracter) {
    Nodo *nuevoNodo = malloc(sizeof(Nodo));
    if (!nuevoNodo) {
        return false;
    }
    nuevoNodo->carater = new Nodo() //malloc(sizeof(char));
    /*
    if (!nuevoNodo->caracter) {
        free(nuevoNodo);
        return false;
    }
    */
    nuevoNodo->caracter = caracter;
    nuevoNodo->siguiente = this->primero;
    nuevoNodo->anterior = NULL;
    this->primero = nuevoNodo;
    return true;

}

/*
Inicializa un iterador de lista doblemente enlazada.
PRE: Recibe un puntero a una lista doblemente enlazada.
POST: Si la lista esta vacia, inserta un caracter nulo
en la misma.
*/
void IteradorListaDoble::create(ListaDoble *lista) {
    if (lista->primero == NULL) {
        bool seInserto = lista.insertar_primero('\0');
        if (!seInserto) {
            return false;
        }
    }
    this->actual = lista->primero;
    //this->anterior = NULL;
}

/*
Destruye un iterador de lista doblemente enlazada.
*/
void IteradorListaDoble::destroy() {
    // No hace nada
}
/*

Nos indica si el iterador esta al final de la lista 
doblemente enlazada.
PRE: No recibe nada.
POST: Devuelve true si esta al final de la lista 
doblemente enlazada, false en caso contrario. 

bool IteradorListaDoble::esta_al_final() {
    return this->actual->siguiente == NULL;
}
*/

/*
Avanza al siguiente nodo de la lista doblemente enlazada.
PRE: No recibe nada.
POST: Devuelve true si se logro avanzar al siguiente nodo,
false en caso contrario, dado que ya se encuentra al final.
*/
bool IteradorListaDoble::avanzar() {
    if (this->actual->siguiente == NULL) {
        return false;
    }
    this->actual = this->actual->siguente;
}

/*
Retrocede al anterior nodo de la lista doblemente enlazada.
PRE: No recibe nada.
Post: Devuelve true si logro retroceder al anterior nodo, 
false en caso contrario.
*/
bool IteradorListaDoble::retroceder() {
    if (this->actual->anterior == NULL) {
        return false
    }
    this->actual = this->actual->anterior;
}
