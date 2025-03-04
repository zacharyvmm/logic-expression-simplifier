#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	LEFT,
	RIGHT,
	FULL
} State;

Node nodes[MAX_NODES];
int node_top = 0;

// NOTE: Also technically push
Node* create_node(Type type){
	printf("#%d - TYPE: %d\n", node_top, type);

	if (node_top > MAX_NODES - 1){
		printf("NODE: Error their is an overflow\n");
		return NULL;
	}

	Node* node = &nodes[node_top++];
	node->type = type;

	node->value = '\0';
	node->left = node->right = NULL;

	return node;
}

// Checks if the trees are identical
// TODO: This need to work with "equivalent" trees
// In the sense that the order is different
//
// That or I need to make a function that check if theirs an adjacent OR or AND that has that value
bool compare_trees(Node* a, Node* b){
	if (a == NULL && b == NULL)
		return true;

	if (a->type == b->type
			&& a->left->type == b->left->type
			&& a->right->type == b->right->type
			&& a->left->value == b->left->value
			&& a->right->value == b->right->value
			){
		bool left = compare_nodes(a->left, b->left);
		bool right = compare_nodes(a->right, b->right);
		return left && right;
	}

	return false;
}

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
			printf("Went LEFT\n");
			parent->left = node;
			node->parent = parent;

			printf("Set on left\n");
			if (type == OPEN)
				*state = LEFT;
			else if (parent->parent == NULL || 
					(parent->parent->type == OPEN && node->type != NOT)
				)
				// parent is the ROOT
				// If this is the first node to be set
				*state = FULL;
			else
				*state = RIGHT;


			break;
		case RIGHT:
			printf("Went RIGHT\n");
			parent->right = node;
			node->parent = parent;
			if (type == OPEN)
				*state = LEFT;
			else
				*state = FULL;

			break;
		case FULL:
			printf("Went UP\n");
			// IF NOT an Operand
			if (type == NOT || type == VAR) {
				printf("Their needs to be an operand before this TOKEN\n");
				break;
			}

			Node* grandparent = parent->parent;

			while (grandparent != NULL && grandparent->right != NULL
					&& grandparent->type != OPEN){
				grandparent = grandparent->parent;
			}

			// TODO: Remove after testing
			if (grandparent == NULL){
				printf("I have made a grave error\n");

				exit(1);
			}

			//if (grandparent->parent == NULL){
			if (grandparent->type == OPEN){
				//printf("We are at root\n");
				printf("We are at the closest OPEN\n");

				// Insert node of left and attach the current left

				node->left = grandparent->left;
				node->left->parent = node;
				grandparent->left = node;
				
				// New node only right empty
				*state = RIGHT;

			} else {
				grandparent->right = node;

				// New node left and right empty
				*state = LEFT;
			}
			node->parent = grandparent;



			//while (grandparent->left != NULL && grandparent->right != NULL)
			
			// create a node above the parent
			break;
	}

	if (type == OPEN){
		scope_stack_push(node);
	}


	printf("END is not '%d'\n", *state);

	return node;
}


Node* create_tree(char* input_string){
	printf("Start\n");
	printf("%s\n", input_string);
	char* token = strtok(input_string, " ");
	Node* root = create_node(OPEN);
	printf("Created root\n");
	//root->left = create_node(NULL); // first Operand

	//Node* current = root->left;
	Node* current = root;
	State state = LEFT;

	while (token != NULL){
		switch (token[0]){
			// NOT
			case '-':
				current = add_to_tree(current, &state, NOT);
				break;
			// AND
			case '^':
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

		token = strtok(NULL, " ");
	}
	return root;
}
