#include <iostream>
#include "leitura/read.h"
#include "buffer.h"
using namespace std;

int main() {
    Tabela grapes = carregarCSV("../grapes.csv");
    Tabela wines = carregarCSV("../wines.csv");

    cout << grapes.qtd_paginas << endl;
    cout << wines.qtd_paginas << endl;

    Buffer buffer;
    DiscoSimulado disco;

    // ordena pela coluna 0
    buffer.gerarRuns(grapes, disco, 0);
    disco.mostrarConteudoDisco(0);
    disco.mostrarRuns(0);
    buffer.mostrarBuffer();
}