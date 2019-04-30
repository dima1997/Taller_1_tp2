#ifndef INTERPRETE_H
#define INTERPRETE_H
#include <fstream>
#include <vector>
#include <string>
#define ENTRADA_SALIDA_ESTANDAR "\0"

//class InterpreteBF

class InterpreteBF {
    private:
    std::ifstream entrada; 
    std::ofstream salida; 
    std::vector<char> script;
    std::vector<unsigned char> datos;
    int indice;

    public: 
    /*
    PRE: Recibe el nombre de un archivo de entrada, y de otro
    de salida (std::string &), y un vector con todos los caracteres
    de un script.bf a interpretar (vector<char>).
    POST: Si el nombre de la entrada y/o salida es una string
    "\0", entonces se utilizara std::cin/std::cout respectivamente,
    como flujo de entrada/salida.
    Inicializa un interprete de brainfuck.
    */
    InterpreteBF(std::string &etda, std::string &slda, std::vector<char> scrpt);

    /*
    Destruye el interprete de BrainFuck.
    */
    ~InterpreteBF();

    /*
    PRE: Recibe una doble referencia a otro interprete (InterpreteBF &&).
    POST: Construye un nuevo interprete moviendo semanticamente todos
    los atributos del inteprete recibido al actual.
    El interprete recibido queda en estado nulo.
    */
    InterpreteBF(InterpreteBF &&otroInteprete);

    /*
    PRE: Recibe una doble referencia a otro interprete (InterpreteBF &&).
    POST: Asigna por moviendo semanticamente todos los atributos del 
    inteprete recibido al actual.
    El interprete recibido queda en estado nulo.
    */
    InterpreteBF& operator=(InterpreteBF &&otroInteprete);

    /*
    PRE: Recibe un sumando (int);
    POST: Suma el sumando al dato actual 
    de la memoria de datos del programa 
    en curso.
    */
    void sumar(int sumando);

    /*
    PRE: No recibe nada.
    POST: Escribe el dato actual en la salida
    del programa en curso.
    */
    void escribir();

    /*
    PRE: No recibe nada.
    POST: Leer un dato de la entrada del programa 
    en curso y lo guarda en la posicion actual, de
    la memoria de datos del mismo.
    */
    void leer();
    
    /*
    PRE: No recibe nada.
    POST: Avanza el indice de datos del inteprete 
    a la siguiente posicion.
    Devuelve true, si logro avanzar, false en caso 
    contrario.
    */
    bool avanzar();

    /*
    PRE: No recibe nada.
    POST: Retrocede a la anterior posicion de la 
    memoria de datos del programa en curso.
    Devuelve true si se pudo retroceder, false en
    caso de que no se pueda retroceder mas.
    */
    bool retroceder();
    
    /*
    PRE: Recibe un caracter.
    POST: Devuelve true si logro interpretar y ejecutar 
    el caracter recibido.
    */
    bool interpretar(char caracter);

    /*
    PRE: Recibe dos numeros (size_t) correspondientes a una 
    posicion inicial y final dentro del script actual.
    POST: Devuelve la posicion donde se encuentra el primer 
    caracter ']' que no corresponda ningun bloque intermedio,
    desde la posicion inicial recibida hasta la posicion final
    sin incluir.
    Si la posicion devuelta es negativa, es porque no se 
    encontro dicho caracter en esas condiciones.
    */
    int _obtener_final_bloque(size_t inicio, size_t fin);
    
    /*
    PRE: Recibe dos numeros (size_t): inicion y fin.
    POST: Procesar e interpreta una sola vez, un bloque
    de caracteres del script del interprete, desde el 
    inicio recibido hasta el fin (sin incluir), tambien
    recibido.
    Devuelve true si se logro intepretar con exito cada 
    caracter del rango definido, false en caso contrario,
    dado algun error.
    */
    bool procesar_un_ciclo(size_t inicio, size_t fin);

    /*
    PRE: Recibe la posicion inicial del script en interpretacion
    desde donde empezar a ejecutar; y la posicion final hasta 
    donde se debe ejecutar (sin incluir).
    Nota: inicio <= final <= largo del script.
    POST: Ejecuta el bloque de codigo hasta que en la posicion
    actual del puntero a la memoria de datos este el 
    caracter nulo. Devuelve bool so logro procesar el bloque
    con exito, false en caso contrario.
    */
    bool procesar_bloque(size_t inicio, size_t final);

    /*
    PRE: No recibe nada.
    POST: Interpreta y ejecuta el script.bf del
    inteprete.
    Devuelve true, si se ejecuto la el script con exito,
    false encaso contrario.
    */
    bool ejecutar();
};
#endif // INTERPRETE_H
