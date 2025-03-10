#ifndef PARSERFILE_H
#define PARSERFILE_H

#include "node.h"

#define MAX_NESTING 10

/*
 * @description: tokenizes the string and builds a tree
 * @ret: returns the root of the tree
 */
Node* create_tree(char* input_string);

#endif
