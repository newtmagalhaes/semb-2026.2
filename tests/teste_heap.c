#include "heap.h"
#include "heapsort.h"

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static Heap heap_de_teste;

static void checar_invariante(const Heap *heap)
{
    assert(heap->tamanho <= HEAP_CAPACIDADE);
    for (size_t filho = 1; filho < heap->tamanho; ++filho) {
        assert(heap->dados[(filho - 1u) / 2u] >= heap->dados[filho]);
    }
}

int main(void)
{
    heap_inicializar(&heap_de_teste);
    assert(heap_esta_vazia(&heap_de_teste));

    int32_t valor;
    assert(!heap_remover_maximo(&heap_de_teste, &valor));
    assert(heap_inserir(&heap_de_teste, INT32_MIN));
    assert(heap_inserir(&heap_de_teste, INT32_MAX));
    assert(heap_inserir(&heap_de_teste, INT32_MAX));
    checar_invariante(&heap_de_teste);
    assert(heap_remover_maximo(&heap_de_teste, &valor) && valor == INT32_MAX);
    assert(heap_remover_maximo(&heap_de_teste, &valor) && valor == INT32_MAX);
    assert(heap_remover_maximo(&heap_de_teste, &valor) && valor == INT32_MIN);
    assert(heap_esta_vazia(&heap_de_teste));

    heap_inicializar(&heap_de_teste);
    size_t contagem[101] = {0};
    for (size_t i = 0; i < HEAP_CAPACIDADE; ++i) {
        int32_t amostra = (int32_t)(i % 101u) - 50;
        assert(heap_inserir(&heap_de_teste, amostra));
        ++contagem[(size_t)(amostra + 50)];
        if (i % 127u == 0) {
            checar_invariante(&heap_de_teste);
        }
    }
    checar_invariante(&heap_de_teste);
    assert(heap_de_teste.tamanho == HEAP_CAPACIDADE);
    assert(!heap_inserir(&heap_de_teste, INT32_MAX));
    assert(heap_de_teste.tamanho == HEAP_CAPACIDADE);

    int32_t anterior = INT32_MAX;
    for (size_t i = 0; i < HEAP_CAPACIDADE; ++i) {
        assert(heap_remover_maximo(&heap_de_teste, &valor));
        assert(valor <= anterior);
        assert(valor >= -50 && valor <= 50);
        assert(contagem[(size_t)(valor + 50)] > 0);
        --contagem[(size_t)(valor + 50)];
        anterior = valor;
        if (i % 127u == 0) {
            checar_invariante(&heap_de_teste);
        }
    }
    assert(heap_esta_vazia(&heap_de_teste));
    assert(!heap_remover_maximo(&heap_de_teste, &valor));
    for (size_t i = 0; i < 101; ++i) {
        assert(contagem[i] == 0);
    }

    int32_t pequeno[] = {7, -1, 7, INT32_MIN, INT32_MAX};
    assert(heapsort_ordenar(pequeno, 5));
    assert(pequeno[0] == INT32_MIN && pequeno[1] == -1 &&
           pequeno[2] == 7 && pequeno[3] == 7 &&
           pequeno[4] == INT32_MAX);
    assert(!heapsort_ordenar(pequeno, HEAP_CAPACIDADE + 1u));
    assert(pequeno[0] == INT32_MIN); /* erro antes de ler/escrever */
    assert(!heapsort_ordenar(NULL, 1));
    assert(heapsort_ordenar(NULL, 0));

    printf("Heap: capacidade %u; sizeof(Heap) = %zu bytes; testes OK\n",
           HEAP_CAPACIDADE, sizeof(Heap));
    return 0;
}
