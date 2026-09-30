#include "heap.h"

void heap_inicializar(Heap *heap)
{
    heap->tamanho = 0;
}

bool heap_esta_vazia(const Heap *heap)
{
    return heap->tamanho == 0;
}

bool heap_inserir(Heap *heap, int32_t valor)
{
    if (heap->tamanho == HEAP_CAPACIDADE) {
        return false;
    }

    size_t indice = heap->tamanho++;
    while (indice > 0) {
        size_t pai = (indice - 1u) / 2u;
        if (heap->dados[pai] >= valor) {
            break;
        }
        heap->dados[indice] = heap->dados[pai];
        indice = pai;
    }
    heap->dados[indice] = valor;
    return true;
}

bool heap_remover_maximo(Heap *heap, int32_t *maior)
{
    if (heap_esta_vazia(heap)) {
        return false;
    }

    *maior = heap->dados[0];
    size_t restantes = --heap->tamanho;
    if (restantes == 0) {
        return true;
    }

    int32_t ultimo = heap->dados[restantes];
    size_t pai = 0;

    /* O limite fixo (8000) tambem impede overflow em 2*pai+1. */
    while (2u * pai + 1u < restantes) {
        size_t filho = 2u * pai + 1u;
        if (filho + 1u < restantes &&
            heap->dados[filho + 1u] > heap->dados[filho]) {
            ++filho;
        }
        if (ultimo >= heap->dados[filho]) {
            break;
        }
        heap->dados[pai] = heap->dados[filho];
        pai = filho;
    }

    heap->dados[pai] = ultimo;
    return true;
}
