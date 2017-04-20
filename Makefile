all: index

index: index.c index.h
	gcc -Wall -g -o indexer index.c
clean:
	rm indexer
