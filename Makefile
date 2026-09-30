CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Wconversion -Werror
CPPFLAGS = -Iinclude

.PHONY: all test clean

all: bin/heapsort

bin:
	mkdir -p bin

bin/heapsort: src/main.c src/heap.c src/heapsort.c include/heap.h include/heapsort.h | bin
	$(CC) $(CPPFLAGS) $(CFLAGS) src/main.c src/heap.c src/heapsort.c -o $@

bin/teste_heap: tests/teste_heap.c src/heap.c src/heapsort.c include/heap.h include/heapsort.h | bin
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/teste_heap.c src/heap.c src/heapsort.c -o $@

test: bin/heapsort bin/teste_heap
	./bin/teste_heap
	python3 -m unittest -v tests/heapsort_test.py
# 	python3 tests/teste_referencia.py ./bin/heapsort

clean:
	rm -rf bin
