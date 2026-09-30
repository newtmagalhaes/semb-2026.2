# T1 — ordenação com max-heap de capacidade fixa

Primeira versão para **host**, de acordo com a proposta de heap separada que a dupla descreveu e o professor aceitou excepcionalmente. O núcleo em C não usa recursão nem `malloc`; a leitura por `scanf` e a impressão ficam apenas na interface do host. O código é original e foi escrito a partir das operações descritas nas fontes ao fim do arquivo.

## Resumo em cinco linhas para o documento da turma

> O programa ordena um lote de inteiros usando uma max-heap binária de capacidade fixa.
> Recebe um vetor com até 8.000 valores `int32_t` e armazena os valores em uma heap separada.
> Remove repetidamente o maior valor e preenche o vetor de saída de trás para frente, em ordem crescente.
> A construção por inserções e as remoções custam `O(n log n)` no pior caso; a heap auxiliar usa `O(n)` de memória.
> Pode organizar lotes de amostras ou prioridades inteiras em sistemas embarcados que disponham da RAM necessária.

## Estrutura e interface

| Arquivo | Responsabilidade |
| --- | --- |
| `include/heap.h`, `src/heap.c` | `Heap` com `dados[8000]` e `tamanho`; inicializar, inserir, remover máximo, consultar se está vazia. |
| `include/heapsort.h`, `src/heapsort.c` | `heapsort_ordenar(vetor, quantidade)`: copia para a heap e escreve o resultado crescente no mesmo vetor. |
| `src/main.c` | Lê a entrada textual e exibe o resultado, apenas no host. |

A heap é uma árvore binária armazenada em vetor: o pai da posição `i > 0` fica em `(i-1)/2`; os filhos de `i` ficam em `2*i+1` e `2*i+2`. Em uma max-heap, cada pai é maior ou igual aos filhos. Inserir corrige o caminho da folha à raiz; remover o máximo corrige o caminho da raiz à folha. Esses caminhos têm altura `O(log n)`. Há um laço para as inserções, outro para as remoções e laços internos nas operações da heap.

**Entrada:** primeiro um inteiro `n` entre 0 e 8000; depois `n` inteiros com sinal de 32 bits, separados por espaço ou linha. **Saída:** os mesmos `n` valores em ordem crescente, separados por espaços, com uma quebra de linha. O teste de exemplo usa `7` e `4 -2 7 4 0 -8 3`; a saída esperada é `-8 -2 0 3 4 4 7`. A função de ordenação recebe e devolve o vetor na mesma área; somente sua heap de trabalho é separada.

## Executar e verificar

Requer compilador C11, `make` e Python 3 para os testes. Na pasta do projeto:

```sh
make
./bin/heapsort < dados/exemplo.in
make test
```

`make test` executa verificações da propriedade da max-heap após inserções e remoções, testa heap vazia e cheia, números extremos, duplicatas e o tamanho máximo. Depois compara 26 casos de entrada do programa, inclusive vetores de 8.000 elementos em ordem crescente, decrescente, iguais e aleatórios com semente fixa, com `sorted()` do Python, usado como implementação de referência independente. Também verifica entradas inválidas. O resultado esperado é o mesmo vetor ordenado, sem perda ou duplicação de valores, e todos os testes marcados `OK`. Nesta primeira versão, todos os testes passaram no host; os 26 casos também passaram com os verificadores de acesso à memória e comportamento indefinido habilitados (detecção de vazamentos desabilitada porque não funciona neste ambiente de execução).

Na demonstração, execute o exemplo manual, `make test` e mostre um trecho do caso de 8.000 elementos. Explique por que a raiz contém o maior valor, como cada remoção restaura a propriedade da heap e por que a escrita acontece do fim para o começo.

## Memória e decisão para a placa

`int32_t` ocupa **4 bytes**: `dados[8000]` da heap ocupa 32.000 bytes; o vetor estático do `main` ocupa mais 32.000 bytes. No host de 64 bits, `sizeof(Heap)` costuma ser 32.008 bytes devido ao campo `size_t` (o teste imprime o valor real). As duas áreas de dados somam cerca de **64.000 bytes**, fora a pilha e a biblioteca C. Portanto, **8.000 inteiros de 32 bits não equivalem a 8 KB**; essa discrepância no enunciado deve ser discutida com o professor antes de adaptar para a placa.

A heap de trabalho e o vetor do host são `static`, então não são vetores grandes na pilha. Isso evita estouro de pilha, mas **não reduz a RAM total**. A variante clássica in-place poderia usar um único vetor e memória auxiliar `O(1)`; esta versão segue a proposta da dupla, com operações da heap separadas e memória auxiliar `O(n)`. A escolha não garante pontuação máxima no critério de dificuldade: é melhor demonstrar bem as operações, a análise e os testes do que reivindicar complexidade que o algoritmo não tem. Para embarcar, será necessário confirmar modelo da placa, RAM disponível e formato de entrada/saída; se a RAM não comportar 64 KB de dados, será preciso rever com o professor a representação ou a interface.

`src/heapsort.c` contém uma única heap estática: chamadas sequenciais funcionam, mas a função não é reentrante, o que importa se a placa usar interrupções ou tarefas concorrentes. O `main` depende de entrada/saída padrão do host; na placa, ele será substituído, mantendo o núcleo.

## Fontes e referência

- [MIT OpenCourseWare — Lecture 4: Heaps and Heap Sort](https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-fall-2011/resources/lecture-4-heaps-and-heap-sort/): descrição da fila de prioridade, operações de heap e ordenação.
- [MIT OpenCourseWare — Lecture 8: Binary Heaps (PDF)](https://ocw-preview.odl.mit.edu/courses/6-006-introduction-to-algorithms-spring-2020/40d4851e550507ca14dc778b9b2266cc_MIT6_006S20_lec8.pdf): construção por inserções, remoção do máximo e comparação com a versão in-place.
- [Documentação oficial do Python — `heapq`](https://docs.python.org/3/library/heapq.html): propriedade de min-heap/max-heap e operações de inserção e remoção.
- [Documentação oficial do Python — `sorted()`](https://docs.python.org/3/library/functions.html#sorted): função independente usada como resultado de referência nos testes.

## Pontos para a apresentação

1. **Funcionamento (20%)**: acompanhar à mão o exemplo e provar que o maior sempre sai da raiz.
2. **Dificuldade (30%)**: defender os ajustes de heap na subida e descida, índices dos filhos, capacidade fixa e caso de erro; reconhecer que ordenação foi aceita excepcionalmente.
3. **Testes (20%)**: mostrar os resultados da comparação com `sorted()`, a propriedade da heap durante a execução e os limites de 0 e 8.000 itens.
4. **Reuso (10%)**: distinguir o núcleo `heap`/`heapsort` da leitura e saída do host e explicar o limite de uma chamada por vez.
5. **Segurança na apresentação (20%)**: saber responder sobre `O(n log n)`, `O(n)` de memória auxiliar, diferença para o in-place e RAM real da placa.
