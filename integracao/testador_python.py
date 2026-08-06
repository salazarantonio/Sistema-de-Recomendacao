import sys
import csv
import meu_solver


def ler_arquivo(caminho, delimitador=','):
    vetor_clientes = []
    nomes_produtos = []
    mapa_cliente = {}
    mapa_produto = {}
    compras_por_cliente = []

    with open(caminho, encoding='utf-8') as f:
        leitor = csv.reader(f, delimiter=delimitador)
        next(leitor)  # descarta cabecalho

        for linha in leitor:
            _data, cod_cliente, cod_produto, nome_produto = linha

            if cod_cliente not in mapa_cliente:
                indice_cliente = len(vetor_clientes)
                vetor_clientes.append(cod_cliente)
                mapa_cliente[cod_cliente] = indice_cliente
                compras_por_cliente.append([])
            else:
                indice_cliente = mapa_cliente[cod_cliente]

            if cod_produto not in mapa_produto:
                indice_produto = len(nomes_produtos)
                nomes_produtos.append(nome_produto)
                mapa_produto[cod_produto] = indice_produto
            else:
                indice_produto = mapa_produto[cod_produto]

            compras_por_cliente[indice_cliente].append(indice_produto)

    return vetor_clientes, nomes_produtos, mapa_cliente, compras_por_cliente


if __name__ == "__main__":
    # sys.argv equivale a argv do C: sys.argv[0] eh o nome do script,
    # len(sys.argv) equivale a argc.
    if len(sys.argv) < 2:
        print(f"Uso: python {sys.argv[0]} <caminho_do_csv> [delimitador]")
        sys.exit(1)

    caminho = sys.argv[1]
    delimitador = sys.argv[2] if len(sys.argv) > 2 else ','

    vetor_clientes, nomes_produtos, mapa_cliente, compras_por_cliente = ler_arquivo(caminho, delimitador)

    print(f"Clientes carregados: {len(vetor_clientes)}")
    print(f"Produtos carregados: {len(nomes_produtos)}")

    meu_solver.carregar_dados(compras_por_cliente, len(nomes_produtos))
    meu_solver.calcular_similaridade()

    k = 5
    count = 0
    while count < 3:
        codigo = input("Digite o codigo original de um cliente: ").strip()
        if codigo not in mapa_cliente:
            print(f"Cliente {codigo} nao encontrado.\n")
            count += 1
            continue

        indice_cliente = mapa_cliente[codigo]
        recomendacoes = meu_solver.recomendar_produtos(cliente_id=indice_cliente, k=k)

        print(f"Top {k} produtos recomendados para o cliente {codigo}:")
        if not recomendacoes:
            print("  (nenhum produto disponivel para recomendar)")
        else:
            for id_produto, ranking in recomendacoes:
                print(f"  - {nomes_produtos[id_produto]} (ranking: {ranking})")
        print()
        count += 1

    meu_solver.liberar()
    print("Encerrando programa.")