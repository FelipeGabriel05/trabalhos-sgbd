# Implementação de Hash Extensível em C++

## Visão Geral

Este projeto implementa uma estrutura de dados de **Hash Extensível**, com suporte a operações dinâmicas de inserção, remoção e busca, além de persistência em disco.

O objetivo é simular o funcionamento de um índice baseado em hashing que cresce dinamicamente conforme a necessidade, mantendo eficiência nas operações.

---

## Funcionalidades Implementadas

### 1. Inicialização do Hash

* Comando: `PG/x`
* Define a **Profundidade Global (PG)** inicial.
* Cria o diretório com tamanho `2^PG`.
* Inicializa um único bucket com **Profundidade Local (PL = 0)**.
* Todos os ponteiros do diretório apontam inicialmente para esse bucket.

---

### 2. Função Hash

```cpp
return chave & ((1 << profundidade) - 1);
```

* Utiliza os **bits menos significativos** da chave.
* Gera o índice no diretório com base na profundidade global.

---

### 3. Inserção (`INC:x`)

* Insere a chave no bucket correspondente.
* Evita duplicatas.
* Mantém as chaves ordenadas no bucket.

#### Tratamento de Overflow:

* Capacidade do bucket: **5 chaves**
* Quando excede:

  * Se `PL < PG` → realiza **split**
  * Se `PL == PG` → **duplica o diretório** (PG++)
  * Depois executa o split

---

### 4. Split de Bucket

* Cria um novo bucket.
* Incrementa a profundidade local (PL).
* Redistribui as chaves com base em um novo bit:

```cpp
(k >> bit_verificacao) & 1
```

* Atualiza os ponteiros do diretório.

---

### 5. Busca (`BUS=:x`)

* Localiza o bucket via hash.
* Verifica se a chave existe.
* Retorna:

```txt
BUS=:chave/1  → encontrada
BUS=:chave/0  → não encontrada
```

---

### 6. Remoção (`REM:x`)

* Remove a chave do bucket, se existir.
* Atualiza o arquivo do bucket.
* Não realiza merge de buckets (simplificação).

---

## Persistência em Disco

### Buckets

* Arquivos: `bucket_ID.txt`
* Estrutura:

```
PL
chave1 chave2 chave3 ...
```

---

### Diretório

* Arquivo: `diretorio.txt`
* Estrutura:

```
PG
id_bucket id_bucket id_bucket ...
```

---

## Fluxo Geral

```txt
Entrada (in.txt)
    ↓
Leitura linha a linha
    ↓
Execução (Inserir, Buscar, Remover)
    ↓
Atualização do hash
    ↓
Saída (out.txt)
```

---

## Estrutura Conceitual

* **PG (Profundidade Global):**

  * Define o tamanho do diretório
  * `2^PG` entradas

* **PL (Profundidade Local):**

  * Define quantos bits um bucket distingue
  * Cada bucket pode ter PL diferente

---

## Regras Importantes

* Sempre vale:

```
PL ≤ PG
```

* Número de ponteiros para um bucket:

```
2^(PG - PL)
```

---

## Exemplo de Comandos

```
PG/2
INC:1
INC:2
INC:3
INC:4
INC:5
BUS=:3
REM:2
BUS=:2
```

---

## Saída Esperada

```
PG/2
INC:1/2,0
INC:2/2,0
...
BUS=:3/1
REM:2/1,2,1
BUS=:2/0
P:/2
```
