#include <iostream>

#include "leitura/read.h"

#include "buffer/buffer.h"

// #include "structs/disco.h"

#include "juncao/juncao.h"

using namespace std;

int main() {


    Tabela grapes = carregarCSV("../grapes.csv");

    Tabela wines = carregarCSV("../wines.csv");

    cout << "\nTabelas carregadas\n";

    // Descobre indices da coluna

    int indiceGrapes = grapes.esquema.nome_para_indice["chave_primaria"];

    int indiceWines = wines.esquema.nome_para_indice["chave_estrangeira"];

    // Ordena grapes

    Buffer bufferGrapes;

    DiscoSimulado discoGrapes;

    bufferGrapes.gerarRuns(grapes, discoGrapes, indiceGrapes);

    discoGrapes.mostrarRuns(indiceGrapes);

    bufferGrapes.mergeRuns(discoGrapes, indiceGrapes);

    cout << "\nGRAPES ORDENADA\n";

    discoGrapes.mostrarRuns(indiceGrapes);

    // Ordena Wines

    Buffer bufferWines;

    DiscoSimulado discoWines;

    bufferWines.gerarRuns(wines, discoWines, indiceWines);

    discoWines.mostrarRuns(indiceWines);

    bufferWines.mergeRuns(discoWines, indiceWines);

    cout << "\nWINES ORDENADA\n";

    discoWines.mostrarRuns(indiceWines);

    // Tabela ordenadas

    Tabela winesOrdenada;
    winesOrdenada.paginas = discoWines.paginas;

    winesOrdenada.qtd_paginas = discoWines.paginas.size();

    Tabela grapesOrdenada;
    grapesOrdenada.paginas = discoGrapes.paginas;

    grapesOrdenada.qtd_paginas = discoGrapes.paginas.size();

    // EXECUTA MERGE JOIN

    MergeJoin join;

    Tabela resultado = join.executar(winesOrdenada, grapesOrdenada, indiceWines, indiceGrapes);

    // MOSTRA RESULTADO

    cout << "\n........ RESULTADO JOIN .........\n";

    for(Pagina p : resultado.paginas) {
        for(Tupla t : p.tuplas) {
            for(string c : t.colunas) {
                cout << c << " | ";
            }
            cout << endl;
        }
    }

    cout<< "\n..........................................\n";

    return 0;
}