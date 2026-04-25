#include "extensible.h"
#include "../leitura/read.h"
#include <iostream>
#include <cmath>
#include <algorithm> 

using namespace std;

ExtensibleHash::ExtensibleHash(int pg_inicial) {
    this->PG = pg_inicial;
    
    // o tamanho inicial do diretório é 2 elevado a PG
    int tamanho_diretorio = 1 << PG; 
    this->proximo_id_bucket = 0;

    // Cria o primeiro bucket no disco
    Bucket bucket_inicial;
    bucket_inicial.id = proximo_id_bucket++;
    bucket_inicial.PL = 0; //Inicia com Profundidade Local 0
    salvarBucketNoDisco(bucket_inicial);

    // Inicialmente, todos os ponteiros do diretório apontam para o único bucket existente
    for (int i = 0; i < tamanho_diretorio; i++) {
        diretorio.push_back(bucket_inicial.id);
    }
    
    // diretório  persistido em disco
    salvarDiretorioNoDisco(); 
}

// calculo da função hash 
int ExtensibleHash::calcularHash(int chave, int profundidade) {
    // Retorna apenas os 'profundidade' bits menos significativos da chave.
    return chave & ((1 << profundidade) - 1);
}

// gerenciamento de memória e disco  

Bucket ExtensibleHash::lerBucketDoDisco(int id) {
    Bucket b;
    b.id = id;
    b.PL = 0;
    
    // Abre o arquivo de texto específico deste bucket
    ifstream file("bucket_" + to_string(id) + ".txt");
    if (file.is_open()) {
        file >> b.PL; // Lê a Profundidade Local
        int chave;
        while (file >> chave) {
            b.chaves.push_back(chave); // Carrega as chaves para a RAM
        }
        file.close();
    }
    return b;
}

// Recebe o bucket por referência constante (const &) para evitar cópias caras na RAM
void ExtensibleHash::salvarBucketNoDisco(const Bucket& b) {
    ofstream file("bucket_" + to_string(b.id) + ".txt");
    file << b.PL << "\n";
    
    // Grava as chaves separadas por espaço
    for (const int& k : b.chaves) {
        file << k << " ";
    }
    file.close();
}

// Salva a estrutura do diretório para garantir que o índice seja recuperável
void ExtensibleHash::salvarDiretorioNoDisco() {
    ofstream file("diretorio.txt");
    file << PG << "\n"; // Primeira linha salva a profundidade global 
    for (size_t i = 0; i < diretorio.size(); i++) {
        file << diretorio[i] << " "; // Salva os IDs dos buckets apontados
    }
    file << "\n";
    file.close();
}

// operação de busca
void ExtensibleHash::Buscar(int chave, ofstream& arquivo_out) {
    int indice_hash = calcularHash(chave, PG);
    
    // evita crash caso o índice calculado estoure o vetor
    if (indice_hash >= (int)diretorio.size()) return;
    
    int bucket_id = diretorio[indice_hash];
    Bucket b = lerBucketDoDisco(bucket_id); // Traz o bucket para a memória
    
    int qtd_encontradas = 0;
    for (const int& k : b.chaves) {
        if (k == chave) {
            qtd_encontradas = 1;
            string texto_recuperado = lerPaginaCSV(chave); 
            // cout << ">> Texto da linha " << chave << ": " << texto_recuperado << endl;
            break;
        }
    }
    
    // Grava no log out.txt no formato BUS:x/qtd
    arquivo_out << "BUS:" << chave << "/" << qtd_encontradas << "\n";
}


void ExtensibleHash::Remover(int chave, ofstream& arquivo_out) {
    int indice_hash = calcularHash(chave, PG);
    if (indice_hash >= (int)diretorio.size()) return;
    
    int bucket_id = diretorio[indice_hash];
    Bucket b = lerBucketDoDisco(bucket_id);
    int qtd_removidas = 0;
    
    // Procura a chave no vetor. Se encontrar, apaga (erase).
    auto it = find(b.chaves.begin(), b.chaves.end(), chave);
    if (it != b.chaves.end()) {
        b.chaves.erase(it);
        qtd_removidas = 1;
        salvarBucketNoDisco(b); // Sobrescreve o arquivo no disco sem a chave apagada
    }
    
    arquivo_out << "REM:" << chave << "/" << qtd_removidas << "," << PG << "," << b.PL << "\n";
}

void ExtensibleHash::Inserir(int chave, ofstream& arquivo_out) {
    int indice_hash = calcularHash(chave, PG);
    if (indice_hash >= (int)diretorio.size()) return;
    
    int bucket_id = diretorio[indice_hash];
    Bucket b = lerBucketDoDisco(bucket_id);
    
    //  Prevenção de duplicatas
    if (find(b.chaves.begin(), b.chaves.end(), chave) != b.chaves.end()) {
        arquivo_out << "INC:" << chave << "/" << PG << "," << b.PL << "\n";
        return; 
    }

   
    // Se o bucket estiver cheio (5 elementos), precisamos fazer o Split
    // Usamos while para garantir que continuaremos dividindo, se após uma divisão todas 
    // as chaves  ainda possuírem o mesmo bit de hash, o bucket 
    // continuará cheio e precisará ser dividido novamente em um nível mais profundo
    while (b.chaves.size() >= 5) {
        
        // Se a Profundidade Local for igual a Global, o diretório esgotou sua capacidade
        if (b.PL == PG) {
            int tamanho_antigo = diretorio.size();
            // Duplica o diretório, os novos ponteiros apontam para os mesmos buckets dos originais
            for (int i = 0; i < tamanho_antigo; i++) {
                diretorio.push_back(diretorio[i]);
            }
            PG++; // Aumenta a Profundidade Global
            arquivo_out << "DUP DIR:/" << PG << "," << b.PL << "\n";
        }
        
        // Realiza a divisão do bucket que estourou a capacidade
        realizarSplit(b.id, arquivo_out);
        
        // Como a estrutura mudou,recalculamos para qual gaveta esta chave específica deve ir agora
        indice_hash = calcularHash(chave, PG);
        bucket_id = diretorio[indice_hash];
        b = lerBucketDoDisco(bucket_id);
    }

    // quando saímos do laço, temos certeza matemática de que há espaço no bucket b
    b.chaves.push_back(chave);
    
    // mantém as chaves ordenadas dentro do bucket
    sort(b.chaves.begin(), b.chaves.end()); 
    
    salvarBucketNoDisco(b);
    arquivo_out << "INC:" << chave << "/" << PG << "," << b.PL << "\n";
}

// split
void ExtensibleHash::realizarSplit(int bucket_id, ofstream& arquivo_out) {
    Bucket bucket_antigo = lerBucketDoDisco(bucket_id);
    
    // Cria um bucket novo
    Bucket bucket_novo;
    bucket_novo.id = proximo_id_bucket++;
    
    // Ambos os buckets ganham +1 de profundidade local
    bucket_antigo.PL++;
    bucket_novo.PL = bucket_antigo.PL;
    
    vector<int> chaves_remanescentes;
    
    int bit_verificacao = bucket_novo.PL - 1;
    
    // Redistribuição das chaves antigas
    for (const int& k : bucket_antigo.chaves) {
        // Usa shift bitwise >> para olhar especificamente para o novo bit
        // Se o bit for 1, a chave se muda para a casa nova. Se for 0, fica na antiga
        if ((k >> bit_verificacao) & 1) {
            bucket_novo.chaves.push_back(k); 
        } else {
            chaves_remanescentes.push_back(k); 
        }
    }
    bucket_antigo.chaves = chaves_remanescentes;
    
    // Ordena os dois buckets após a confusão da mudança
    sort(bucket_antigo.chaves.begin(), bucket_antigo.chaves.end());
    sort(bucket_novo.chaves.begin(), bucket_novo.chaves.end());
    
    // Salva ambos no disco para liberar a RAM
    salvarBucketNoDisco(bucket_antigo);
    salvarBucketNoDisco(bucket_novo);
    
    // atualiza os ponteiros do diretório que antes apontavam para o bucket antigo
    // os índices que tiverem o bit de verificação igual a 1 passam a apontar para o novo bucket
    for (size_t i = 0; i < diretorio.size(); i++) {
        if (diretorio[i] == bucket_id) {
            if ((i >> bit_verificacao) & 1) {
                diretorio[i] = bucket_novo.id;
            }
        }
    }
    
    // como o diretório mudou de configuração, reescrevemos seu backup no disco
    salvarDiretorioNoDisco(); 
}
