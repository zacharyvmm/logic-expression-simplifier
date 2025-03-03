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

	printf("---- START OF MAIN ----\n");

	Node* root = create_tree(argv[1]);

	printf("--- START OF TREEPRINT ---\n");
	treeprint(root, 0);
	printf("--- END OF TREEPRINT ---\n");

	printf("---- END OF MAIN ----\n");


	return 0;
}
