#include "node.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef enum {
	LEFT,
	RIGHT,
	FULL
} State;

Node nodes[MAX_NODES];
int node_top = 0;

// NOTE: Also technically push
Node* create_node(Type type){
	//printf("#%d - TYPE: %d\n", node_top, type);

	if (node_top > MAX_NODES - 1){
		printf("NOTE: Error their is an overflow\n");
		assert(node_top > MAX_NODES - 1);
	}

	Node* node = &nodes[node_top++];
	node->type = type;

	node->value = '\0';
	node->parent = node->left = node->right = NULL;

	return node;
}

void reset_tree(){
	node_top = 0;

	//for (int i = 0; i < MAX_NODES; i++)
	//	nodes[i] = (Node){.left=NULL, .right=NULL, .parent=NULL, .value='\0', .type=CLOSE};
}

// Checks if the trees are identical
// TODO: This need to work with "equivalent" trees
// In the sense that the order is different
//
// That or I need to make a function that check if theirs an adjacent OR or AND that has that value
bool compare_trees(Node* a, Node* b){
	printf("compare_trees\n");
	//if (a == NULL && b == NULL)
	//	return true;
	if (a->type == b->type){

		if (a->type == VAR)
			return a->value == b->value;

		bool left = false;
		bool right = false;
		if (	
			a->left != NULL && b->left != NULL 
			&& a->left->type == b->left->type
			&& a->left->value == b->left->value
		   ){
			left = compare_trees(a->left, b->left);
		}
		
		if (	
			a->right != NULL && b->right != NULL 
			&& a->right->type == b->right->type
			&& a->right->value == b->right->value
		   ){
			right = compare_trees(a->right, b->right);
		}
		printf("Comparison: left: %d and right: %d\n", left, right);
		return left && right;
	}

	printf("NO Comparison\n");
	return false;
}

/*
 * This function should be giving all the accessible nodes from a root node.
 * The assumption is that the type we look to the other side of teh operator or a go up.
 */
void find_accessible(Node** nodes, int* index, Node* from, Type type){
	printf("find_accessible\n");
	// left, right, neg left, neg right

	if (from == NULL)
		return;

	printf("from->type: %d, type: %d\n", from->type, type);

	if (from->type == NOT){
		assert(type == AND || type == OR);
		if (type == AND)
			type = OR;
		else
			type = AND;
		find_accessible(nodes, index, from->left, type);
		return;
	}

	if (from->type != type)
		return;

	printf("#%d - %p:%d: '%c'\n", *index, from, from->type, from->value);
	nodes[(*index)++] = from;
	printf("#%d - %p:%d: '%c'\n", *index, from, from->type, from->value);

	find_accessible(nodes, index, from->left, type);
	find_accessible(nodes, index, from->right, type);

	if (from->parent->left == from)
		find_accessible(nodes, index, from->parent->right, type);
	else
		find_accessible(nodes, index, from->parent->left, type);
}

int accessible(Node** nodes, Node* from){
	int index = 0;
	printf("from: %d\n", from->type);

	nodes[index++] = from->parent;
	
	find_accessible(nodes, &index, from, from->parent->type);

	if (from->parent->left == from)
		find_accessible(nodes, &index, from->parent->right, from->parent->type);
	else
		find_accessible(nodes, &index, from->parent->left, from->parent->type);

	return index;
}


char operator_chars[] = {'&', '|', '>', '~'};

void create_tree_to_string(Node* root, char* buffer, int buffer_size) {
	char left_buffer[buffer_size];
	char right_buffer[buffer_size];

	switch(root->type){
		case VAR:
			buffer[0] = root->value;
			buffer[1] = '\0';
			break;
		case AND:
		case OR:
		case THEN:
		case BTHEN:
			create_tree_to_string(root->left, &left_buffer, buffer_size);
			create_tree_to_string(root->right, &right_buffer, buffer_size);

			sprintf(buffer, "( %s %c %s )", left_buffer, operator_chars[root->type - 2], right_buffer);
			break;
		case NOT:
			create_tree_to_string(root->left, &left_buffer, buffer_size);
			sprintf(buffer, "! %s", left_buffer);
			break;
		case CLOSE:
			buffer[0] = 'F';
			buffer[1] = '\0';
			break;
		case OPEN:
			buffer[0] = 'T';
			buffer[1] = '\0';
			break;
		default:
			printf("DEFAULT\n");
			assert(false);
			break;
	}

	return;
}

void tree_to_string(Node* root, char* buffer, int buffer_size) {
	// if this is the root
	assert(root->parent == NULL);
	assert(root->type == OPEN);

	create_tree_to_string(root->left, buffer, buffer_size);
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

		token = strtok(NULL, " ");
	}
	return root;
}
