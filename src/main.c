#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "node.h"
#include "parser.h"
#include "solve.h"
#include "display.h"


int main(int argc, char* argv[]) {
	if (argc != 2){
		printf("You have passed no or too many argument, thus the program will run all the tests.\n");

		// IF NOT ARGUMENTS ARE PASSED THEN WE RUN TESTS 

		char tests[NUM_TESTS][MAX_LENGHT_STRING] = {
			"p & q",
			"( p & ( q | r ) ) | p",
			"( p & q | r ) | p",
			"( ( p | q ) & p ) > p",
			"( ( p & q ) | p ) ~ ( ( p | q ) & p ) ~ p",
			"( p & p ) & ( p | p )",
			"! ! ! p",
			"! p",
			"! ! p",
			"! ! ! ! p",
			"( ( p > q ) & ( q > r ) ) > ( p > r )",
			"( p & ( p > q ) ) > q",
			"p > ( p > p )",
			"( p > ( q & r ) ) > ( ( p > q ) & ( p > r ) )",
			"p > ( q ~ p )",
			"p & ! ( p | ! q )",
			"( ( p | q ) > r ) > ( ( p > r ) & ( q > r ) )",
			"( ( p > r ) & ( q > r ) & ( p | q ) ) > r",
			"( ( ! p & q ) | p ) ~ ( ( p | q ) & ! p ) ~ p",
		};

		char expected_outputs[NUM_TESTS][MAX_LENGHT_STRING] = {
			"( p & q )",
			"p",
			"p",
			"T",
			"p",
			"p",
			"! p",
			"! p",
			"p",
			"p",
			"T",
			"T",
			"T",
			"T",
			"( p > q )",
			"F",
			"T",
			"r",
			"( ( q | p ) & ( ! ( p & q ) ) )",
		};

		for (int i = 0; i < NUM_TESTS; i++){
			test(tests[i], expected_outputs[i]);
		}

		return 0;
	}

	printf("---- START OF MAIN ----\n");

	Node* root = create_tree(argv[1]);

	printf("--- START OF TREEPRINT ---\n");
	treeprint(root);
	printf("--- END OF TREEPRINT ---\n");
	

	printf("\n--- START OF OPTIMIZATIONS ---\n");

	bool success = reduce_tree(root);

	if (success)
		printf("The tree has SUCCESSFULLY been reduced.\n");
	else
		printf("The tree has FAILLED to be reduced.\n");

	printf("-- START OF TREEPRINT --\n");
	treeprint(root);
	printf("-- END OF TREEPRINT --\n");

	printf("--- END OF OPTIMIZATIONS ---\n");

	printf("---- END OF MAIN ----\n");


	char output[150]; 
	tree_to_string(root, output, 150);

	printf("\n\n≡ %s\n", output);

	return 0;
}
