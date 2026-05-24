#include <iostream>
#include "leitura/read.h"
using namespace std;

int main() {
    Tabela grapes = carregarCSV("../grapes.csv");
    Tabela wines = carregarCSV("../wines.csv");

    cout << grapes.qtd_paginas << endl;
    cout << wines.qtd_paginas << endl;
}