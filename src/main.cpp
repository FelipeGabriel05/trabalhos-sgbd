#include <iostream>
#include "./leitura/read.h"
#include "./structs/pages.h"
#include "./buffer/buffer.h"
using namespace std;

int main() {
    // Buffer buffer(Politicas::FIFO);
    Buffer buffer(Politicas::LRU);
    // Buffer buffer(Politicas::MRU);
    // Buffer buffer(Politicas::CLOCK);

    pages p1;
    p1.page = 2;
    p1.conteudo = lerPagina(2);

    pages p2;
    p2.page = 3;
    p2.conteudo = lerPagina(3);
    
    pages p3;
    p3.page = 4;
    p3.conteudo = lerPagina(4);

    buffer.Fetch(p1.page);
    buffer.Fetch(p2.page);
    buffer.Fetch(p3.page);
    buffer.Fetch(5);
    buffer.Fetch(6);
    buffer.Fetch(7);
    buffer.mostrar();
    buffer.Fetch(3);
    buffer.Fetch(3);
    buffer.Fetch(3);
    buffer.Fetch(5);
    buffer.Fetch(6);
    buffer.Fetch(7);
    buffer.Fetch(4);
    buffer.Fetch(10);
    buffer.mostrar();
    buffer.DisplayStats();
    return 0;
}