#include "solve.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>


Node* copy_tree(Node* root){
	if (root == NULL)
		return NULL;

	Node* new_root = create_node(root->type);
	new_root->left = copy_tree(root->left);
	new_root->right = copy_tree(root->right);
	return new_root;
}

void unpack_then(Node* root){
	assert(root->type == THEN);
	// change type to OR	
	root->type = NOT;

	// Add NOT above left
	
	assert(root->left != NULL);

	Node* left_not = create_node(NOT);
	
	root->left->parent = left_not;

	left_not->left = root->left;
	root->left = left_not;
}

void unpack_bthen(Node* root){
	assert(root->type == BTHEN);


	Node* new_node = copy_tree(root);
	Node* left = new_node->left;

	new_node->left = new_node->right;
	new_node->right = left;
	new_node->type = THEN;

	root->type = AND;


	// NOTE: left is also used to create the THEN of the left side
	left = create_node(THEN);
	root->left->parent = left;
	root->right->parent = left;

	left->left = root->left;
	left->right = root->right;

	root->left = left;
	root->right = new_node;

	unpack_then(root->left);
	unpack_then(root->right);
}

void delete_tree(Node* root){
	assert(root != NULL);

	if (root->left != NULL)
		delete_tree(root->left);

	if (root->right != NULL)
		delete_tree(root->right);

	// CHANGE: SOFT DELETE
	root->left = NULL;
	root->right = NULL;
	root->type = CLOSE;
	root = NULL;

	return;
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

/*
 * Simplifies expression were left and right has the same value (also works with negative cases).
 */
bool same_value_reduce(Node* root){
	printf("same_value_reduce\n");
	bool left_not = root->left->type == NOT;
	bool right_not = root->right->type == NOT;

	if (
			(!right_not && !left_not && compare_trees(root->left, root->right))
			 || (right_not && left_not && compare_trees(root->left->left, root->right->left))
	) {
		// left: POSITIVE and right: POSITIVE
		// left: NEGATIVE and right: NEGATIVE
		switch (root->type){
		case AND:
		case OR:
			// POSITIVE
			// p & p = p
			// p | p = p

			// NEGATIVE
			// !p & !p = !p
			// !p | !p = !p

			printf("SAME value: op: %d, left: %c, right: %c\n", root->type, root->left->value, root->right->value);
			// The root and right should be deleted, and replaced with LEFT OR RIGHT
			replace_parent_with_child(root, root->right);
			return true;
		case THEN:
		case BTHEN:
			// POSITIVE
			// p -> p = !p|p = T
			// p <-> p = (p -> p) & (p -> p) = (!p|p) & (!p|p)= T & T = T

			// NEGATIVE
			// !p -> !p = p|!p = T
			// !p <-> !p = (!p -> !p) & (!p -> !p) = (p|!p) & (p|!p)= T & T = T

			// The left and right should be deleted, and the root should be replaced with a TAUTOLOGY
			delete_tree(root->left);
			delete_tree(root->right);

			root->type = OPEN;
			return true;
		case CLOSE:
		case VAR:
		case NOT:
		case OPEN:
		default:
			printf("ERROR: this state should never happen\n");
			assert(false);
			break;
		}
	} else if (
			(right_not && compare_trees(root->left, root->right->left))
			|| (left_not && compare_trees(root->left->left, root->right))
		) {
		// left: POSITIVE and right: NEGATIVE
		// left: NEGATIVE and right: POSITIVE
		switch (root->type){
		case AND:
			// POSITIVE
			// p & !p = F

			// NEGATIVE
			// !p & p = F

			// The left and right should be deleted, and the root should be replaced with a CONTRADICTION

			delete_tree(root->left);
			delete_tree(root->right);

			root->type = CLOSE;
			return true;
		case OR:
			// POSITIVE
			// p | !p = T

			// NEGATIVE
			// !p | p = T

			// The left and right should be deleted, and the root should be replaced with a TAUTOLOGY

			delete_tree(root->left);
			delete_tree(root->right);

			root->type = OPEN;
			return true;
		case THEN:
			// POSITIVE
			// p -> !p = !p|!p = !p

			// NEGATIVE
			// !p -> p = p|p = p

			// The root and right should be deleted, and replaced with RIGHT

			replace_parent_with_child(root, root->right);
			return true;
		case BTHEN:
			// POSITIVE
			// p <-> !p = (p -> !p) & (!p -> p) = (!p|!p) & (p|p) = !p & p = F

			// NEGATIVE
			// !p <-> p = (!p -> p) & (p -> !p) = (p|p) & (!p|!p)= p & !p = F

			// The left and right should be deleted, and the root should be replaced with a CONTRADICTION

			delete_tree(root->left);
			delete_tree(root->right);

			root->type = CLOSE;
			return true;
		case CLOSE:
		case VAR:
		case NOT:
		case OPEN:
		default:
			printf("ERROR: this state should never happen\n");
			assert(false);
			break;
		}
	}

	return false;
}


// TODO: Use compare_trees instead of type VAR and value checking
// TODO: Does not handle NOT on LEFT or RIGHT
bool nested_value_reduce(Node* root, Type nested_type){
	printf(">>nested_value_reduce\n");
	// p & (p | q) = p | (p & q) = p

	Node* left = root->left;
	Node* right = root->right;

	// Can just check the value of the sides
	if (left->type == nested_type && right->type == VAR 
			&& (left->left->value == right->value || left->right->value == right->value)){
			// replace root with right

			printf("First case\n");

			replace_parent_with_child(root, right);
			return true;
	}else if(right->type == nested_type && left->type == VAR
			&& (right->left->value == left->value || right->right->value == left->value)){
			// replace root with left

			printf("Second case\n");

			replace_parent_with_child(root, left);
			return true;
	}
			
	printf("Nothing happended case\n");

	return false;
}

// Basiclly => when the children are reduced try again
typedef bool (*Reduce_callback)(Node*, bool);

bool try_reduce_tail(Node* root, Reduce_callback callback){
	bool success;

	if (root->left == NULL && root->right == NULL)
		return true;

	if (root->left->type != VAR){
		success = reduce_tree(root->left);

		// TODO: It should also return true if the reduce changed nothing
		if (!success || (root->left == NULL && root->right == NULL))
			return true;
		return callback(root, true);
	}

	if (root->right->type != VAR){
		success = reduce_tree(root->right);

		// TODO: It should also return true if the reduce changed nothing
		if (!success || (root->left == NULL && root->right == NULL))
			return true;
		return callback(root, true);
	}

	return false;
}

bool try_and_reduce(Node* root, bool callbacked){
	printf("try_and_reduce\n");
	if (same_value_reduce(root))
		return true;

	if (nested_value_reduce(root, OR))
		return true;

	if (callbacked)
		return false;

	return try_reduce_tail(root, &try_and_reduce);
}

bool try_or_reduce(Node* root, bool callbacked){
	printf("try_or_reduce\n");
	if (same_value_reduce(root))
		return true;

	if (nested_value_reduce(root, AND))
		return true;

	if (callbacked)
		return false;

	return try_reduce_tail(root, &try_or_reduce);
}


bool reduce_tree(Node* root){
	printf("REDUCE TREE: %p, type: %d, left: %p, right: %p\n", root, root->type, root->left, root->right);
	if (root == NULL || (root->left == NULL && root->right == NULL)){
		return NULL;
	}

	// paterns
	#define NOT_CALLBACKED false
	
	switch(root->type){
		case AND:
			printf("AND\n");
			return try_and_reduce(root, NOT_CALLBACKED);
		case OR:
			printf("OR\n");
			return try_or_reduce(root, NOT_CALLBACKED);
		case THEN:
		case BTHEN:
			printf("THEN\n");
			return false;
		case CLOSE:
		case VAR:
			printf("hello\n");
			// return false;
			break;
		case NOT:
		case OPEN:
		default:
			printf("DEFAULT\n");
			return reduce_tree(root->left);
			break;
	}
	
	return false;
}
