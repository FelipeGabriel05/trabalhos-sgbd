#include <iostream>
#include "leitura/read.h"
#include "buffer/buffer.h"
#include "disco/disco.h"
#include "juncao/juncao.h"

using namespace std;

int main() {
    Tabela grapes = carregarCSV("../grapes.csv");
    Tabela wines  = carregarCSV("../wines.csv");

    cout << "\nTabelas carregadas com sucesso\n";

    // ─────────────────────────────────────────
    // INDICES DAS COLUNAS DE JUNCAO
    int indiceGrapes = grapes.esquema.nome_para_indice["chave_primaria"];
    int indiceWines  = wines.esquema.nome_para_indice["chave_estrangeira"];

    // ─────────────────────────────────────────
    // ORDENA TABELA GRAPES
    Buffer bufferGrapes;
    DiscoSimulado discoGrapes;

    bufferGrapes.gerarRuns(grapes, discoGrapes, indiceGrapes);
    discoGrapes.salvarRunsTXT("saida/runs_grapes_iniciais.txt", indiceGrapes);
    discoGrapes.mostrarRuns(indiceGrapes);
    bufferGrapes.mergeRuns(discoGrapes, indiceGrapes);

    cout << "\nGrapes totalmente ordenada\n";
    discoGrapes.mostrarRuns(indiceGrapes);

    discoGrapes.exportarCSV("saida/grapes_ordenada.csv", indiceGrapes);

    // ─────────────────────────────────────────
    // ORDENA TABELA WINES
    Buffer bufferWines;
    DiscoSimulado discoWines;

    bufferWines.gerarRuns(wines, discoWines, indiceWines);
    discoWines.salvarRunsTXT("saida/runs_wines_iniciais.txt", indiceWines);
    discoWines.mostrarRuns(indiceWines);
    bufferWines.mergeRuns(discoWines, indiceWines);

    cout << "\nWines totalmente ordenada\n";
    discoWines.mostrarRuns(indiceWines);

    discoWines.exportarCSV("saida/wines_ordenada.csv", indiceWines);

    // ─────────────────────────────────────────
    // CORRIGIDO: monta as tabelas ordenadas usando APENAS as páginas
    // da run final (runs[0].first até runs[0].second).
    // Antes usava disco.paginas inteiro, que inclui todas as runs
    // intermediárias — causando desordem e duplicatas no join.

    Tabela grapesOrdenada;
    grapesOrdenada.esquema = grapes.esquema;
    {
        int inicio = discoGrapes.runs[0].first;
        int fim    = discoGrapes.runs[0].second;
        for(int i = inicio; i <= fim; i++) {
            grapesOrdenada.paginas.push_back(discoGrapes.paginas[i]);
        }
        grapesOrdenada.qtd_paginas = grapesOrdenada.paginas.size();
    }

    Tabela winesOrdenada;
    winesOrdenada.esquema = wines.esquema;
    {
        int inicio = discoWines.runs[0].first;
        int fim    = discoWines.runs[0].second;
        for(int i = inicio; i <= fim; i++) {
            winesOrdenada.paginas.push_back(discoWines.paginas[i]);
        }
        winesOrdenada.qtd_paginas = winesOrdenada.paginas.size();
    }

    // ─────────────────────────────────────────
    // EXECUTA MERGE JOIN
    MergeJoin join;

    Tabela resultado = join.executar(winesOrdenada, grapesOrdenada, indiceWines, indiceGrapes);

    // ─────────────────────────────────────────
    // SALVA E MOSTRA RESULTADO JOIN
    join.salvarResultadoCSV(resultado, "saida/resultado_join.csv");

    cout << "\n RESULTADO JOIN \n";
    for(Pagina p : resultado.paginas) {
        for(Tupla t : p.tuplas) {
            for(string c : t.colunas) {
                cout << c << " | ";
            }
            cout << endl;
        }
    }

    cout << "\n====================================\n";
    cout << "\nArquivos salvos na pasta /saida\n";

    return 0;
}
