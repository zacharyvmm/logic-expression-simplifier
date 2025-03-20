#include "parser.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef enum {
	LEFT,
	RIGHT,
	FULL
} State;


// The pointers to the last open parenthasis
// So that I can delete it later
Node* scope[MAX_NESTING];
int scope_top = 0;

void scope_stack_push(Node* node){
	if (scope_top > MAX_NESTING - 1){
		printf("SCOPE: Error their is an overflow\n");
		return;
	}
	scope[scope_top++] = node;
}

Node* scope_stack_pop(){
	return scope[--scope_top];
}


Node* add_to_tree(Node* parent, State* state, Type type){
	//printf("add_to_tree\n");
	Node* node;

	if (type == CLOSE){
		// DELETE THE SCOPE NODE

		node = scope_stack_pop();

		if (node->parent->left == node)
			node->parent->left = node->left; 
		else
			node->parent->right = node->left; 

		node->left->parent = node->parent; 
		node->type = CLOSE;
		return node->left;
	}

	
	node = create_node(type);

	switch(*state){
		case LEFT:
			//printf("Went LEFT\n");
			assert(parent->type != VAR);

			parent->left = node;
			node->parent = parent;

			//printf("Set on left\n");
			if (type == OPEN || type == NOT)
				*state = LEFT;
			else if (parent->type == OPEN || parent->type == NOT)
				// parent is the ROOT
				// If this is the first node to be set
				*state = FULL;
			else
				*state = RIGHT;


			break;
		case RIGHT:
			//printf("Went RIGHT\n");
			assert(parent->type != VAR);
			assert(parent->type != NOT);
			assert(parent->type != OPEN);

			parent->right = node;
			node->parent = parent;
			if (type == OPEN || type == NOT)
				*state = LEFT;
			else
				*state = FULL;

			break;
		case FULL:
			//printf("Went UP\n");
			// IF NOT an Operand
			if (type == NOT || type == VAR) {
				printf("Their needs to be an operand before this TOKEN\n");
				break;
			}

			Node* grandparent = parent->parent;
			//printf("%p\n", grandparent);

			//printf("grandparent type: %d\n", grandparent->type);

			while (grandparent != NULL && (grandparent->right != NULL
					|| grandparent->type != OPEN)){
				grandparent = grandparent->parent;
			}

			assert(grandparent->type != VAR);
			assert(grandparent->type != NOT);


			// TODO: Remove after testing
			// I have made a grave error
			assert(grandparent != NULL);

			//if (grandparent->parent == NULL){
			if (grandparent->type == OPEN){
				//printf("We are at the closest OPEN\n");

				// Insert node of left and attach the current left

				node->left = grandparent->left;
				node->left->parent = node;
				grandparent->left = node;
				
				// New node only right empty
				*state = RIGHT;

			} else {
				grandparent->left = node;

				// New node left and right empty
				*state = RIGHT;
			}
			node->parent = grandparent;



			//while (grandparent->left != NULL && grandparent->right != NULL)
			
			// create a node above the parent
			break;
	}

	if (type == OPEN){
		scope_stack_push(node);
	}

	//printf("END state type (0:left, 1:right, 2:full): '%d'\n", *state);

	return node;
}


Node* create_tree(char* input_string){
	//printf("Start\n");
	//printf("%s\n", input_string);
	char* token = strtok(input_string, " ");

	Node* root = create_node(OPEN);
	//root->left = create_node(NULL); // first Operand

	//Node* current = root->left;
	Node* current = root;
	State state = LEFT;

	while (token != NULL){
		assert(token[0] != '\0');
		switch (token[0]){
			// NOT
			case '!':
				current = add_to_tree(current, &state, NOT);
				break;
			// AND
			case '&':
				current = add_to_tree(current, &state, AND);
				break;

			// OR
			case '|':
				current = add_to_tree(current, &state, OR);
				break;

			// THEN
			case '>':
				current = add_to_tree(current, &state, THEN);
				break;

			// BIDERECTIONAL THEN
			case '~':
				current = add_to_tree(current, &state, BTHEN);
				break;

			// OPEN parenthasis
			case '(':
				current = add_to_tree(current, &state, OPEN);
				break;

			// CLOSE parenthasis
			case ')':
				current = add_to_tree(current, &state, CLOSE);
				break;

			// VAR
			default:
				current = add_to_tree(current, &state, VAR);
				current->value = token[0];
				break;
		}

		if (token[1] == '\0')
			token = strtok(NULL, " ");
		else
			token++;
	}
	return root;
}
