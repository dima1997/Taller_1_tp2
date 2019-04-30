#ifndef HILOS_H
#define HILOS_H
#include <queue>
#include <condition_variable>
#include <thread>
#include <vector>
#include "interprete.h"

// struct InterpPriori

struct InterpPriori{
    public:
    size_t prioridad;
    private :
    InterpreteBF interprete;
    
    public:
    /*
    PRE: Recibe una prioridad (size_t), y un 
    interpreteBF (InterpreteBF) ya creado.
    POST: Inicializa un Interprete con prioridad.
    */
    InterpPriori(size_t prioridad, InterpreteBF iBF);
    
    /*
    Destruye un interprete con prioridad.
    */
    ~InterpPriori();

    /*
    Ejecuta el interprete, que guarda.
    */
    bool ejecutar();
};

// struct IPComparador

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

//class PrioriColaInterpProtegida

class PrioriColaInterpProtegida{
    private:
    colaPrioridadInterprete_t heapInterp;
    bool colaAbierta; 
    std::mutex centinela; 
    std::condition_variable continuar_desencolar; 

    public:
    /*
    PRE: Recibe un valor booleano que indique cuando el hilo principal
    ya no va encolar mas elementos.
    POST: Inicializa una cola de interpretes con prioridad protegida.
    */
    PrioriColaInterpProtegida();
    
    /*Destruye una cola de interpretes con prioridad protegida.*/
    ~PrioriColaInterpProtegida();
    
    /*
    Se le notifica a la cola que no se encolaran mas elementos 
    en ella.
    */
    void cerrarCola();

    /*
    PRE: Recibe un interprete con prioridad (InterpPriori *)
    POST: Encola el interprete recibido en la cola.
    */
    void encolar(InterpPriori *interpP);
    
    /*
    Desencola un interprete con prioridad (InterpPriori *) y
    lo devuelve.
    Si el valor devuelto es NULL, es porque la cola esta cerrada
    y vacia.
    */
    InterpPriori *desencolar();

    /*Devuelve true si la cola esta vacia, false en caso contrario.*/
    bool estaVacia();
};

//class Thread 

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
    Mueve semanticamente los recursos de un Thread a otro, 
    por asignacion.
    */
    Thread& operator=(Thread&& other) {
        this->thread = std::move(other.thread);
        return *this;
    }
};

//clase HiloBF

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

#endif // HILOS_H
