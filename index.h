#ifndef INDEX_H
#define INDEX_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Node{
	//int occurrence;
	
	char * word;
	//char * fileName;
	//Node * nextFile;
	struct fileNode * filesHead;
	struct Node * nextWord;
}Node;
typedef struct fileNode{
	int occurrence;
	char * fileName;
	struct fileNode * nextFile;
}fileNode;

#endif