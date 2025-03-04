#ifndef SOLVEFILE_H
#define SOLVEFILE_H

#include "node.h"
#include <stdbool.h>

/*
 * @description: reduce the size of the TREE
 * @ret: returns if the tree was successfully reduced */

bool reduce_tree(Node* root);

#endif
