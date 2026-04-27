#ifndef EXTENSIBLE_H
#define EXTENSIBLE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../structs/bucket.h"

using namespace std;

class ExtensibleHash {
private:
    int PG;                // Profundidade Global do diretório
    vector<int> diretorio; // Vetor que atua como o Diretório (armazena os IDs dos buckets apontados)
    int proximo_id_bucket; // Variável de controle para gerar nomes únicos para os arquivos txt dos buckets

    // gerenciamento de disco
    Bucket lerBucketDoDisco(int id);
    void salvarBucketNoDisco(const Bucket& b);
    void salvarDiretorioNoDisco(); // cria o diretorio.txt para garantir persistência total
    
    int calcularHash(int chave, int profundidade);
    void realizarSplit(int bucket_id);

public:
    ExtensibleHash(int pg_inicial);
    
    // operações principais 
    void Inserir(int chave, ofstream& arquivo_out);
    void Remover(int chave, ofstream& arquivo_out);
    void Buscar(int chave, ofstream& arquivo_out);
    
    
    int getPG() { return PG; }

    void imprimirEstadoDoDiretorio() {
        cout << "\nEstado atual do hash" << endl;
        cout << "Profundidade Global : " << PG << endl;
        for (size_t i = 0; i < diretorio.size(); i++) {
            cout << "Indice [" << i << "] (Binario " << i << ") aponta para Bucket " << diretorio[i] << endl;
        }
       
    }
};

#endif
