#ifndef MODO_THREAD_POOL_H
#define MODO_THREAD_POOL_H
#include <vector>
#include <string>
#include "hilos.h"

//class Spliter

class Spliter {
    public:
    /*Crea un spliter (un separador de strings)*/
    Spliter();

    /*Destruye un spliter*/
    ~Spliter();

    /*
    PRE: Recibe un dos referencias a string (std::string &): 
    una cadena, y un separador. 
    POST: Devuelve un vector de strings (std::vector<std::string>)
    donde cada string es una parte de la cadena recibidida que se 
    separa del resto de la misma, donde aparece el separador 
    recibido (sin incluirlo). 
    */
    std::vector<std::string> split(std::string &cad, std::string &sep);
};

class ThreadPool{
    private:
    size_t cantidadHilos;
    
    public:
    /*Inicializa un ThreadPool de cantidad de hilos (size_t) recibida*/
    explicit ThreadPool(size_t cantidadHilos);
    
    /*Detruye un ThreadPool*/
    ~ThreadPool();
    
    /*
    PRE: Recibe una linea (&string) a procesar.
    POST: Devuelve un interprete con prioridad (InterPrior *), 
    listo para ser ejecutado.
    Queda a responsabilidad del usuario liberar la memoria 
    reservada por medio de la sentencia delete.
    */
    InterpPriori *procesar_linea(std::string &linea);
    
    /*
    Ejecuta el modo thread-pool.
    Devuelve:
     0 si la ejecucion fue exitosa.
     1 si ocurrio algun error de intepretacion.
     */
    int ejecutar();    
};

#endif // MODO_THREAD_POOL_H
