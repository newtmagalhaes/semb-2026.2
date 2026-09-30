#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HEAP_CAPACIDADE 8000u

/* Max-heap binaria, com capacidade fixa e sem alocacao dinamica. */
typedef struct {
    int32_t dados[HEAP_CAPACIDADE];
    size_t tamanho;
} Heap;

/* Todas as funcoes exigem um ponteiro Heap valido. */
void heap_inicializar(Heap *heap);
bool heap_esta_vazia(const Heap *heap);
bool heap_inserir(Heap *heap, int32_t valor); /* false se cheia */
bool heap_remover_maximo(Heap *heap, int32_t *maior); /* false se vazia */

#endif
