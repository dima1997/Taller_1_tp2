#include <iostream>
#include <fstream>
#include "tp.h"
/*
Inicializa un nodo de lista doblemente enlazada.
*/
Nodo::Nodo() {
    this->caracter = new char[1]();
    this->anterior = NULL;
    this->siguiente = NULL;
}

/*
Destruye el nodo de lista doblemente enlazada.
*/
Nodo::~Nodo() {
    delete [] this->caracter;
}

/*
Inicializa una lista doblemente enlazada.
*/
ListaDoble::ListaDoble() {
    this->primero = NULL;
}
/*
Destruye la lista doblemente enlazada.
*/
ListaDoble::~ListaDoble() {
	char* actual = this->primero;
	char* siguiente = NULL;
	while (actual != NULL) {
		siguiente = actual->siguiente;
		delete[] actual;
		actual = siguiente;
	}
}

/*
Inserta un caracter en la primer posicion.
PRE: Recibe un caracter (char).
POST: devuelve true si logro insertar el caracter
en la primera posicion con exito, false en caso
contrario.
*/
void ListaDoble::insertar_primero(char caracter) {
    Nodo *nuevoNodo->carater = new Nodo();
    nuevoNodo->caracter = caracter;
    nuevoNodo->siguiente = this->primero;
    nuevoNodo->anterior = NULL;
    this->primero = nuevoNodo;
}

/*
Inicializa un iterador de lista doblemente enlazada.
PRE: Recibe un puntero a una lista doblemente enlazada,
con al menos un elemento en ella.
POST: Si la lista esta vacia, inserta un caracter nulo
en la misma.
*/
IteradorListaDoble::IteradorListaDoble(ListaDoble *lista) {
    this->actual = lista->primero;
}

/*
Destruye un iterador de lista doblemente enlazada.
*/
IteradorListaDoble::~IteradorListaDoble() {
    // No hace nada
}

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
    return true;
}

/*
Retrocede al anterior nodo de la lista doblemente enlazada.
PRE: No recibe nada.
Post: Devuelve true si logro retroceder al anterior nodo, 
false en caso contrario.
*/
bool IteradorListaDoble::retroceder() {
    if (this->actual->anterior == NULL) {
        return false;
    }
    this->actual = this->actual->anterior;
    return true;
}

/*
Agrega un caracter en la posicion siguiente a la actual.
PRE: Recibe un caracter (char).
POST: Devuelve true si logro insertar el caracter con exito
en la siguiente posicion. 
El iterador se mantiene en la posicion actual, sin moverse al 
nuevo caracter insertado.
*/
bool IteradorListaDoble::insertar_siguiente(char caracter) {
    Nodo *nuevoNodo = new Nodo();
    nuevoNodo->caracter = caracter;
    Nodo *siguiente = this->actual->siguiente;
    nuevoNodo->siguiente = siguiente;
    if (siguiente) {
        siguiente->anterior = nuevoNodo;
    }
    nuevoNodo->anterior = this->actual;
    this->actual->siguiente = nuevoNodo;
    return true;
}

/*
Escribe uno nuevo caracter sobre la posicion actual.
PRE: Recibe un caracter (char).
POST: Reemplaza el caracter en la posicion actual del 
iterador y por el recibido. 
*/
void IteradorListaDoble::escribir(char caracter) {
    *(this->actual->caracter) = caracter;
}

/*
Lee el caracter en la posicion actual.
PRE: Recibe un puntero a char donde guardar el 
valor leido.
POST: Guarda en la posicion de memoria recibida 
el caracter leido.
*/
void IteradorListaDoble::leer(char *caracter) {
    *caracter = *(this->actual->caracter);
}

InterpreteBrainFuck::InterpreteBrainFuck() {
    //No hace nada
}

InterpreteBrainFuck::~InterpreteBrainFuck() {
    //No hace nada
}

bool InterpreteBrainFuck::procesar_script(char *rutaScript) {
    ListaDoble datos();
    datos.insertar_primero('\0');
    IteradorListaDoble iterador(datos);
    bool hayErrorDeIntepretacion = false;
    fstream ficheroScript;
    ficheroScript.open(rutaScript,ios::in);
    if (! ficheroScript.is_open()){
        hayErrorDeIntepretacion = true;
    } 

    char caracter;
    while (! ficheroScript.eof() && hayErrorDeIntepretacion == false) {
        ficheroScript >> caracter;
        if (caracter == '\n' or caracter == '\t') {
            continue;
        }
        bool seEjecuto;
        seEjecuto = this.interpretar(caracter, iterador);
        if (!seEjecuto) {
            hayErrorDeIntepretacion == true;
        }
    }

    ficheroScript.close();
    return !hayErrorDeIntepretacion;
} // Aqui se deberia de destruir todo.

bool InterpreteBrainFuck::interpretar(char caracter, IteradorListaDoble iter) {

    if (caracter == '+') {
        return this._sumar_uno_actual(iter);
    } else if (caracter == '-') {
        return false;
    } else if (caracter == '.') {
        return false;
    } else if (caracter == ',') {
        return false;
    } else if (caracter == '<') {
        return false;
    } else if (caracter == '>') {
        return false;
    } else if (caracter == '[') {
        return false;
    } else if (caracter == ']') {
        return false; 
    } else {
        return false;
    }
}

bool _sumar_uno_actual(IteradorListaDoble iterador) {
    char caracterActual;
    iterador.leer(&caracterActual);
    //conitnua...
}