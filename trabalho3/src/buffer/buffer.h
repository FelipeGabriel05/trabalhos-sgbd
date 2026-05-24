#ifndef BUFFER_H
#define BUFFER_H
#include "../structs/dados.h"
#include "../disco/disco.h"
#include "../sort/sort.h"
#include <string>
#include <vector>
using namespace std;

class Buffer {

    public: 
        vector<Pagina> frames;
        size_t capacidade = 5;
        bool inserir(Pagina p);
        void limpar();
        bool cheio();
        int qtdFrames();
        void gerarRuns(Tabela &tabela, DiscoSimulado &disco, int indice_coluna);
        void mergeRuns(DiscoSimulado &disco);
        void mostrarBuffer();
        int FIFO();    
        void Evict();
};

#endif