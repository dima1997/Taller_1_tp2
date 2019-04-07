#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "tp.h"
using namespace std;
/*
Inicializa un nodo de lista doblemente enlazada.
*/
Nodo::Nodo() {
    this->caracter = new char[1]();
    this->anterior = NULL;
    this->siguiente = NULL;
}

/*
Nodo::Nodo(const Nodo &otroNodo) {
    this->caracter = new char[1]();
    this->caracter = otroNodo.caracter;
    this->anterior = otroNodo.anterior;
    this->siguiente = otroNodo.siguiente;
}
*/

/*
Destruye el nodo de lista doblemente enlazada.
*/
Nodo::~Nodo() {
    delete[] this->caracter;
}

/*
Inicializa una lista doblemente enlazada.
*/
ListaDoble::ListaDoble() {
    this->primero = NULL;
}
/*
ListaDoble::ListaDoble(const ListaDoble &otraLista) {
    this->primero = otraLista.primero;
}
*/
/*
Destruye la lista doblemente enlazada.
*/
ListaDoble::~ListaDoble() {
	Nodo* actual = this->primero;
	Nodo* siguiente = NULL;
	while (actual != NULL) {
		siguiente = actual->siguiente;
		delete actual;
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
    Nodo *nuevoNodo = new Nodo();
    *(nuevoNodo->caracter) = caracter;
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
IteradorListaDoble::IteradorListaDoble(ListaDoble &lista) {
    this->actual = lista.primero;
}
/*
IteradorListaDoble::IteradorListaDoble(IteradorListaDoble &otroIterador) {
    this->actual = otroIterador.actual;   
}
*/
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
    this->actual = this->actual->siguiente;
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
    *(nuevoNodo->caracter) = caracter;
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

/*
Inicializa un interprete de BrainFuck.
PRE: Recibe una referencia al fichero (ya abierto) 
desde donde el programa en curso leera informacion;
y una referencia al fichero (ya abierto), en el que 
el programa en curso volvacara informacion.
POST: inicializa el interprete.
*/
InterpreteBF::InterpreteBF(istream &entrada, ostream &salida) {
    ListaDoble *listaBF = new ListaDoble();
    (*listaBF).insertar_primero('\0');
    this->listaBF = listaBF;
    IteradorListaDoble *iteradorBF = new IteradorListaDoble(*listaBF);
    this->iteradorBF = iteradorBF;
    this->entrada = &entrada;
    this->salida = &salida;
}

/*
Destruye el interprete de BrainFuck.
*/
InterpreteBF::~InterpreteBF() {
    delete this->iteradorBF;
    delete this->listaBF;
}

/*
PRE: Recibe un bloque de codigo del script del programa
en curso; y el indice desde donde empezar a buscar en el
bloque.
POST: Devuelve la posicion donde se encuentra el primer 
caracter ']' que no corresponda ningun bloque intermedio.
Si la posicion devuelta es negativo, es porque no se 
encontro dicho caracter en esas condiciones.
*/
int InterpreteBF::_obtener_final(vector<char> *bloque, int inicio) {
    int i;
    size_t bloquesIntermedios = 0;
    for (i = inicio; (*bloque)[i]!='\0'; ++i) {
        if ((*bloque)[i] == '[') {
            bloquesIntermedios +=1;
        }
        if ((*bloque)[i] != ']') {
            continue;
        }
        if (bloquesIntermedios > 0) {
            bloquesIntermedios -= 1;
            continue;
        }
        break;
    }
    if ((*bloque)[i] == 0) {
        return -1;
    }
    return i;
}

/*
PRE: Recibe un bloque de ciclo de codigo del script del 
programa en curso; la posicion inicial del bloque
desde donde empezar a ejecutar; y la posicion final
hasta donde se debe ejecutar (sin incluir).
Nota: inicio <= final <= largo del bloque.
POST: Ejecuta el bloque de codigo hasta que en la posicion
actual del puntero a la memoria de datos este el 
caracter nulo. Devuelve bool so logro procesar el bloque
con exito, false en caso contrario.
*/
bool InterpreteBF::procesar_bloque(vector<char> *bloque, int inicio, int final) {
    char caracterDato;
    char caracterBloque;
    (*this->iteradorBF).leer(&caracterDato);
    while (caracterDato != '\0') {
        for (int i = inicio ; i<final; ++i) {
            caracterBloque = (*bloque)[i];
            if (caracterBloque == '[') {
                // Se procesa un nuevo sub bloque.
                int subFinal = _obtener_final(bloque, i);
                if (subFinal < 0) {
                    return false;
                }
                bool seProceso;
                seProceso = procesar_bloque(bloque, i, subFinal);
                if (!seProceso) {
                    return false;
                }
            }
            bool seInterpreto = this->interpretar(caracterBloque); 
            if (!seInterpreto) {
                return false;
            }
        }
        this->iteradorBF->leer(&caracterDato);
    }
    return true;
}

/*
PRE: REcibe un script, fichero abierto (fstream *).
POST: Lee el script en curso hasta encontrar un ']'
que no corresponda a otro '[' en el medio.
Devuelve el bloque de caracteres que va desde
el actual caracter del archivo, hasta ']', 
sin incluirlo, siendo el proximo caracter a leer
el siguiente a ']'.
Devuelve NULL si no encontro el caracter ']'. 
Queda a responsabilidad del usuario liberar la 
memoria reservada.
*/
vector<char> *InterpreteBF::_obtener_bloque(istream &script) {
    size_t factorRedimensionar = 2;
    size_t largoBloque = 10; 

    vector<char> *bloque = new vector<char>(largoBloque);
    if (bloque == NULL) {
        return NULL;
    }

    char caracter = 0;
    int bloquesIntermedios = 0;
    size_t i = 0;
    while (!script.eof()) {
        script >> caracter;
        if (caracter == '\n' or caracter == '\t') {
            // Ahorro memoria
            continue;
        }
        if (caracter == '[') {
            bloquesIntermedios += 1;
        }
        if (caracter == ']') {
            if (bloquesIntermedios > 0) {
                bloquesIntermedios -= 1;
            } else {
                break; //Arreglamos esto
            }
        }
        (*bloque)[i] = caracter;
        ++i;
        if (i >= largoBloque) {
            size_t nuevoLargo = largoBloque * factorRedimensionar;
            /*
            size_t memoriaReservar = sizeof(char)*nuevoLargo;
            char* nuevoBloque = realloc(bloque, memoriaReservar);
            */
            (*bloque).resize(nuevoLargo);
            if (bloque == NULL) {
                delete[] bloque; 
                return NULL;
            }
            //bloque = nuevoBloque;
            largoBloque = nuevoLargo; 
        }
    }
    if (script.eof()) {
        delete[] bloque;
        return NULL;
    }
    (*bloque)[i] = 0;
    size_t largoFinal = i+1;
    /*
    size_t memoriaReservar = sizeof(char)*largoFinal;
    char *bloqueFinal = realloc(bloque, memoriaReservar);
    */
    (*bloque).resize(largoFinal);
    if (bloque == NULL) {
        delete[] bloque;
        return NULL;
    }
    //bloque = bloqueFinal;
    largoBloque = largoFinal;
    return bloque;
}

/*
PRE: Recibe un sumando (int);
POST: Suma el sumando al dato actual 
de la memoria de datos del programa 
en curso.
*/
void InterpreteBF::sumar(int sumando) {
    char caracterActual;
    this->iteradorBF->leer(&caracterActual);
    int valorACSII = int(caracterActual);
    //sumamos 1 y lo volvermos a convertir en char
    valorACSII = (valorACSII + sumando) % 255; 
    // cantidad de caracteres ACSII
    caracterActual = valorACSII; //Esperomos que funcione
    this->iteradorBF->escribir(caracterActual);
    return;
}

/*
PRE: No recibe nada.
POST: Escribe el dato actual en la salida
del programa en curso.
*/
void InterpreteBF::escribir() {
    char caracterActual;
    (*this->iteradorBF).leer(&caracterActual);
    (*this->salida).put(caracterActual);
}

/*
PRE: No recibe nada.
POST: Leer un dato de la entrada del programa 
en curso y lo guarda en la posicion actual, de
la memoria de datos del mismo.
*/
void InterpreteBF::leer() {
    char caracterLeido;
    (*this->entrada).get(caracterLeido);
    (*this->iteradorBF).escribir(caracterLeido);  
}

/*
PRE: No recibe nada.
POST: Avanza a la siguiente posicion de la 
memoria de datos del programa en curso.
*/
void InterpreteBF::avanzar() {
    if (! this->iteradorBF->avanzar()) {
        this->iteradorBF->insertar_siguiente('\0');
        this->iteradorBF->avanzar();
    }
}

/*
PRE: No recibe nada.
POST: Retrocede a la anterior posicion de la 
memoria de datos del programa en curso.
*/
void InterpreteBF::retroceder() {
    this->iteradorBF->retroceder();
}

bool InterpreteBF::interpretar(char caracter) {
    // faltan considerar todos los posible errores y levantar mensajes
    if (caracter == '+') {
        this->sumar(1);
        return true;
    } else if (caracter == '-') {
        this->sumar(-1);
        return true;
    } else if (caracter == '.') {
        this->escribir();
        return true;
    } else if (caracter == ',') {
        this->leer();
        return true;
    } else if (caracter == '<') {
        this->retroceder();
        return true;
    } else if (caracter == '>') {
        this->avanzar();
        return true;
    } else if (caracter == '\n' or caracter == '\t'){
        // Son caracteres utilizados
        // para hacer del script mas estructurado y 
        // legible. => Los ignoramos.
        return true;
    }
    // caracter desconocido.
    return false;
}

bool InterpreteBF::procesar_script(string rutaScript) {
    //ListaDoble datos();
    //datos.insertar_primero('\0');
    //IteradorListaDoble iterador(datos);
    bool hayErrorDeIntepretacion = false;
    ifstream ficheroScript;
    ficheroScript.open(rutaScript,ios::in);
    if (! ficheroScript.is_open()){
        hayErrorDeIntepretacion = true;
    } 
    char caracter;
    while (! ficheroScript.eof() && hayErrorDeIntepretacion == false) {
        ficheroScript >> caracter; //cin.get()
        if (caracter == '[') {
            vector<char> *bloque = (*this)._obtener_bloque(ficheroScript);
            if (bloque == NULL) {
                hayErrorDeIntepretacion = true;
                continue;
            }
            size_t largoBloque = (*bloque).size();
            bool seProceso;
            seProceso = this->procesar_bloque(bloque, 0, largoBloque-1); 
            //sin contar al \0
            delete bloque; // por alguna razon no se libera por si sola con RAII
            if (!seProceso) {
                hayErrorDeIntepretacion = true;
            }
            continue;
        }
        bool seEjecuto;
        seEjecuto = this->interpretar(caracter);
        hayErrorDeIntepretacion = !seEjecuto;
    }
    ficheroScript.close();
    return !hayErrorDeIntepretacion;
} // Aqui se deberia de destruir todo.

bool es_script_bf(string nombreFichero) {
    int largoNombre = nombreFichero.size();
    string terminacionScriptBF (".bf");
    int largoTerminacion = terminacionScriptBF.size();
    if (largoNombre < largoTerminacion) {
        return false;
    }
    for (int i = 0; i<largoTerminacion; ++i) {
        int iNombre = largoNombre -1 -i;
        int iTerminacion = largoTerminacion -1 -i;
        if (nombreFichero[iNombre] != terminacionScriptBF[iTerminacion]) {
            return false;
        }
    }
    return true;
}

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        return 2;
    }
    //const char *modo = argv[1];
    string modo (argv[1]);
    //const char *script = argv[2];
    string script (argv[2]);

    if (modo.compare("interprete") == 0) {
        if (!es_script_bf(script)) {
            return 2;
        }
        InterpreteBF interprete(cin, cout);
        bool seInterpreto;
        seInterpreto = interprete.procesar_script(script);
        if (!seInterpreto){
            return 1;
        }
        //El destruir deberia darse solo
    }
    return 0;
}
