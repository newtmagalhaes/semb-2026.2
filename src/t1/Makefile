CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Wconversion -Werror
CPPFLAGS = -Iinclude

.PHONY: all test clean

all: bin/heapsort

bin:
	mkdir -p bin

bin/heapsort: main.c heapsort/heap.c heapsort/heapsort.c include/heap.h include/heapsort.h | bin
	$(CC) $(CPPFLAGS) $(CFLAGS) ./main.c ../heapsort/heap.c ../heapsort/heapsort.c -o $@

bin/teste_heap: tests/teste_heap.c heapsort/heap.c heapsort/heapsort.c include/heap.h include/heapsort.h | bin
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/teste_heap.c heapsort/heap.c heapsort/heapsort.c -o $@

test: bin/heapsort bin/teste_heap
	./bin/teste_heap
	python3 -m unittest -v tests/heapsort_test.py

clean:
	rm -rf bin
