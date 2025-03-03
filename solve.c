#include "solve.h"

#include <assert.h>
#include <bool.h>


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

typedef enum {
	VAR,
	NOT,
	AND,
	OR,
	THEN,
	BTHEN,
	OPEN,
	CLOSE
} Type;

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

bool simple_reduce(Node* root){
	// find p & p
	// find p | p
	// find p & !p
	// find p | !p
	//
	// find !p & p
	// find !p | p
	// find !p & !p
	// find !p | !p

	Node* left = root->left;
	Node* right = root->right;

	bool left_var = left->type == VAR;
	bool right_var = left->type == VAR;

	bool left_not_var = left->type == NOT && left->left->type == VAR;
	bool right_not_var = right->type == NOT && right->left->type == VAR;

	if ((right_var && left_var && right->value == left->value) 
			|| (right_not_var && left_not_var && right->left->value == left->left->value)){


		// (p & p) or (!p & !p)
		// (!p | !p) or (!p | !p)
		replace_parent_with_child(root, left);
		return true;
	} else if ((right_not_var && left_var && right->left->value == left->value)
			|| ){
		// (p & !p) or (p | !p)

		if (root->type == OR){
			left->value = NULL;

			// it's a TAUTOLOGY
			left->type = OPEN;
		} else if (root->type == AND){
			left->value = NULL;

			// it's a CONTRADICTION
			left->type = CLOSE;
		}

		// TODO: The right->left has to be deleted
		replace_parent_with_child(root, left);
		return true;
	} else if (left_not_var && right_var && right->value == left->left->value){
		// (!p & p) or (!p | p)

		if (root->type == OR){
			right->value = NULL;

			// it's a TAUTOLOGY
			// BUG: IF the right had children it could make a bug
			right->type = OPEN;
		} else if (root->type == AND){
			right->value = NULL;

			// it's a CONTRADICTION
			right->type = CLOSE;
		}

		// TODO: The left->left has to be deleted
		replace_parent_with_child(root, right);
		return true;
	}

	/*if (left->type == VAR){
		if (right->type == VAR && left->value == right->value){
			replace_parent_with_child(root, left);
		} else if (right->type == NOT && right->left->type == VAR && left->value == right->left->value){
			replace_parent_with_child(root, left);
			
			if (root->type == OR){
				left->value = NULL;

				// it's a TAUTOLOGY
				left->type = OPEN;
			} else if (root->type == AND){
				left->value = NULL;

				// it's a CONTRADICTION
				left->type = CLOSE;
			}
		}
	} else if (left->type == NOT && left->left->type == VAR){
		if (right->type == VAR && left->value == right->value){
			replace_parent_with_child(root, left);

			if (root->type == OR){
				left->value = NULL;

				// it's a TAUTOLOGY
				left->type = OPEN;
			} else if (root->type == AND){
				left->value = NULL;

				// it's a CONTRADICTION
				left->type = CLOSE;
			}
		} else if (right->type == NOT && right->left->type == VAR && left->value == right->left->value){
			replace_parent_with_child(root, left);
		}
	}*/

	return false;
}

bool special_reduce(Node* root, Type nested_type){
	// p & (p | q) = p | (p & q) = p

	Node* left = root->left;
	Node* right = root->right;

	// Can just check the value of the sides
	if (left->type == nested_type && right->type == VAR 
			&& (left->left->value == right->value || left->right->value == right->value)){
			// replace root with right

			replace_parent_with_child(root, right);
			return true;
	}else if(right->type == nested_type && left->type == VAR
			&& (right->left->value == left->value || right->right->value == left->value)){
			// replace root with left

			replace_parent_with_child(root, left);
			return true;
	}

	return false;
}


Node* reduce_tree(Node* root){
	if (root == NULL || (root->left == NULL && root->right == NULL))
		return NULL;

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
		case OPEN:
		case CLOSE:

		case VAR:
			return NULL;
		case NOT:
		default:
			reduce_tree(root->left);
			break;
	}
	
	if (root->type == OR 
			&& (root->right->type == OR && root->left->type == AND)
			&& (root->left->type == VAR && root)
	

	return NULL;
}
