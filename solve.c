#include "solve.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>


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

void replace_parent_with_child(Node* parent, Node* child){
	// the children of the parent are not preserved

	child->parent = parent->parent;

	if (parent->parent->left == parent)
		parent->parent->left = child;
	else
		parent->parent->right = child;

	// CHANGE: SOFT DELETE
	
	if (parent->left == child){
		parent->right->left = NULL;
		parent->right->right = NULL;
	}else{
		parent->left->left = NULL;
		parent->left->right = NULL;
	}
	
	parent->left = NULL;
	parent->right = NULL;
}

/*
 * Simplifies expression were left and right has the same value (also works with negative cases).
 */
bool simple_reduce(Node* root){
	bool left_not = left->type == NOT;
	bool right_not = right->type == NOT;

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

			// The root and right should be deleted, and replaced with left
			break;
		case THEN:
		case BTHEN:
			// POSITIVE
			// p -> p = !p|p = T
			// p <-> p = (p -> p) & (p -> p) = (!p|p) & (!p|p)= T & T = T

			// NEGATIVE
			// !p -> !p = p|!p = T
			// !p <-> !p = (!p -> !p) & (!p -> !p) = (p|!p) & (p|!p)= T & T = T

			// The left and right should be deleted, and the root should be replaced with a TAUTOLOGY
			break;
		case CLOSE:
		case VAR:
		case NOT:
		case OPEN:
		default:
			printf("ERROR Case this state should never happen\n");
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
			break;
		case OR:
			// POSITIVE
			// p | !p = T

			// NEGATIVE
			// !p | p = T

			// The left and right should be deleted, and the root should be replaced with a TAUTOLOGY
			break;
		case THEN:
			// POSITIVE
			// p -> !p = !p|!p = !p

			// NEGATIVE
			// !p -> p = p|p = p

			// The root and right should be deleted, and replaced with RIGHT
			break;
		case BTHEN:
			// POSITIVE
			// p <-> !p = (p -> !p) & (!p -> p) = (!p|!p) & (p|p) = !p & p = F

			// NEGATIVE
			// !p <-> p = (!p -> p) & (p -> !p) = (p|p) & (!p|!p)= p & !p = F

			// The left and right should be deleted, and the root should be replaced with a CONTRADICTION
			break;
		case CLOSE:
		case VAR:
		case NOT:
		case OPEN:
		default:
			printf("ERROR Case this state should never happen\n");
			assert(false);
			break;
		}
	}
}


// TODO: Use compare_trees instead of type VAR and value checking
bool special_reduce(Node* root, Type nested_type){
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


Node* reduce_tree(Node* root){
	printf("REDUCE TREE: %p, type: %d, left: %p, right: %p\n", root, root->type, root->left, root->right);
	if (root == NULL || (root->left == NULL && root->right == NULL)){
		return NULL;
	}

	// paterns
	
	switch(root->type){
		case AND:
			if (special_reduce(root, OR))
				break;
			break;
		case OR:
			if (special_reduce(root, AND))
				break;
			break;
		case THEN:
		case BTHEN:
			printf("THEN\n");
			return NULL;
			break;
		case CLOSE:
		case VAR:
			printf("hello\n");
			return NULL;
		case NOT:
		case OPEN:
		default:
			printf("DEFAULT\n");
			reduce_tree(root->left);
			break;
	}
	
	return root;
}
