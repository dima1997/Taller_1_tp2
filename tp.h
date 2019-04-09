#ifndef TP_H
#define TP_H
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Nodo {
    public:
    char caracter;
    Nodo *anterior; //struct Nodo
    Nodo *siguiente; //struct Nodo

    Nodo();
    ~Nodo();
};

struct ListaDoble {
    public:
    Nodo *primero;
    
    ListaDoble();
    //ListaDoble(const ListaDoble &otraLista);
    void imprimir();
    ~ListaDoble();
    void insertar_primero(char caracter);
};

struct IteradorListaDoble {
    public:
    Nodo *actual;

    /*
    Inicializa un iterador de lista doblemente enlazada.
    PRE: Recibe un puntero a una lista doblemente enlazada,
    con al menos un elemento en ella.
    POST: Si la lista esta vacia, inserta un caracter nulo
    en la misma.
    */
    IteradorListaDoble(ListaDoble &lista);

    //IteradorListaDoble(IteradorListaDoble &otroIterador);
    
    /*
    Destruye un iterador de lista doblemente enlazada.
    */
    ~IteradorListaDoble();

    /*
    Avanza al siguiente nodo de la lista doblemente enlazada.
    PRE: No recibe nada.
    POST: Devuelve true si se logro avanzar al siguiente nodo,
    false en caso contrario, dado que ya se encuentra al final.
    */
    bool avanzar();

    /*
    Retrocede al anterior nodo de la lista doblemente enlazada.
    PRE: No recibe nada.
    Post: Devuelve true si logro retroceder al anterior nodo, 
    false en caso contrario.
    */
    bool retroceder();
    /*
    Agrega un caracter en la posicion siguiente a la actual.
    PRE: Recibe un caracter (char).
    POST: Devuelve true si logro insertar el caracter con exito
    en la siguiente posicion. 
    El iterador se mantiene en la posicion actual, sin moverse al 
    nuevo caracter insertado.
    */
    bool insertar_siguiente(char caracter);
    /*
    Escribe uno nuevo caracter sobre la posicion actual.
    PRE: Recibe un caracter (char).
    POST: Reemplaza el caracter en la posicion actual del 
    iterador y por el recibido. 
    */
    void escribir(char caracter);
    /*
    Lee el caracter en la posicion actual.
    PRE: Recibe un puntero a char donde guardar el 
    valor leido.
    POST: Guarda en la posicion de memoria recibida 
    el caracter leido.
    */
    void leer(char *caracter);
};

struct InterpreteBF {
    public: 
    istream *entrada; 
    ostream *salida; 
    ListaDoble *listaBF;
    IteradorListaDoble *iteradorBF;
    
    /*
    istream &entrada; 
    ostream &salida; 
    ListaDoble &listaBF;
    IteradorListaDoble &iteradorBF;
    */
    /*
    Inicializa un interprete de BrainFuck.
    PRE: Recibe una referencia al fichero (ya abierto) 
    desde donde el programa en curso leera informacion;
    y una referencia al fichero (ya abierto), en el que 
    el programa en curso volvacara informacion.
    POST: inicializa el interprete.
    */
    InterpreteBF(istream &entrada, ostream &salida);
    InterpreteBF();
    /*
    Destruye el interprete de BrainFuck.
    */
    ~InterpreteBF();
    bool procesar_script(string rutaScript);
    void sumar(int sumando);
    void avanzar();
    void retroceder();
    void leer();
    void escribir();
    bool interpretar(char caracter);
    vector<char> *_obtener_bloque(istream &script);
    int _obtener_final(vector<char> *bloque, int inicio);
    bool procesar_bloque(vector<char> *bloque, int inicio, int final);
};
#endif // TP_H