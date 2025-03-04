#include <stdio.h>
#include <assert.h>

#include "node.h"
#include "solve.h"

void treeprint(Node* root, int level){
	if (root == NULL)
		return;

	for (int i = 0; i < level; i++)
		printf(i==level-1 ? "|>": " ");

	char* types[] = {"VAR\0", "NOT\0", "AND\0", "OR\0", "THEN\0", "BTHEN\0", "TRUE\0", "FALSE\0"};

	if (root->value != '\0')
		printf("%d(%c)\n", root->type, root->value);
	else
		printf("%d(%s)\n", root->type, types[root->type]);
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
	

	printf("\n--- START OF OPTIMIZATIONS ---\n");

	bool success = reduce_tree(root);

	if (success)
		printf("The tree has SUCCESSFULLY been reduced.\n");
	else
		printf("The tree has FAILLED to be reduced.\n");

	printf("-- START OF TREEPRINT --\n");
	treeprint(root, 0);
	printf("-- END OF TREEPRINT --\n");

	printf("--- END OF OPTIMIZATIONS ---\n");

	printf("---- END OF MAIN ----\n");


	return 0;
}
