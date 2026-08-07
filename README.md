# Sistema de Recomendação

Implementação de um sistema de recomendação para clientes de lojas de varejo utilizando filtragem colaborativa baseada em usuários e similaridade de Jaccard. O projeto foi desenvolvido em C++ como parte da disciplina de Programação Estruturada. :contentReference[oaicite:0]{index=0}

## Funcionalidades

- Leitura de base de compras em formato CSV.
- Construção da lista de compras dos clientes.
- Mapeamento entre códigos originais e índices internos.
- Cálculo da matriz de similaridade entre clientes.
- Recomendação de produtos utilizando o método da vizinhança.
- Multiplicação de matrizes otimizada.
- Implementação utilizando matrizes esparsas (CSR).
- Integração dos módulos C++ com Python utilizando pybind11. :contentReference[oaicite:1]{index=1} :contentReference[oaicite:2]{index=2} :contentReference[oaicite:3]{index=3} :contentReference[oaicite:4]{index=4}

## Estrutura

```text
.
├── 1a5/
├── dados/
├── integracao/
├── gitignore
├── LICENSE
└── README
```

## Requisitos

- C++17 ou superior
- GCC, Clang ou MSVC
- Git

## Compilação e execução

[RUN](integracao/howToRun.txt)

## Clonar a branch `salazarismo`

```bash
git clone --branch salazarismo --single-branch https://github.com/main/Sistema-de-Recomendacao.git
```

Caso o repositório já esteja clonado:

```bash
git fetch origin
git switch -c salazarismo origin/salazarismo
```

## Algoritmo

O sistema utiliza filtragem colaborativa baseada em usuários.

1. Constrói a matriz de compras.
2. Calcula a interseção entre clientes.
3. Computa a similaridade de Jaccard.
4. Ordena os vizinhos mais similares.
5. Ranqueia os produtos ainda não adquiridos pelo cliente.
6. Retorna os `k` produtos mais recomendados. :contentReference[oaicite:5]{index=5}

## Projeto

O desenvolvimento foi dividido em seis módulos:

- ListaCompras
- Similaridade
- Recomendação
- Multiplicação eficiente
- Matrizes esparsas (CSR)
- Integração C++/Python com pybind11 :contentReference[oaicite:6]{index=6}

## Licença

[Licença](LICENSE)

##
Projeto acadêmico desenvolvido para a disciplina de Programação Estruturada - UFPB 2026.1
