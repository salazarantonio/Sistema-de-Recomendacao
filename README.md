# Sistema de Recomendação — Atividades 1, 2 e 3

Implementação das Atividades 1 (ListaCompras), 2 (Similaridade — versão
inicial) e 3 (Recomendação) do projeto, em estilo **procedural** (structs +
funções livres, sem classes, sem métodos, sem herança/polimorfismo). A
única "concessão" ao C++ são os containers da STL (`std::vector`,
`std::map`, `std::string`), como combinado — em C puro seria necessário
reimplementar listas dinâmicas e tabelas hash na mão.

## Estrutura de arquivos

```
projeto/
├── include/
│   ├── lista_compras.h     // struct ListaCompras + assinaturas (Atividade 1)
│   ├── similaridade.h      // struct Similaridade + assinaturas (Atividade 2)
│   └── recomendacao.h      // struct ItemRanking + assinaturas (Atividade 3)
├── src/
│   ├── lista_compras.cpp
│   ├── similaridade.cpp
│   └── recomendacao.cpp
├── testadores/
│   ├── teste_atividade1.cpp
│   ├── teste_atividade2.cpp
│   └── teste_atividade3.cpp
├── dados/
│   └── dados_clientes.csv  // amostra real extraída da base fornecida
├── Makefile
└── README.md
```

Cada atividade tem seu **próprio programa testador**, como pedido nos
entregáveis do documento do projeto.

## Compilando

```
make          # compila os três testadores
make clean    # remove os binários
```

Requer um compilador C++ com suporte a C++11 (g++ ou clang++).

## Executando

```
./teste_atividade1                 # usa dados/dados_clientes.csv por padrão
./teste_atividade1 caminho/outro.csv

./teste_atividade2
./teste_atividade3
```

Todos os três aceitam opcionalmente o caminho do CSV como argumento.

## Sobre os dados

O arquivo `dados/dados_clientes.csv` é uma **amostra real** (600 linhas)
extraída de um dos arquivos do `dados.zip` fornecido
(`dados_venda_cluster_1.csv`), contendo 485 clientes e 56 produtos. Foi
usada uma amostra (em vez da base completa) porque a Atividade 2 pede
explicitamente a versão **densa** e **não otimizada** do cálculo de
similaridade (matriz `n x n`, multiplicação `A x A^T` sem aproveitar
esparsidade — essa otimização é o assunto da Atividade 4, fora do escopo
pedido aqui). A base completa tem ~63 mil clientes distintos, o que
tornaria a matriz densa `n x n` proibitiva em memória (bilhões de células)
para essa versão inicial. Qualquer um dos arquivos originais do zip pode
ser usado normalmente, bastando passar o caminho como argumento — apenas
arquivos grandes vão demorar/consumir muita memória nesta versão.

Vale notar que o arquivo `dados_venda_cluster_0.csv` do zip usa `;` como
separador (em vez de `,`) — os testadores aqui pressupõem `,`, conforme
especificado no documento do projeto.

## Decisões de implementação

- **Leitura do CSV** (`lista_compras.cpp`): implementa um parser simples
  de CSV (RFC 4180) que respeita campos entre aspas — necessário porque
  vários nomes de produto contêm aspas literais (ex.: `TELEVISOR 32"...`),
  representadas no arquivo como `""`. A leitura é feita em uma única
  passada pelo arquivo (mais simples que o esquema de duas passadas da
  Figura 4 do documento, e equivalente em resultado): ao encontrar um
  código de cliente ou produto novo, um índice interno é criado na hora.
- **Compras duplicadas**: se o mesmo cliente aparece comprando o mesmo
  produto em mais de uma linha do CSV, o produto só é registrado uma vez
  na lista de compras (a estrutura representa o *conjunto* de produtos
  comprados, como pede a distância de Jaccard).
- **Similaridade** (`similaridade.cpp`): segue exatamente os passos do
  documento — matriz de compras densa `A`, transposta `A^T`, multiplicação
  padrão `I = A x A^T`, e depois `S[i][j] = 1 - I[i][j]/|P_i|`. É
  propositalmente a versão **não otimizada** (constrói a transposta e usa
  o algoritmo de multiplicação de três loops), pois a otimização que evita
  a transposta e aproveita a simetria é o objetivo da Atividade 4, que não
  foi pedida aqui.
- **"Cliente mais similar"**: como `S` é uma distância (0 = idêntico,
  1 = nenhum produto em comum), o cliente mais similar é o de **menor**
  valor de `S`, entre os que têm pelo menos um produto em comum
  (`S < 1`).
- **Recomendação** (`recomendacao.cpp`): segue os 4 passos do algoritmo da
  Seção 2.3 literalmente. Produtos já comprados pelo cliente são
  excluídos da lista de candidatos à recomendação.
