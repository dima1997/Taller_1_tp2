#include <string>
#include "modo_interprete.h"
#include "modo_thread_pool.h"

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
    if (modo.compare("thread-pool") == 0){
        std::string cantidadHilosCadena(argv[2]);
        size_t cantidadHilos = stoi(cantidadHilosCadena);
        ThreadPool modoThreadPool(cantidadHilos);
        return modoThreadPool.ejecutar();
    }
    return 2;
}
