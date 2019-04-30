#include <iostream>
#include <vector>
#include <string>
#include "interprete.h"

/*
PRE: Recibe el nombre de un archivo de entrada, y de otro
de salida (std::string &), y un vector con todos los caracteres
de un script.bf a interpretar (vector<char>).
POST: Si el nombre de la entrada y/o salida es una string
"\0", entonces se utilizara std::cin/std::cout respectivamente,
como flujo de entrada/salida.
Inicializa un interprete de brainfuck.
*/
InterpreteBF::InterpreteBF(std::string &etda, std::string &slda, 
    std::vector<char> scrpt) : script(std::move(scrpt)), datos(1,0) {
    if (etda.compare(ENTRADA_SALIDA_ESTANDAR) != 0){
        (this->entrada).open(etda); 
    }
    if (slda.compare(ENTRADA_SALIDA_ESTANDAR) != 0){
        (this->salida).open(slda); 
    }
    this->indice = 0;
 }

/*
Destruye el interprete de BrainFuck.
*/
InterpreteBF::~InterpreteBF() {
    if (this->entrada.is_open()){
        (this->entrada).close(); 
    }
    if (this->salida.is_open()){
        (this->salida).close(); 
    }
}

/*
PRE: Recibe una doble referencia a otro interprete (InterpreteBF &&).
POST: Construye un nuevo interprete moviendo semanticamente todos
los atributos del inteprete recibido al actual.
El interprete recibido queda en estado nulo.
*/
InterpreteBF::InterpreteBF(InterpreteBF &&otroInterprete){
    this->entrada.swap(otroInterprete.entrada);
    this->salida.swap(otroInterprete.salida);
    this->script = std::move(otroInterprete.script);
    this->datos = std::move(otroInterprete.datos);
    this->indice = std::move(otroInterprete.indice);
    std::vector<char> vectorNulo(0);
    std::vector<unsigned char> vectorNuloSinSigno(0);
    otroInterprete.script = vectorNulo;
    otroInterprete.datos = vectorNuloSinSigno;
    otroInterprete.indice = 0;
}
/*
PRE: Recibe una doble referencia a otro interprete (InterpreteBF &&).
POST: Asigna por moviendo semanticamente todos los atributos del 
inteprete recibido al actual.
El interprete recibido queda en estado nulo.
*/
InterpreteBF& InterpreteBF::operator=(InterpreteBF &&otroInterprete){
    if (this == &otroInterprete){
        return *this;
    }
    this->entrada.swap(otroInterprete.entrada);
    this->salida.swap(otroInterprete.salida);
    this->script = std::move(otroInterprete.script);
    this->datos = std::move(otroInterprete.datos);
    this->indice = std::move(otroInterprete.indice);
    if (otroInterprete.entrada.is_open()){
        otroInterprete.entrada.close();
    } 
    if (otroInterprete.salida.is_open()){
        otroInterprete.salida.close();
    }
    std::vector<char> vectorNulo(0);
    std::vector<unsigned char> vectorNuloSinSigno(0);
    otroInterprete.script = vectorNulo;
    otroInterprete.datos = vectorNuloSinSigno;
    otroInterprete.indice = 0;
    return *this;
}

/*
PRE: Recibe un sumando (int);
POST: Suma el sumando al dato actual 
de la memoria de datos del programa 
en curso.
*/
void InterpreteBF::sumar(int sumando) {
    (this->datos)[this->indice] += sumando;
}

/*
PRE: No recibe nada.
POST: Escribe el dato actual en la salida
del programa en curso.
*/
void InterpreteBF::escribir() {
    char caracterActual = (this->datos)[this->indice];
    if (this->salida.is_open()){
        (this->salida).put(caracterActual);
    } else {
        std::cout.put(caracterActual);
    }
}

/*
PRE: No recibe nada.
POST: Leer un dato de la entrada del programa 
en curso y lo guarda en la posicion actual, de
la memoria de datos del mismo.
*/
void InterpreteBF::leer() {
    char caracterLeido = 0; 
    if (this->entrada.is_open()){
        (this->entrada).get(caracterLeido); 
    } else {
        std::cin.get(caracterLeido);
    }
    (this->datos)[this->indice] = int(caracterLeido); 
}

/*
PRE: No recibe nada.
POST: Avanza el indice de datos del inteprete 
a la siguiente posicion.
Devuelve true, si logro avanzar, false en caso 
contrario.
*/
bool InterpreteBF::avanzar() {
    size_t largoDatos = (this->datos).size();
    size_t proximoIndice = this->indice + 1;  
    if (largoDatos <= proximoIndice){
        char nuevoCaracter = 0;
        try {
            (this->datos).push_back(nuevoCaracter);
        } catch (std::bad_alloc& ba){
            return false;
        }
    }
    this->indice += 1;
    return true;
}

/*
PRE: No recibe nada.
POST: Retrocede a la anterior posicion de la 
memoria de datos del programa en curso.
Devuelve true si se pudo retroceder, false en
caso de que no se pueda retroceder mas.
*/
bool InterpreteBF::retroceder() {
    if (this->indice - 1 < 0){
        return false;
    }
    this->indice -= 1;
    return true;
}

/*
PRE: Recibe un caracter.
POST: Devuelve true si logro interpretar y ejecutar 
el caracter recibido.
*/
bool InterpreteBF::interpretar(char caracter) {
    if (caracter == int('+')) {
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
        return this->retroceder();
    } else if (caracter == '>') {
        return this->avanzar();;
    } else if (caracter == '\n' or caracter == '\t' or caracter == ' '){
        // Son caracteres utilizados
        // para hacer del script mas estructurado y 
        // legible. => Los ignoramos.
        return true;
    }
    // caracter desconocido.
    return false;
}

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
int InterpreteBF::_obtener_final_bloque(size_t inicio, size_t fin) {
    size_t i;
    size_t bloquesIntermedios = 0; 
    for (i = inicio; i<fin; ++i) {
        if ((this->script)[i] == '[') {
            bloquesIntermedios +=1;
        }
        if ((this->script)[i] != ']') {
            continue;
        }
        // Entonces es ]
        if (bloquesIntermedios > 1) {
            bloquesIntermedios -= 1;
            continue;
        } 
        break;
    }
    if (i >= fin) {
        return -1;
    }
    return i;
}

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
bool InterpreteBF::procesar_un_ciclo(size_t inicio, size_t fin){
    bool todoOK = true;
    char caracterActual;
    size_t i;
    for (i = inicio; i < fin && todoOK; ++i){
        caracterActual = (this->script)[i];
        if (caracterActual == '['){ 
            int finalBloque = this->_obtener_final_bloque(i, fin);
            if (finalBloque < 0){
                todoOK = false;
                continue;
            }
            todoOK = this->procesar_bloque(i+1, finalBloque);
            // no incluimos el [ actual
            i = finalBloque;
            continue;
        }
        todoOK = this->interpretar(caracterActual);
    }
    return todoOK;
}

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
bool InterpreteBF::procesar_bloque(size_t inicio, size_t final) {
    bool todoOK = true;
    char datoActual = 0;
    datoActual = (this->datos)[this->indice];
    while ((datoActual != 0) && (todoOK == true)) {
        todoOK = this->procesar_un_ciclo(inicio, final);
        datoActual = (this->datos)[this->indice]; 
    }
    return todoOK;
}

/*
PRE: No recibe nada.
POST: Interpreta y ejecuta el script.bf del
inteprete.
Devuelve true, si se ejecuto la el script con exito,
false encaso contrario.
*/
bool InterpreteBF::ejecutar() {
    size_t largoScript = (this->script).size();
    size_t i = 0;
    return this->procesar_un_ciclo(i, largoScript);
}
