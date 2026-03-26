#ifndef PAGES_H
#define PAGES_H
#include <iostream>
using namespace std;

struct pages {
    int page;                       // page e o numero da linha 
    string conteudo;                // conteudo e o texto da linha  
    bool dirty_bit = false;         // Indica se a pagina foi modificada desde que foi trazida do disco para o pool de buffers
    
    int ultimo_acesso = 0;          // armazena o ultimo acesso a pagina. Logicamente para LRU define as paginas menos acessadas.
    int tempo_insercao = 0;
    // bool referencia;
};

#endif