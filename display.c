#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "display.h"
#include "node.h"
#include "solve.h"


void display_treeprint(Node* root, int level){
	if (root == NULL)
		return;

	for (int i = 0; i < level; i++)
		printf(i==level-1 ? "|>": " ");

	char* types[] = {"VAR\0", "NOT\0", "AND\0", "OR\0", "THEN\0", "BTHEN\0", "TRUE\0", "FALSE\0"};

	if (root->value != '\0')
		printf("%d(%c)\n", root->type, root->value);
	else
		printf("%d(%s)\n", root->type, types[root->type]);
	display_treeprint(root->left, level+1);
	display_treeprint(root->right, level+1);
}

void treeprint(Node* root){
	display_treeprint(root, 0);
}

#define COLOR_RED "\33[41m"
#define COLOR_GREEN "\33[42m"
#define BOLD "\33[1m"

#define COLOR_END "\33[0m"

void test(char input_string[], char expected_output[]){
	printf("\n---- START OF TEST ----\n\ninput: '%s'\n\n", input_string);

	char input[MAX_LENGHT_STRING];
	strcpy(input, input_string);

	Node* root = create_tree(input_string);

	printf("-- START OF %sINITIAL%s TREEPRINT --\n", BOLD, COLOR_END);
	treeprint(root);
	printf("-- END OF %sINITIAL%s TREEPRINT --\n", BOLD, COLOR_END);


	printf("\n--- START OF OPTIMIZATIONS ---\n");

	bool success = reduce_tree(root);

	if (success)
		printf("The tree has SUCCESSFULLY been reduced.\n");
	else
		printf("The tree has FAILLED to be reduced.\n");

	printf("-- START OF %sOPTIMIZED%s TREEPRINT --\n", BOLD, COLOR_END);
	treeprint(root);
	printf("-- END OF %sOPTIMIZED%s TREEPRINT --\n", BOLD, COLOR_END);

	printf("--- END OF OPTIMIZATIONS ---\n");

	printf("--- START OF EVALUATING EXPECTED OUTPUT ---\n\n");
	char* output = tree_to_string(root);

	printf("output string: %s\n", output);
	printf("expected output: %s\n", expected_output);


	if (strcmp(output, expected_output) == 0)
		printf("%sSUCCESS: `%s` has been optimised to `%s`.%s\n", COLOR_GREEN, input, expected_output, COLOR_END);
	else
		printf("%sERROR: Expected `%s`, but got `%s`.%s\n", COLOR_RED, expected_output, output, COLOR_END);


	printf("-- END OF TEST --\n");
}
