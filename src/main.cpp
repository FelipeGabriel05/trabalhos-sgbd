#include <iostream>
#include "./leitura/read.h"
#include "./structs/pages.h"
#include "./buffer/buffer.h"
using namespace std;

int main() {
    Buffer* buffer;
    int politica;
    cout << "1 = FIFO" << endl << "2 = LRU" << endl << "3 = MRU" << endl << "4 = CLOCK" << endl;
    cout << "Digite qual politica de substituicao deseja usar: " << endl;
    cin >> politica;

    switch (politica)
    {
    case 1:
        buffer = new Buffer(Politicas::FIFO);
        break;
    case 2:
        buffer = new Buffer(Politicas::LRU);
        break;
    case 3:
        buffer = new Buffer(Politicas::MRU);
        break;
    case 4:
        buffer = new Buffer(Politicas::CLOCK);
        break;
    default:
        cout << "Opcao invalida";
        break;
    }

    pages p1;
    p1.page = 2;
    p1.conteudo = lerPagina(2);

    pages p2;
    p2.page = 3;
    p2.conteudo = lerPagina(3);
    
    pages p3;
    p3.page = 4;
    p3.conteudo = lerPagina(4);

    buffer->Fetch(p1.page);
    buffer->Fetch(p2.page);
    buffer->Fetch(p3.page);
    buffer->Fetch(5);
    buffer->Fetch(6);

    buffer->Fetch(7);
    buffer->mostrar();
    
    buffer->Fetch(4);
    buffer->Fetch(4);
    buffer->Fetch(3);
    buffer->Fetch(5);
    buffer->Fetch(6);
    buffer->Fetch(7);
    buffer->Fetch(4);
    buffer->Fetch(10);
    buffer->mostrar();
    buffer->DisplayStats();
    delete buffer;
    return 0;
}