#include "heapsort.h"
#include "heap.h"

#include <inttypes.h>
#include <stdio.h>

/* Entrada e saida da versao host. Esta camada sera trocada no embarque. */
static int32_t vetor[HEAP_CAPACIDADE];

int main(void)
{
    size_t quantidade;
    // printf("Informe quantidade: ");
    if (scanf("%zu", &quantidade) != 1 || quantidade > HEAP_CAPACIDADE) {
        fputs("Erro: informe uma quantidade entre 0 e 8000.\n", stderr);
        return 1;
    }

    for (size_t i = 0; i < quantidade; ++i) {
        if (scanf("%" SCNd32, &vetor[i]) != 1) {
            fprintf(stderr, "Erro: valor invalido na posicao %zu.\n", i);
            return 1;
        }
    }

    if (!heapsort_ordenar(vetor, quantidade)) {
        fputs("Erro: nao foi possivel ordenar.\n", stderr);
        return 1;
    }

    // printf("Vetor Ordenado: ");
    for (size_t i = 0; i < quantidade; ++i) {
        printf(i == 0 ? "%" PRId32 : " %" PRId32, vetor[i]);
    }
    putchar('\n');
    return 0;
}
