struct Nodo {
    public:
    char *caracter;
    Nodo *anterior; //struct Nodo
    Nodo *siguiente; //struct Nodo

    Nodo();
    ~Nodo();
};

struct ListaDoble {
    public:
    Nodo *primero;
    
    ListaDoble();
    ~ListaDoble();
    insertar_primero(char caracter);
};

struct IteradorListaDoble {
    public:
    Nodo *actual;
    //Nodo *anterior;

    IteradorListaDoble(ListaDoble lista);
    ~IteradorListaDoble();
    bool avanzar();
    bool retroceder();
    bool escribir(char caracter);
    char* leer();

}