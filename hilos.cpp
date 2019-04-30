#include "hilos.h"

/*
PRE: Recibe una prioridad (size_t), y un 
interpreteBF (InterpreteBF) ya creado.
POST: Inicializa un Interprete con prioridad.
*/
InterpPriori::InterpPriori(size_t prioridad, InterpreteBF iBF)
        : prioridad(prioridad), interprete(std::move(iBF)){}
/*
Destruye un interprete con prioridad.
*/
InterpPriori::~InterpPriori(){}

/*
Ejecuta el interprete, que guarda.
Devuelve true si se ejecuto con exito, 
false en caso contrario.
*/
bool InterpPriori::ejecutar(){
    return this->interprete.ejecutar();
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
PrioriColaInterpProtegida::PrioriColaInterpProtegida(){
    this->colaAbierta = true;
}

/*Destruye una cola de interpretes con prioridad protegida.*/
PrioriColaInterpProtegida::~PrioriColaInterpProtegida(){}

/*
Se le notifica a la cola que no se encolaran mas elementos 
en ella.
*/
void PrioriColaInterpProtegida::cerrarCola(){
    std::unique_lock<std::mutex> lock(this->centinela);
    this->colaAbierta = false;
    this->continuar_desencolar.notify_all();
}

/*
PRE: Recibe un interprete con prioridad (InterpPriori *)
POST: Encola el interprete recibido en la cola.
*/
void PrioriColaInterpProtegida::encolar(InterpPriori *interpP){
    {
        std::unique_lock<std::mutex> lock(this->centinela);
        this->heapInterp.push(interpP);
    }
    this->continuar_desencolar.notify_one();
}

/*
Desencola un interprete con prioridad (InterpPriori *) y
lo devuelve.
Si el valor devuelto es NULL, es porque la cola esta cerrada
y vacia.
*/
InterpPriori *PrioriColaInterpProtegida::desencolar(){
    std::unique_lock<std::mutex> lock(this->centinela);
        while (this->heapInterp.empty() && this->colaAbierta){
        this->continuar_desencolar.wait(lock);
    }
    if (this->heapInterp.empty()){
        return NULL;
    }
    InterpPriori *iPrioriActual = this->heapInterp.top();
    this->heapInterp.pop(); 
    return iPrioriActual;
}

/*Devuelve true si la cola esta vacia, false en caso contrario.*/
bool PrioriColaInterpProtegida::estaVacia(){
    std::unique_lock<std::mutex> lock(this->centinela);
    return this->heapInterp.empty();
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

/*
Ejecuta interpretes que desencola de la cola prioritaria, hasta
que la misma este vacia.
*/
void HiloBF::run(){
    while (true){ 
        InterpPriori *iPriori = this->heapInterp.desencolar();  
        if (iPriori != NULL){
            iPriori->ejecutar();
            delete iPriori;
        } else {
            //Cola cerrada y vacia.
            //Ya no queda mas que ejecutar.
            break;
        }
    }
    return;
}
