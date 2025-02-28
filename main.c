#include <stdio.h>

#include "node.h"

void treeprint(Node* root, int level){
	if (root == NULL)
		return;

	for (int i = 0; i < level; i++)
		printf(i==level-1 ? "|>": " ");

	printf("%d(%c)\n", root->type, root->value);
	treeprint(root->left, level+1);
	treeprint(root->right, level+1);
}

int main(int argc, char* argv[]) {
	if (argc == 0 || argc > 2){
		printf("You must pass 1 argument to the program\n");
	}

	Node* root = create_tree(argv[1]);

	/*
	Node* root = create_node(SCOPE);
	root->left = create_node(VAR);
	root->right = create_node(VAR);
	*/

	treeprint(root, 0);

	printf("No errors\n");


	return 0;
}
