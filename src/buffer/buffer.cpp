#include "buffer.h"
#include <random>
#include <climits>

std::random_device rd; 
// 2. Inicializa o gerador Mersenne Twister com a semente
std::mt19937 gen(rd()); 
// 3. Define o intervalo desejado entre 0 e 1
std::uniform_int_distribution<> distr(0 , 1); 

// Inserir pagina no buffer
bool Buffer::inserir(pages p) {
    if(paginas.size() < (size_t)capacidade) {
        paginas.push_back(p);
        cout << "Pagina: " << p.page <<". Inserido com sucesso" << endl;
        return true;
    } else {
        cout << "Buffer cheio!" << endl;
        return false;
    }
}

// Verifica se a pagina esta presente no buffer. Se estiver retorne ela
pages* Buffer::buscar(int key) {
    for(auto &p : paginas) {
        // ultimo acesso importante para MRU e LRU
        // referencia importante para CLOCK
        if(p.page == key) {
            cache_hit++;
            p.ultimo_acesso = ++contador_global;
            p.referencia = true;
            return &p;
        } 
    }
    cache_miss++;
    return nullptr;
}

// Mostra todas as paginas do meu buffer.
void Buffer::DisplayCache() {
    cout << "\n--- BUFFER ---\n";
    for (auto &p : paginas) {
        cout << "Chave: " << p.page 
             << " | Valor: " << p.conteudo 
             << " | Atualizacao: " << (p.dirty_bit ? "true" : "false")
             << endl;
    }
}

// Mostra o cache hit e o cache miss
void Buffer::DisplayStats() {
    cout << "Cache Hit => " << cache_hit << endl;
    cout << "Cache Miss => " << cache_miss << endl;
}

string Buffer::Fetch(int key) {
    pages* p = Buffer::buscar(key);
    if(p != nullptr) {
        cout << "Conteudo encontrado. Page = " << p->page << endl;
        return p->conteudo;
    } 
    
    pages pIn;
    pIn.page = key;
    pIn.conteudo = lerPagina(key);
    pIn.dirty_bit = distr(gen);
    pIn.ultimo_acesso = ++contador_global;
    pIn.referencia = true;

    // buffer com espaco
    if(Buffer::inserir(pIn)) {
        return pIn.conteudo;
    } else {
        // buffer cheio
        Buffer::Evict();
        Buffer::inserir(pIn);
        return pIn.conteudo;
    }
}

void Buffer::Evict() {
    int indice;
    switch (politicaAtual) {
        case Politicas::FIFO :
            indice = Buffer::FIFO();
            break;
        case Politicas::LRU :
            indice = Buffer::LRU();
            break;
        case Politicas::MRU :
            indice = Buffer::MRU();
            break;
        case Politicas::CLOCK :
            indice = Buffer::CLOCK();
            break;
        default:
            cout << "Erro na selecao da politica de substituicao";
            break;
    } 
    pages removida = paginas[indice];

    if(removida.dirty_bit) {
        cout << "pagina removida = " << removida.page;
        cout << "W ";
    }

    paginas.erase(paginas.begin() + indice);
    if(ponteiro_clock >= paginas.size()) {
        ponteiro_clock = 0;
    }
}

// Retornam o indice (Logo abaixo as politicas de substituicao)

// Remove o primeiro a ser colocado no pool do buffer. Primeiro a entrar e o primeiro a sair
int Buffer::FIFO() {
    return 0;
}

// Remove o menos acessado recentemente. (elimina quem ja faz tempo que nao foi acessado)
int Buffer::LRU() {
    int menor_acesso = INT_MAX;
    int indice = 0;
    int incrementa = 0; 
    for(auto &p : paginas) {
        if(p.ultimo_acesso < menor_acesso) {
            cout << "page = " << p.page << ". Ultimo acesso " << p.ultimo_acesso << endl;
            menor_acesso = p.ultimo_acesso;
            indice = incrementa;
        }
        incrementa++; 
    }
    cout << "indice escolhido para remocao = " << indice << endl;
    return indice;
}

// Remove o mais acessado recentemente. (elimina quem foi acessado recentemente)
int Buffer::MRU() {
    int maior_acesso = INT_MIN;
    int indice = 0;
    int incrementa = 0; 
    for(auto &p : paginas) {
        if(p.ultimo_acesso > maior_acesso) {
            cout << "page = " << p.page << ". Ultimo acesso " << p.ultimo_acesso << endl;
            maior_acesso = p.ultimo_acesso;
            indice = incrementa;
        }
        incrementa++; 
    }
    cout << "indice escolhido para remocao = " << indice << endl;
    return indice;
}

int Buffer::CLOCK() {
    if(paginas.empty()) return -1;
    while(true) {
        if(paginas[ponteiro_clock].referencia == false) {
            int indice = ponteiro_clock;
            ponteiro_clock = (ponteiro_clock + 1) % paginas.size();
            return indice;
        } else {
            paginas[ponteiro_clock].referencia = false;
            ponteiro_clock = (ponteiro_clock + 1) % paginas.size();
        }
    }
}