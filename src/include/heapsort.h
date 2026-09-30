#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Ordena em ordem crescente. Aceita NULL somente quando quantidade == 0.
 * Retorna false, sem alterar o vetor, para quantidade acima da capacidade.
 * Usa uma unica heap estatica interna; nao e reentrante.
 */
bool heapsort_ordenar(int32_t dados[], size_t quantidade);

#endif
