#include "buffer.h"
#include <random>

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
        cout << "Buffer cheio! (precisa implementar evict)" << endl;
        return false;
    }
}

// Verifica se a pagina esta presente no buffer. Se estiver retorne ela
pages* Buffer::buscar(int key) {
    for(auto &p : paginas) {
        if(p.page == key) {
            cache_hit++;
            p.ultimo_acesso++;
            return &p;
        } 
    }
    cache_miss++;
    return nullptr;
}

// Mostra todas as paginas do meu buffer.
void Buffer::mostrar() {
    cout << "\n--- BUFFER ---\n";
    for (auto &p : paginas) {
        cout << "Page: " << p.page 
             << " | Conteudo: " << p.conteudo 
             << " | Dirty: " << p.dirty_bit 
             << endl;
    }
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
        // case Politicas::MRU :
        //     indice = Buffer::MRU();
        //     break;
        // case Politicas::CLOCK :
        //     indice = Buffer::CLOCK();
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
}

// Retorna indice
int Buffer::FIFO() {
    return 0;
}

int Buffer::LRU() {
    int menor_acesso = 2.14e9;
    int indice;
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

// Mostra o cache hit e o cache miss
void Buffer::DisplayStats() {
    cout << "Cache Hit => " << cache_hit << endl;
    cout << "Cache Miss => " << cache_miss << endl;
}