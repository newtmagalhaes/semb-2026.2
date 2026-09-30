"""Compara a aplicacao C com sorted() do Python, de forma independente."""

from pathlib import Path
import random
import subprocess
import sys


EXECUTAVEL = Path(sys.argv[1]).resolve()
RAIZ = Path(__file__).resolve().parent.parent
GERADOR = random.Random(20260929)


def executar(valores: list[int]) -> list[int]:
    entrada = f"{len(valores)}\n" + " ".join(map(str, valores)) + "\n"
    processo = subprocess.run(
        [str(EXECUTAVEL)], input=entrada, text=True, capture_output=True,
        check=False, timeout=15,
    )
    assert processo.returncode == 0, processo.stderr
    return [int(parte) for parte in processo.stdout.split()]


casos = {
    "vazio": [],
    "unitario": [42],
    "exemplo": [4, -2, 7, 4, 0, -8, 3],
    "extremos": [-(2**31), 2**31 - 1, 0, -(2**31), 2**31 - 1],
    "crescente_8000": list(range(8000)),
    "decrescente_8000": list(range(8000, 0, -1)),
    "iguais_8000": [7] * 8000,
    "duplicados_8000": [i % 11 - 5 for i in range(8000)],
}
for numero, tamanho in enumerate([2, 3, 31, 257, 7999, 8000] * 3):
    casos[f"aleatorio_{numero}_{tamanho}"] = [
        GERADOR.randint(-(2**31), 2**31 - 1) for _ in range(tamanho)
    ]

assert (RAIZ / "dados/exemplo.out").read_text().split() == [
    str(item) for item in sorted(casos["exemplo"])
]
assert (RAIZ / "dados/exemplo.in").read_text() == (
    "7\n4 -2 7 4 0 -8 3\n"
)

for nome, dados in casos.items():
    obtido = executar(dados)
    esperado = sorted(dados)
    assert obtido == esperado, f"{nome}: {obtido[:12]} != {esperado[:12]}"
    print(f"OK {nome}: {len(dados)} elementos")

for entrada_invalida in ["8001\n", "2\n1 x\n", "-1\n"]:
    processo = subprocess.run(
        [str(EXECUTAVEL)], input=entrada_invalida, text=True,
        capture_output=True, check=False, timeout=15,
    )
    assert processo.returncode != 0, entrada_invalida

print(f"{len(casos)} casos de ordenacao e 3 entradas invalidas: OK")
