## Trabalho 01 SGBD - Gerenciador de Buffer 

Alunos: FELIPE GABRIEL LIMA FLORINDO - 569760 e YASMIN SANTOS DE AMORIM - 566326  

## Descrição do Projeto

Esse projeto implemeta um simulador de Gerenciador de Buffer em C++. O sistema
gerencia o carregamento de páginas de um arquivo em disco ```bancodedados.csv```
para a memória principal, respeitando um limite máximo de 5 páginas.

O simulador suporta quatro políticas de substituição de páginas:
* FIFO (First-In, First-Out)
* LRU (Least Recently Used)
* MRU (Most Recently Used)
* CLOCK 

Estrutura de diretórios:

* ```src/```: Contém os arquivos de código-fonte (.cpp e .h), incluindo a lógica de leitura, estruturação das páginas, o buffer e o Makefile.
* ```bancodedados.csv```: Arquivo de texto que simula o disco contendo as páginas lidas sob demanda.
* ```Readme.txt```: ocumentação do projeto.

## Funcionalidades Implementadas

* Fetch(int key): Busca uma página específica pelo ID (número da linha). Se não estiver no buffer (Cache Miss), lê do disco e aplica uma flag de atualização randômica (TRUE/FALSE). Se estiver, acusa Cache Hit. Invoca o descarte (Evict) caso o buffer esteja cheio.
* Evict(): Remove uma página do buffer de acordo com a política escolhida. Se a página removida possuir a flag de atualização como TRUE, sinaliza a escrita com um "W" após a remoção.
* DisplayCache(): Imprime o estado atual do buffer (Chave da Página, Conteúdo em Texto e Flag de Atualização).
* DisplayStats(): Exibe o total de Cache Hits e Cache Misses da sessão executada.


## Como Compilar e Executar
O projeto utiliza um Makefile para facilitar a compilação. Pelo terminal, navegue até a pasta
raiz ou a pasta src e execute:
1. make 
2. ./programa 
3. Escolha qual política de substituição que deseja utilizar 




