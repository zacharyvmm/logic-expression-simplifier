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
 * @ret: returns the root of the tree
 */
Node* create_node(Type type);
void reset_tree();
void add_negation(Node* node);
bool collapse_negation(Node* node);
void replace_parent_with_child(Node* parent, Node* child);
void unpack_bthen(Node* root);
void unpack_then(Node* root);
void tautology(Node* node);
void contradiction(Node* node);

bool compare_trees(Node* a, Node* b);

/* 
 * @description: Find all accessible Operators for a child node.
 * @ret: returns the size of the operator nodes.
 */
int accessible(Node* nodes[], Node* from);

void tree_to_string(Node* root, char* buffer, int buffer_size);

#endif
