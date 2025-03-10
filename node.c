#include "node.h"

#include <stdio.h>
#include <assert.h>

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

Node* copy_tree(Node* root){
	if (root == NULL)
		return NULL;

	Node* new_root = create_node(root->type);
	new_root->value = root->value;
	new_root->parent = root->parent;

	new_root->left = copy_tree(root->left);
	new_root->right = copy_tree(root->right);
	return new_root;
}

void delete_tree(Node* root){
	assert(root != NULL);

	if (root->left != NULL)
		delete_tree(root->left);

	if (root->right != NULL)
		delete_tree(root->right);

	// CHANGE: SOFT DELETE
	root->parent = root->left = root->right = NULL;
	root->value = '\0';
	root->type = CLOSE;
	root = NULL;

	return;
}

void add_negation(Node* node){
	assert(node->parent != NULL);

	switch (node->type) {
	case OPEN:
		node->type = CLOSE;
		break;
	case CLOSE:
		node->type = OPEN;
		break;
	case NOT:
		node->left->parent = node->parent;
		if (node->parent->left == node)
			node->parent->right = node->left;
		else
			node->parent->left = node->right;

		// SOFT DELETE
		node->left = NULL;
		node->parent = NULL;
		node = NULL;
		break;
	case VAR:
	case AND:
	case OR:
	case THEN:
	case BTHEN:
		Node* not = create_node(NOT);
		not->parent = node->parent;
		not->left = node;

		if (node->parent->left == node)
			node->parent->left = not;
		else
			node->parent->right = not;

		node->parent = not;
		break;
	}
}

void tautology(Node* node){
	printf("type: %d\n", node->type);
	delete_tree(node->right);
	delete_tree(node->left);

	node->value = '\0';
	node->type = OPEN;
	node->left = NULL;
	node->right = NULL;
}

void contradiction(Node* node){
	printf("type: %d\n", node->type);
	delete_tree(node->right);
	delete_tree(node->left);

	node->value = '\0';
	node->type = CLOSE;
	node->left = NULL;
	node->right = NULL;
}

void unpack_then(Node* root){
	assert(root->type == THEN);

	// change type to OR	
	root->type = OR;

	// Add NOT above left
	
	assert(root->left != NULL);

	add_negation(root->left);
	assert(root->left->type == NOT || root->left->type == OPEN || root->left->type == CLOSE);
}

void unpack_bthen(Node* root){
	assert(root->type == BTHEN);

	Node* new_node = copy_tree(root);
	Node* left = new_node->left;

	new_node->parent = root;
	new_node->left->parent = new_node;
	new_node->right->parent = new_node;

	new_node->left = new_node->right;
	new_node->right = left;
	new_node->type = THEN;

	root->type = AND;


	// NOTE: left is also used to create the THEN of the left side
	left = create_node(THEN);
	left->parent = root;
	root->left->parent = left;
	root->right->parent = left;

	left->left = root->left;
	left->right = root->right;

	root->left = left;
	root->right = new_node;

	//unpack_then(root->left);
	//unpack_then(root->right);
}

// the children of the parent are not preserved
void replace_parent_with_child(Node* parent, Node* child){
	child->parent = parent->parent;

	if (parent->parent->left == parent)
		parent->parent->left = child;
	else
		parent->parent->right = child;

	// CHANGE: SOFT DELETE
	
	if (parent->left == child){
		delete_tree(parent->right);
	}else{
		delete_tree(parent->left);
	}
	
	parent->left = NULL;
	parent->right = NULL;
	parent = NULL;
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
	
	//find_accessible(nodes, &index, from, from->parent->type);

	if (from->parent->left == from)
		find_accessible(nodes, &index, from->parent->right, from->parent->type);
	else
		find_accessible(nodes, &index, from->parent->left, from->parent->type);

	return index;
}


/// PRINT OUT TREE ///

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
