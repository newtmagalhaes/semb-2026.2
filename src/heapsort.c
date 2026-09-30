#include "heapsort.h"
#include "heap.h"

/* Reserva estatica: nao ocupa a pilha e dispensa malloc. */
static Heap area_de_trabalho;

bool heapsort_ordenar(int32_t dados[], size_t quantidade)
{
    if (quantidade > HEAP_CAPACIDADE ||
        (quantidade != 0 && dados == NULL)) {
        return false;
    }

    heap_inicializar(&area_de_trabalho);

    for (size_t i = 0; i < quantidade; ++i) {
        if (!heap_inserir(&area_de_trabalho, dados[i])) {
            return false; /* impossivel com o limite ja verificado */
        }
    }

    for (size_t i = quantidade; i > 0; --i) {
        int32_t maior;
        if (!heap_remover_maximo(&area_de_trabalho, &maior)) {
            return false; /* impossivel apos quantidade insercoes */
        }
        dados[i - 1u] = maior;
    }
    return true;
}
