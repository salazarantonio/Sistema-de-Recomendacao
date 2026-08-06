# Como baixar a branch `salazarismo`

## Clonar apenas a branch

```bash
git clone --branch salazarismo --single-branch https://github.com/salazarantonio/Sistema-de-Recomendacao.git
```

## Caso o repositório já tenha sido clonado

Atualize as referências remotas:

```bash
git fetch origin
```

Crie uma branch local baseada na remota:

```bash
git switch -c salazarismo origin/salazarismo
```

Se sua versão do Git não suporta `switch`:

```bash
git checkout -b salazarismo origin/salazarismo
```
