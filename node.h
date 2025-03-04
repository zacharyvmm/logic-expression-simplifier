#ifndef NODEFILE_H
#define NODEFILE_H

#include <stdbool.h>

typedef enum {
	VAR,
	NOT,
	AND,
	OR,
	THEN,
	BTHEN,
	OPEN,
	CLOSE
} Type;


typedef struct Node {
	struct Node* parent;
	struct Node* left;
	struct Node* right;

	char value;
	Type type;
} Node;

#define MAX_NODES 100
#define MAX_NESTING 10

/*
 * @description: tokenizes the string and builds a tree
 * @ret: returns the root of the tree */

Node* create_tree(char* input_string);
Node* create_node(Type type);
bool compare_trees(Node* a, Node* b);

#endif
