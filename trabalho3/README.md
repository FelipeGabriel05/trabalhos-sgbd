# Implementação do Operador Sort-Merge Join em C++

## Descrição

Este trabalho consiste na implementação do operador de junção **Sort-Merge Join (SMJ)** sem utilização de SGBDs, simulando o funcionamento interno de um banco de dados.

O sistema implementa:

- Leitura de arquivos `.csv`
- Estruturação das tabelas em páginas e tuplas
- Gerenciamento de buffer com limite de memória
- Ordenação externa utilizando Sort-Merge
- Intercalação externa (merge runs)
- Operação de junção Merge Join
- Simulação de disco em memória
- Exportação de resultados em `.txt` e `.csv`

---

# Estrutura do Projeto

```text
.
├── main.cpp
├── Makefile
│
├── leitura/
│   ├── read.cpp
│   └── read.h
│
├── structs/
│   └── dados.h
│
├── buffer/
│   ├── buffer.cpp
│   └── buffer.h
│
├── sort/
│   ├── sort.cpp
│   └── sort.h
│
├── disco/
│   ├── disco.cpp
│   └── disco.h
│
├── join/
│   ├── juncao.cpp
│   └── juncao.h
│
├── saida/
│   ├── runs_grapes_iniciais.txt
│   ├── runs_wines_iniciais.txt
│   ├── grapes_ordenada.csv
│   ├── wines_ordenada.csv
│   └── resultado_join.csv
│
├── grapes.csv
└── wines.csv
```

