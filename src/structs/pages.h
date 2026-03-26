#ifndef PAGES_H
#define PAGES_H
#include <iostream>
using namespace std;

struct pages {
    int page;                       // page e o numero da linha 
    string conteudo;                // conteudo e o texto da linha  
    bool dirty_bit = false;         // Indica se a pagina foi modificada desde que foi trazida do disco para o pool de buffers
    int pin_count = 0;              // Quantas operações estão usando na página no momento.
    
    // int tempo_insercao;
    // int ultimo_acesso;
    // bool referencia;
};

#endif