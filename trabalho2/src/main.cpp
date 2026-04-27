#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "./hash/extensible.h"

using namespace std;

int main() {
    ifstream arquivo_in("../in.txt");
    ofstream arquivo_out("../out.txt");

    if (!arquivo_in.is_open()) {
        cout << "erro ao abrir in.txt na pasta raiz" << endl;
        return 1;
    }

    string linha_comando;
    ExtensibleHash* indice_hash = nullptr;

    while (getline(arquivo_in, linha_comando)) {
        if (linha_comando.empty()) continue;

        if (linha_comando.find("PG/") == 0) {
            // inicialização do Hash
            int pg_inicial = stoi(linha_comando.substr(3));
            indice_hash = new ExtensibleHash(pg_inicial);
            arquivo_out << linha_comando << "\n";
        } 
        else if (linha_comando.find("INC:") == 0) {
            int chave = stoi(linha_comando.substr(4));
            indice_hash->Inserir(chave, arquivo_out);
        }
        else if (linha_comando.find("REM:") == 0) {
            int chave = stoi(linha_comando.substr(4));
            indice_hash->Remover(chave, arquivo_out);
        }
        // identifica o comando original do arquivo, mas grava diferente dentro do Buscar
        else if (linha_comando.find("BUS=:") == 0) {
            int chave = stoi(linha_comando.substr(5));
            indice_hash->Buscar(chave, arquivo_out);
        }
    }

    // escreve a última linha pegando o pg final da classe
    if (indice_hash != nullptr) {
        indice_hash->imprimirEstadoDoDiretorio();
        arquivo_out << "P:/" << indice_hash->getPG() << "\n"; 
        delete indice_hash; //libera a memória alocada
    }

    arquivo_in.close();
    arquivo_out.close();

    cout << "verifique o arquivo out.txt" << endl;
    return 0;
}
