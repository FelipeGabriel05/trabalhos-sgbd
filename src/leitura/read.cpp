#include "read.h"

// Funcao que busca uma pagina (linha) pelo numero
string lerPagina(int key) {
    ifstream arquivo("../bancodedados.csv");
    string linha;
    int contador = 0;

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return "";
    }

    // Ignora o cabecalho (primeira linha)
    getline(arquivo, linha);

    // Le linha por linha
    while (getline(arquivo, linha)) {
        contador++;

        if (contador == key) {
            arquivo.close();
            return linha;
        }
    }

    arquivo.close();
    return "Pagina nao encontrada";
}