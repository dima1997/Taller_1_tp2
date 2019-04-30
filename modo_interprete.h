#ifndef MODO_INTERPRETE_H
#define MODO_INTERPRETE_H
#include <fstream>
#include <string>
#include <vector>

//class Archivo

class ArchivoLec {
    private:
    std::ifstream archLectura;

    public:
    /*Inicializa un archivo en modo lectura*/
    ArchivoLec();
    
    /*Cierra un archivo*/
    ~ArchivoLec();
    
    /*
    PRE: Recibe una ruta a un archivo, y el modo en que dicho archivo
    desea abrirse.
    POST: Devuelve true si logro abrir el archivo, false en caso 
    contrario.
    */
    bool abrir(std::string &ruta);
    
    /*Cierra el archivo si es que esta abierto*/
    void cerrar();

    /*
    Devuelve un vector de caracteres (std::vector<char>) con 
    todos los caracteres del archivo. Si el archivo esta cerrado
    en modo lectura, entonces devuelve un vector vacio.
    */
    std::vector<char> a_vector();
};

//class Intepretador

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

#endif // MODO_INTERPRETE_H
