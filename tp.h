#ifndef TP_H
#define TP_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

/*
PRE: Recibe una cadena de caracteres (string &), un separador 
(string &), y un vector de strings (vector<string> &).
POST: LLena el vector recibido (en sus ultimas posiciones), con
las partes de la cadena recibida que este separados por el separador
recibido.
*/
void split(std::string &cad, std::string &sep, std::vector<std::string> &vSplt);

/*
PRE: Recibe una ruta a un archivo (string &).
POST: Devuelve un vector de caracteres (vector<char>)
con todos los caracteres del archivo; o NULL si ocurrió
un error.
Queda a responsabilidad del usuario liberar la memoria
reservada.
*/
std::vector<char> *cargar_archivo(std::string &rutaArchivo);


class InterpreteBF {
    private:
    std::ifstream entrada; 
    std::ofstream salida; 
    std::vector<char> *script;
    std::vector<int> *datos;
    int indice;
    bool todoOK;

    public: 
    /*
    Inicializa un interprete de BrainFuck.
    PRE: Recibe una referencia al fichero (ya abierto) 
    desde donde el programa en curso leera informacion;
    una referencia al fichero (ya abierto), en el que 
    el programa en curso volvacara informacion; y el 
    script (vector<char>) a intepretar.
    POST: inicializa el interprete.
    */
    InterpreteBF(std::string etda, std::string slda, std::vector<char> *script);

    /*
    Destruye el interprete de BrainFuck.
    */
    ~InterpreteBF();

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
    */
    void avanzar();

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
    PRE: No recibe nada.
    POST: Devuelve true si lo interpretado 
    hasta el momento es valido, false en caso
    contrario.
    */
    bool estaTodoOK();

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

struct InterpPriori{
    public:
    size_t prioridad;
    private :
    InterpreteBF *interprete;
    
    public:
    /*
    PRE: Recibe una prioridad (size_t), y un puntero a un 
    interpreteBF (InterpreteBF *) ya creado.
    POST: Inicializa un Interprete con prioridad.
    */
    InterpPriori(size_t prioridad, InterpreteBF *iBF);
    
    /*
    Destruye un interprete con prioridad.
    */
    ~InterpPriori();

    /*
    PRE: Recibe una referencia a otro interprete con prioridad.
    POST: Mueve semanticamente los recursos del actual inteprete con
    prioridad al interprete con prioridad recibido
    */
    void moverSemanticamenteA(InterpPriori &otroIPriori);

    /*
    Ejecuta el interprete, que guarda.
    */
    bool ejecutar();
};

struct IPComparador{
    /*
    PRE: Recibe dos punteros a instancias de InterpPriori (InterpPriori const*).
    POST: Devuelve true, si el primero tiene mayor prioridad que el segundo.
    */
    bool operator() (InterpPriori const* iP1, InterpPriori const* iP2);
};

typedef 
std::priority_queue<InterpPriori*, std::vector<InterpPriori*>, IPComparador>
colaPrioridadInterprete_t;

class PrioriColaInterpProtegida{
    private:
    colaPrioridadInterprete_t *heapInterp;
    bool &colaAbierta; 
    std::mutex centinela; 
    bool seEncolo; 
    std::condition_variable seEncolo_variable; 

    public:
    /*
    PRE: Recibe un valor booleano que indique cuando el hilo principal
    ya no va encolar mas elementos.
    POST: Inicializa una cola de interpretes con prioridad protegida.
    */
    explicit PrioriColaInterpProtegida(bool &colaAbierta);

    /*Destruye una cola de interpretes con prioridad protegida.*/
    ~PrioriColaInterpProtegida();
    
    /*
    PRE: Recibe un interprete con prioridad (InterpPriori *)
    POST: Encola el interprete recibido en la cola.
    */
    void encolar(InterpPriori *interpP);
    /*
    PRE: Recibe una referencia a un interprete con prioridad.
    POST: Desencola un interprete con prioridad y lo guarda en la 
    referencia recibida.
    Devuelve true si se desencolo un interprete, o false si la
    cola esta vacia.
    */
    bool desencolar(InterpPriori &iPriori);


    /*Devuelve true si la cola esta vacia, false en caso contrario.*/
    bool estaVacia();
};

class Thread {
    private:
    std::thread thread;

    public:
    /*Inicializa un Thread*/
    Thread() {}
    
    /*Inicia un nuevo hilo de ejecucion*/
    void start() {
        thread = std::thread(&Thread::run, this);
    }
    
    /*Espera a que el hilo en ejecucion termine*/
    void join() {
        thread.join();
    }
        
    /*Ejecuta la funcion del Thread*/
    virtual void run() = 0;
        
    /*Destruye el Thread*/
    virtual ~Thread() {}
        
    /*Impide que se copien hilos por parametro*/
    Thread(const Thread&) = delete;

    /*Evita que se copien hilos por asignacion*/
    Thread& operator=(const Thread&) = delete;

    /*
    Mueve semanticamente los recursos de un Thread a otro,
    por parametro.
    */
    Thread(Thread&& other) {
        this->thread = std::move(other.thread);
    }
        
    /*
    Meuve semanticamente los recursos de un Thread a otro, 
    por asignacion.
    */
    Thread& operator=(Thread&& other) {
        this->thread = std::move(other.thread);
        return *this;
    }
};

class HiloBF: public Thread {
    private:
    PrioriColaInterpProtegida &heapInterp;
    public: 
    /*
    PRE: Recibe una cola de interpretes con prioridad protegida.
    POST: Inicializa un hilo de ejecucion de script intepretes brainfuck.
    */
    explicit HiloBF(PrioriColaInterpProtegida &hCola);
    
    /*Destruye el hilo de ejecucion.*/
    ~HiloBF();

    /*Ejecuta el hilo de ejecuccion.*/
    virtual void run() override;
};

class ThreadPool{
    private:
    size_t cantidadHilos;
    
    public:
    /*Inicializa un ThreadPool de cantidad de hilos (size_t) recibida*/
    explicit ThreadPool(size_t cantidadHilos);
    
    /*Detruye un ThreadPool*/
    ~ThreadPool();
    
    /*Procesa una linea recibida de la entrada estandar del ThreadPool
      Devuelve una estructura interprete con prioridad, o NULL, si ocurrio
      algun error, queda a resposabilidad del usuario liberar la memoria
      reservada.
    */
    InterpPriori *procesar_linea(std::string &linea);
    
    /*
    Ejecuta el ThreadPool.
    */
    int ejecutar();    
};

class Interpretador{
    private:
    std::string &rutaScript;

    public:
    /*
    PRE: Recibe una ruta a un script.bf (string &).
    POST: Inicializa un intepretador.
    */
    explicit Interpretador(std::string &rutaScript);
    
    /*Destruye un interpretador*/
    ~Interpretador();
    
    /*
    Devuelve true si el script a ejecutar tiene terminacio .bf,
    false en caso contrario.
    */
    bool es_script_bf();
    
    /*
    Ejecuta el modo inteprete del interpretador.
    Devuelve:
     0 si la ejecucion fue exitosa.
     1 si ocurrio algun erro de intepretacion.
     2 si hubo algun error en la linea de comandos.
    */
    int ejecutar();
};
#endif // TP_H
