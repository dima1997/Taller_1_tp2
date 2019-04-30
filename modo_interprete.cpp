#include <vector>
#include <string>
#include "modo_interprete.h"
#include "interprete.h"

/*Inicializa un archivo*/
ArchivoLec::ArchivoLec(){}

/*Cierra un archivo*/
ArchivoLec::~ArchivoLec(){
    this->cerrar();
}

/*
PRE: Recibe una ruta a un archivo, y el modo en que dicho archivo
desea abrirse.
POST: Devuelve true si logro abrir el archivo, false en caso 
contrario.
*/
bool ArchivoLec::abrir(std::string &ruta){
    this->archLectura.open(ruta);
    return this->archLectura.is_open();
}

/*Cierra el archivo si es que esta abierto*/ //falta subir al sercom
void ArchivoLec::cerrar(){
    if (this->archLectura.is_open()) {
        this->archLectura.close();
    } 
}

/*
Devuelve un vector de caracteres (std::vector<char>) con 
todos los caracteres del archivo. Si el archivo esta cerrado
en modo lectura, entonces devuelve un vector vacio.
*/
std::vector<char> ArchivoLec::a_vector(){
    std::vector<char> texto(0);
    if (! this->archLectura.is_open()){
        return std::move(texto);
    }
    char caracterActual;
    this->archLectura.get(caracterActual);
    while (this->archLectura.good()){
        texto.push_back(caracterActual);
        this->archLectura.get(caracterActual);
    }
    return std::move(texto);
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
Ejecuta el modo inteprete.
Devuelve:
 0 si la ejecucion fue exitosa.
 1 si ocurrio algun error de intepretacion.
 2 si hubo algun error en la linea de comandos.
*/
int Interpretador::ejecutar(){
    if (!this->es_script_bf()) {
        return 2;
    }
    ArchivoLec arch;
    if (! arch.abrir(this->rutaScript)){
        return 1;
    }
    std::vector<char> scriptVec = arch.a_vector();
    arch.cerrar(); // Ya no lo necesito, asi que lo cierro.
    std::string nulo(ENTRADA_SALIDA_ESTANDAR);
    InterpreteBF interprete(nulo, nulo, std::move(scriptVec));
    return interprete.ejecutar() ? 0 : 1;
}
