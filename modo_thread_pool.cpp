#include <iostream>
#include <vector>
#include <string>
#include "modo_thread_pool.h"
//Spliter

/*Crea un spliter (un separador de strings)*/
Spliter::Spliter(){}

/*Destruye un spliter*/
Spliter::~Spliter(){}

/*
PRE: Recibe un dos referencias a string (std::string &): 
una cadena, y un separador. 
POST: Devuelve un vector de strings (std::vector<std::string>)
donde cada string es una parte de la cadena recibidida que se 
separa del resto de la misma, donde aparece el separador 
recibido (sin incluirlo). 
*/
std::vector<std::string> Spliter::split(std::string &cad, std::string &sep){
    std::vector<std::string> splitVec(0);
    std::string copiaCad(cad);
    std::string parteCadena;
    size_t pos;
    while ((pos = copiaCad.find(sep)) != std::string::npos) {
        parteCadena = copiaCad.substr(0, pos);
        splitVec.push_back(parteCadena);
        copiaCad.erase(0, pos + sep.size());
    }
    splitVec.push_back(copiaCad); 
    return std::move(splitVec);
}

//Modo ThreadPool

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
    if (posParent1 == std::string::npos){
        return NULL;
    }
    std::string lineaSinParentesis = linea.substr(posParent1);
    Spliter spliter; 
    std::string separador(", ");
    std::vector<std::string> lineaSplit;
    lineaSplit = spliter.split(lineaSinParentesis, separador);
    if (lineaSplit.size() != 5){
        return NULL;
    }
    size_t prioridad = stoi(lineaSplit[1]);
    std::string rutaEntrada = std::move(lineaSplit[2]);
    std::string rutaSalida = std::move(lineaSplit[3]);
    std::string script = std::move(lineaSplit[4]);
    size_t largoScript = script.size();
    std::vector<char> scriptVector(0);
    for (size_t i = 0; i < largoScript; i++){
        (scriptVector).push_back(script[i]);
    }
    InterpreteBF interp(rutaEntrada, rutaSalida, std::move(scriptVector)); 
    InterpPriori *iPriori;
    iPriori = new InterpPriori(prioridad, std::move(interp));
    return iPriori;
}

/*
Ejecuta el modo thread-pool.
Devuelve:
 0 si la ejecucion fue exitosa.
 1 si ocurrio algun error de intepretacion.
 */
int ThreadPool::ejecutar(){
    PrioriColaInterpProtegida heapInterpretes;
    std::vector<Thread*> hilos;
    for (size_t i = 0; i < this->cantidadHilos; ++i) {
        hilos.push_back(new HiloBF(heapInterpretes));
    }
    for (size_t i = 0; i < this->cantidadHilos; ++i) {
        hilos[i]->start();
    }
    std::string linea;
    while (std::cin.good()){  
        getline(std::cin, linea, ')');
        InterpPriori *interp = this->procesar_linea(linea);
        if (interp == NULL){
            continue; 
        }
        heapInterpretes.encolar(interp);
    }
    heapInterpretes.cerrarCola();
    for (size_t i = 0; i < this->cantidadHilos; ++i) {
        hilos[i]->join();
        delete hilos[i];
    }
    int valorRetorno = 0;
    // En caso de que haya ocurrido algun error 
    // y no se haya desencolado todo, liberamos
    // el resto de la memoria restante.
    if (!heapInterpretes.estaVacia()){
        valorRetorno = 1;
    }
    while (!heapInterpretes.estaVacia()){
        InterpPriori *iPriori = heapInterpretes.desencolar();
        if (iPriori != NULL){
            delete iPriori;
        }  
    }
    return valorRetorno;
}
