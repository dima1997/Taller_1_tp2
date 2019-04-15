#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "tp.h"

#define CANTIDAD_ACSII 256

/*
PRE: Recibe una cadena de caracteres (string &), un separador 
(string &), y un vector de strings (vector<string> &).
POST: LLena el vector recibido (en sus ultimas posiciones), con
las partes de la cadena recibida que este separados por el separador
recibido.
*/
void split(std::string &cad, std::string &sep,
            std::vector<std::string> &vSplt){
    std::string copiaCad(cad);
    std::string parteCadena;
    size_t pos;
    while ((pos = copiaCad.find(sep)) != std::string::npos) {
        parteCadena = copiaCad.substr(0, pos);
        vSplt.push_back(parteCadena);
        copiaCad.erase(0, pos + sep.size());
    }
    return;
}

/*
PRE: Recibe una ruta a un archivo (string &).
POST: Devuelve un vector de caracteres (vector<char>)
con todos los caracteres del archivo; o NULL si ocurrió
un error.
Queda a responsabilidad del usuario liberar la memoria
reservada.
*/
std::vector<char> *cargar_archivo(std::string &rutaArchivo){
    std::ifstream archivo;
    archivo.open(rutaArchivo);
    if (!archivo.is_open()){
        return NULL;
    }
    std::vector<char> *texto = new std::vector<char>();
    char caracterActual;
    while (archivo.good()){
        archivo.get(caracterActual);
        texto->push_back(caracterActual);
    }
    archivo.close();
    return texto;
}

/*
Inicializa un interprete de BrainFuck.
PRE: Recibe una referencia al fichero (ya abierto) 
desde donde el programa en curso leera informacion;
y una referencia al fichero (ya abierto), en el que 
el programa en curso volvacara informacion.
POST: inicializa el interprete.
*/
InterpreteBF::InterpreteBF(std::string etda, std::string slda, 
    std::vector<char> *script){
    (this->entrada).open(etda); 
    (this->salida).open(slda); 
    this->script = script;
    this->datos = new std::vector<int>(1,0); 
    this->indice = 0;
    this->todoOK = true;
 }

/*
Destruye el interprete de BrainFuck.
*/
InterpreteBF::~InterpreteBF() {
    (this->entrada).close(); 
    (this->salida).close(); 
    delete this->datos;
    delete this->script;
}

/*
PRE: Recibe un sumando (int);
POST: Suma el sumando al dato actual 
de la memoria de datos del programa 
en curso.
*/
void InterpreteBF::sumar(int sumando) {
    (*this->datos)[this->indice] += sumando;
}

/*
PRE: No recibe nada.
POST: Escribe el dato actual en la salida
del programa en curso.
*/
void InterpreteBF::escribir() {
    int valorActual = (*this->datos)[this->indice];
    char caracterActual = valorActual % CANTIDAD_ACSII;
    if (this->salida.is_open()){
        (this->salida).put(caracterActual); //*
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
    char caracterLeido;
    if (this->entrada.is_open()){
        (this->entrada).get(caracterLeido); //*
    } else {
        std::cin.get(caracterLeido);
    }
    (*this->datos)[this->indice] = int(caracterLeido); 
}

/*
PRE: No recibe nada.
POST: Avanza el indice de datos del inteprete 
a la siguiente posicion.
*/
void InterpreteBF::avanzar() {
    size_t largoDatos = (*this->datos).size();
    size_t proximoIndice = this->indice + 1;
    
    if (largoDatos <= proximoIndice){
        (*this->datos).push_back(0);
    }
    this->indice += 1;
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
        return this->retroceder();
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
        if ((*this->script)[i] == '[') {
            bloquesIntermedios +=1;
        }
        if ((*this->script)[i] != ']') {
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
        caracterActual = (*this->script)[i];
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
    int datoActual = (*this->datos)[this->indice];
    while ((datoActual != 0) && (todoOK == true)) {
        todoOK = this->procesar_un_ciclo(inicio, final);
        datoActual = (*this->datos)[this->indice];
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
    size_t largoScript = (*this->script).size();
    size_t i = 0;
    return this->procesar_un_ciclo(i, largoScript);
} 

/*
PRE: No recibe nada.
POST: Devuelve true si lo interpretado 
hasta el momento es valido, false en caso
contrario.
*/
bool InterpreteBF::estaTodoOK(){
    return this->todoOK;
}

/*
PRE: Recibe una prioridad (size_t), y un puntero a un 
interpreteBF (InterpreteBF *) ya creado.
POST: Inicializa un Interprete con prioridad.
*/
InterpPriori::InterpPriori(size_t prioridad, InterpreteBF *iBF) 
        : prioridad(prioridad), interprete(iBF){}
/*
Destruye un interprete con prioridad.
*/
InterpPriori::~InterpPriori(){
    if (this->interprete != NULL){
        delete this->interprete;
    }
}

/*
PRE: Recibe una referencia a otro interprete con prioridad.
POST: Mueve semanticamente los recursos del actual inteprete con
prioridad al interprete con prioridad recibido
*/
void InterpPriori::moverSemanticamenteA(InterpPriori &otroIPriori){
    otroIPriori.prioridad = this->prioridad;
    otroIPriori.interprete = this->interprete;
    this->prioridad = 0;
    this->interprete = NULL;
}
/*
Ejecuta el interprete, que guarda.
Devuelve true si se ejecuto con exito, 
false en caso contrario.
*/
bool InterpPriori::ejecutar(){
    return this->interprete->ejecutar();
}

/*
PRE: Recibe dos punteros a instancias de InterpPriori (InterpPriori const*).
POST: Devuelve true, si el primero tiene mayor prioridad que el segundo.
*/
bool IPComparador::operator()(InterpPriori const* iP1, InterpPriori const* iP2){
    return (iP1->prioridad > iP2->prioridad);
}

/*
PRE: Recibe un valor booleano que indique cuando el hilo principal
ya no va encolar mas elementos.
POST: Inicializa una cola de interpretes con prioridad protegida.
*/
PrioriColaInterpProtegida::PrioriColaInterpProtegida(bool &colaAbierta)
: colaAbierta(colaAbierta){
    this->heapInterp = new colaPrioridadInterprete_t();
    this->seEncolo = false;
}

/*Destruye una cola de interpretes con prioridad protegida.*/
PrioriColaInterpProtegida::~PrioriColaInterpProtegida(){
    delete this->heapInterp;
}

/*
PRE: Recibe un interprete con prioridad (InterpPriori *)
POST: Encola el interprete recibido en la cola.
*/
void PrioriColaInterpProtegida::encolar(InterpPriori *interpP){
    std::unique_lock<std::mutex> lock(this->centinela);
    this->heapInterp->push(interpP);
    this->seEncolo = true;
    this->seEncolo_variable.notify_one();
}
/*
PRE: Recibe una referencia a un interprete con prioridad.
POST: Desencola un interprete con prioridad y lo guarda en la 
referencia recibida.
Devuelve true si se desencolo un interprete, o false si la
cola esta vacia.
*/
bool PrioriColaInterpProtegida::desencolar(InterpPriori &iPriori){
    std::unique_lock<std::mutex> lock(this->centinela);
    if (! this->colaAbierta){
        return false;
    }
    while (!this->seEncolo){
        this->seEncolo_variable.wait(lock);
    }
    InterpPriori *iPrioriActual = this->heapInterp->top();
    (*iPrioriActual).moverSemanticamenteA(iPriori);
    delete iPrioriActual; 
    //Veamo si funciona
    this->heapInterp->pop();
    return true;
}

/*Devuelve true si la cola esta vacia, false en caso contrario.*/
bool PrioriColaInterpProtegida::estaVacia(){
    return this->heapInterp->empty();
}

/*
PRE: Recibe una cola de interpretes con prioridad protegida, y 
una referencia a un valor booleano, que indique si se seguiran
encolando elelemento en dicha cola.
POST: Inicializa un hilo de ejecucion de script intepretes brainfuck.
*/
HiloBF::HiloBF(PrioriColaInterpProtegida &colaPriori) 
: heapInterp(colaPriori) {}

/*Destruye el hilo de ejecucion.*/
HiloBF::~HiloBF(){}

/*Ejecuta el hilo.*/
void HiloBF::run(){
    while (!this->heapInterp.estaVacia()){
        InterpPriori iPriori(0, NULL);
        if (this->heapInterp.desencolar(iPriori)){
            iPriori.ejecutar();
        } else {
            //Ya no queda mas que ejecutar.
            break;
        }
    }
    return;
}

/*Inicializa un ThreadPool de cantidad de hilos (size_t) recibida*/
ThreadPool::ThreadPool(size_t cantidadHilos){
    this->cantidadHilos = cantidadHilos;
}

/*Detruye un ThreadPool*/
ThreadPool::~ThreadPool(){}

/*
PRE: Recibe una linea (&string) a procesar.
POST: Devuelve un interprete con prioridad (InterPrior *), 
listo para ser ejecutado.
Queda a responsabilidad del usuario liberar la memoria 
reservada por medio de la sentencia delete.
*/
InterpPriori *ThreadPool::procesar_linea(std::string &linea){
    size_t posParent1 = linea.find("("); 
    size_t posParent2 = linea.find(")");
    if (posParent1 == std::string::npos || posParent2 == std::string::npos){
        return NULL;
    }
    std::string lineaSinParentesis = linea.substr(posParent1, posParent2);
    std::vector<std::string> lineaSplit(0); 
    std::string separador(", ");
    split(linea, separador, lineaSplit); 
    if (lineaSplit.size() != 5){
        return NULL;
    }
    size_t prioridad = stoi(lineaSplit[1]);
    std::string rutaEntrada = lineaSplit[2];
    std::string rutaSalida = lineaSplit[3];
    std::string script = lineaSplit[4];
    size_t largoScript = script.size();
    std::vector<char> *scriptVector = new std::vector<char>;
    for (size_t i = 0; i < largoScript; i++){
        (*scriptVector).push_back(script[i]);
    } 
    InterpreteBF *interp; 
    interp = new InterpreteBF(rutaEntrada, rutaSalida, scriptVector);
    InterpPriori *iPriori;
    iPriori = new InterpPriori(prioridad, interp);
    return iPriori;
}

/*Ejecuta el ThreadoPool.*/
int ThreadPool::ejecutar(){
    bool colaAbierta = true;
    PrioriColaInterpProtegida heapInterpretes(colaAbierta);
    std::vector<Thread*> hilos;
    for (size_t i = 0; i < this->cantidadHilos; ++i) {
        hilos.push_back(new HiloBF(heapInterpretes));
    }
    for (size_t i = 0; i < this->cantidadHilos; ++i) {
        hilos[i]->start();
    }
    std::string linea;
    while (std::cin.good()){  
        getline(std::cin, linea);
        InterpPriori *interp = this->procesar_linea(linea);
        if (interp == NULL){
            std::cout << "Error: linea no valida\n";
            continue; 
        }
        heapInterpretes.encolar(interp);
    }
    colaAbierta = false;
    for (size_t i = 0; i < this->cantidadHilos; ++i) {
        hilos[i]->join();
        delete hilos[i];
    }
    return 0;
}

/*
PRE: Recibe una ruta a un script.bf (string &).
POST: Inicializa un intepretador.
*/
Interpretador::Interpretador(std::string &rutaScript) : rutaScript(rutaScript){}

/*Destruye un interpretador*/
Interpretador::~Interpretador(){}

/*
Devuelve true si el script a ejecutar tiene terminacio .bf,
false en caso contrario.
*/
bool Interpretador::es_script_bf(){
    int largoRuta = this->rutaScript.size();
    std::string terminacionScriptBF(".bf");
    int largoTerminacion = terminacionScriptBF.size();
    if (largoRuta < largoTerminacion) {
        return false;
    }
    for (int i = 0; i<largoTerminacion; ++i) {
        int iRuta = largoRuta -1 -i;
        int iTerminacion = largoTerminacion -1 -i;
        if (this->rutaScript[iRuta] != terminacionScriptBF[iTerminacion]) {
            return false;
        }
    }
    return true;
}

/*
Ejecuta el modo inteprete del interpretador.
Devuelve:
 0 si la ejecucion fue exitosa.
 1 si ocurrio algun erro de intepretacion.
 2 si hubo algun error en la linea de comandos.
*/
int Interpretador::ejecutar(){
    if (!this->es_script_bf()) {
        return 2;
    }
    std::vector<char> *script = cargar_archivo(this->rutaScript);
    if (script == NULL){
        std::cout << "Error: no se pudo cargar el script.bf\n";
        return 1; 
    }
    std::string nulo("\0");
    InterpreteBF interprete(nulo, nulo, script);
    return interprete.ejecutar() ? 0 : 1;
}

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        return 2;
    }
    std::string modo(argv[1]);
    if (modo.compare("interprete") == 0) {
        std::string rutaScript(argv[2]);
        Interpretador modoInteprete(rutaScript);
        return modoInteprete.ejecutar();
    }
    if (modo.compare("thread-pool")){
        std::string cantidadHilosCadena(argv[2]);
        size_t cantidadHilos = stoi(cantidadHilosCadena);
        ThreadPool modoThreadPool(cantidadHilos);
        return modoThreadPool.ejecutar();
    }
    return 2;
}
