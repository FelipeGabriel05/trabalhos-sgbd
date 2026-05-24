#ifndef DISCO_H
#define DISCO_H
#include "dados.h"

class DiscoSimulado {
    public:
        // inicio e fim de cada run
        vector<pair<int,int>> runs;
        vector<Pagina> paginas;
        void salvarPagina(Pagina p);
        Pagina lerPagina(int indice);
        int qtdPagina();
        void mostrarEstadoDisco();
        void mostrarConteudoDisco(int indice_coluna);
        void mostrarRuns(int indice_coluna);
};  

#endif