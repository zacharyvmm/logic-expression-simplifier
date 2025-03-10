#include "solve.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>


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

void unpack_then(Node* root){
	assert(root->type == THEN);

	// change type to OR	
	root->type = OR;

	// Add NOT above left
	
	assert(root->left != NULL);

	Node* left_not = create_node(NOT);
	
	left_not->parent = root;
	left_not->left = root->left;
	root->left->parent = left_not;
	root->left = left_not;
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
 *
 * LAWS: IDEMPOTENT LAW and COMPLEMENTARY LAW
 */
bool same_value_reduce(Node* left, Node* right){
	printf("same_value_reduce\n");

	Node* parent = left->parent;
	bool left_not = left->type == NOT;
	bool right_not = right->type == NOT;

	if (
			(!right_not && !left_not && compare_trees(left, right))
			 || (right_not && left_not && compare_trees(left->left, right->left))
	) {
		// left: POSITIVE and right: POSITIVE
		// left: NEGATIVE and right: NEGATIVE
		switch (parent->type){
		case AND:
		case OR:
			// POSITIVE
			// p & p = p
			// p | p = p

			// NEGATIVE
			// !p & !p = !p
			// !p | !p = !p

			// The root and right should be deleted, and replaced with LEFT OR RIGHT
			if (left->parent == right->parent || right->parent->right == right)
				replace_parent_with_child(right->parent, right);
			else
				replace_parent_with_child(right->parent, right->parent->left);
			return true;
		case THEN:
		case BTHEN:
			assert(left->parent == right->parent);
			// POSITIVE
			// p -> p = !p|p = T
			// p <-> p = (p -> p) & (p -> p) = (!p|p) & (!p|p)= T & T = T

			// NEGATIVE
			// !p -> !p = p|!p = T
			// !p <-> !p = (!p -> !p) & (!p -> !p) = (p|!p) & (p|!p)= T & T = T

			// The left and right should be deleted, and the root should be replaced with a TAUTOLOGY

			delete_tree(left);
			delete_tree(right);
			
			parent->left = NULL;
			parent->right = NULL;
			parent->type = OPEN;
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
			(right_not && (compare_trees(left, right->left)))
			|| (left_not && compare_trees(left->left, right))
		) {
		// left: POSITIVE and right: NEGATIVE
		// left: NEGATIVE and right: POSITIVE
		switch (parent->type){
		case AND:
			// POSITIVE
			// p & !p = F

			// NEGATIVE
			// !p & p = F

			// The left and right should be deleted, and the root should be replaced with a CONTRADICTION

			delete_tree(left);
			delete_tree(right);

			// BUG: This does not work
			parent->left = NULL;
			parent->right = NULL;
			parent->type = CLOSE;
			return true;
		case OR:
			// POSITIVE
			// p | !p = T

			// NEGATIVE
			// !p | p = T

			// The left and right should be deleted, and the root should be replaced with a TAUTOLOGY

			delete_tree(left);
			delete_tree(right);

			parent->left = NULL;
			parent->right = NULL;
			parent->type = OPEN;
			return true;
		case THEN:
			assert(left->parent == right->parent);
			// POSITIVE
			// p -> !p = !p|!p = !p

			// NEGATIVE
			// !p -> p = p|p = p

			// The root and right should be deleted, and replaced with RIGHT

			if (left->parent == right->parent || right->parent->right == right)
				replace_parent_with_child(right->parent, right);
			else
				replace_parent_with_child(right->parent, right->parent->left);
			return true;
		case BTHEN:
			assert(left->parent == right->parent);
			// POSITIVE
			// p <-> !p = (p -> !p) & (!p -> p) = (!p|!p) & (p|p) = !p & p = F

			// NEGATIVE
			// !p <-> p = (!p -> p) & (p -> !p) = (p|p) & (!p|!p)= p & !p = F

			// The left and right should be deleted, and the root should be replaced with a CONTRADICTION

			delete_tree(left);
			delete_tree(right);


			parent->left = NULL;
			parent->right = NULL;
			parent->type = CLOSE;
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

bool bool_value_reduce(Node* left, Node* right){
	printf("bool_value_reduce\n");

	if(left->type != OPEN && left->type != CLOSE
			&& right->type != OPEN && right->type != CLOSE)
		return false;

	switch (left->parent->type){
	case AND:
		// AND
		// T & p = p
		// T & T = T
		// T & F = F

		// F & p = F
		// F & F = F
		// F & T = F
		
		if (left->type == CLOSE || right->type == CLOSE){
			// replace OPERATOR with CLOSE
		} else if (left->type == OPEN){
			// replace OPERATOR with RIGHT
		} else if (right->type == OPEN){
			// replace OPERATOR with LEFT
		}

	case OR:
		// OR
		// T | p = T
		// T | T = T
		// T | F = T

		// F | p = p
		// F | F = F
		// F | T = T

		if (left->type == OPEN || right->type == OPEN){
			// replace OPERATOR with OPEN
		} else if (left->type == CLOSE){
			// replace OPERATOR with RIGHT
		} else if (right->type == CLOSE){
			// replace OPERATOR with LEFT
		}
	case THEN:
		// THEN
		// T -> p = p
		// T -> T = T
		// T -> F = F

		// F -> p = T
		// F -> F = T
		// F -> T = T

		if (left->type == CLOSE || right->type == CLOSE){
			// replace OPERATOR with OPEN
		} else if (left->type == OPEN){
			// replace OPERATOR with RIGHT
		} else if (right->type == OPEN){
			// replace OPERATOR with LEFT
		}
	case BTHEN:
		// BIDERECTIONAL THEN
		// T <-> p = (T -> p) & (p -> T) = p & T = p
		// F <-> p = (F -> p) & (p -> F) = T & !p = !p
		// T <-> T = (T -> T) & (T -> T) = T
		// F <-> F = (F -> F) & (F -> F) = T & T = T
		// T <-> F = (T -> F) & (F -> T) = F & T = F
		if (left->type == CLOSE || right->type == CLOSE){
			// replace OPERATOR with OPEN
		} else if (left->type == OPEN){
			// replace OPERATOR with RIGHT
		} else if (right->type == OPEN){
			// replace OPERATOR with LEFT
		} else if (left->type == CLOSE){
			// replace OPERATOR with NOT RIGHT
		} else if (right->type == CLOSE){
			// replace OPERATOR with NOT LEFT
		}
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


// TODO: Use compare_trees instead of type VAR and value checking
// TODO: Does not handle NOT on LEFT or RIGHT
//
// **ABSORPTION LAW**
bool nested_value_reduce(Node* left, Node* right, Type nested_type){
	printf("  nested_value_reduce\n");
	// p & (p | q) = p | (p & q) = p

	// Can just check the value of the sides
	if (left->type == nested_type && left->left != NULL && left->right != NULL
			&& ( compare_trees(left->left, right) || compare_trees(left->right, right) )){
			// replace root with right

			printf("First case\n");
			printf(">> NESTED VALUE REDUCE <<\n");
			replace_parent_with_child(right->parent, right);
			return true;
	}else if(right->type == nested_type && right->left != NULL && right->right != NULL
			&& ( compare_trees(right->left, left) || compare_trees(right->right, left) )){
			// replace root with left

			printf("Second case\n");
			printf(">> NESTED VALUE REDUCE <<\n");

			replace_parent_with_child(left->parent, left);
			return true;
	}
			
	printf("Nothing happended case\n");

	return false;
}

bool reduce_then_bthen(Node* root){
	Node* parent = root->parent;


	bool reduced = false;

	// TRY TO OPTIMIZE RIGHT AND LEFT BEFORE UNPACKING
	if (parent->left != NULL && parent->left->type != VAR)
		reduced = reduced | reduce_tree(parent->left);

	if (parent->right != NULL && parent->right->type != VAR)
		reduced = reduced | reduce_tree(parent->right);
	// END OF PRE OPTIMIZATION


	///////////////////////// THEN /////////////////////////

	if (parent->type == THEN){
		printf("UNPACK THEN\n");
		unpack_then(parent);
		//nested_value_reduce(root, AND);
		return reduce_tree(parent);
	}

	///////////////////////// BTHEN /////////////////////////

	assert(root->parent->type == BTHEN);

	printf("UNPACK BTHEN\n");
	unpack_bthen(parent);

	//unpack_then(root->left);
	//nested_value_reduce(root->left, AND);
	if (parent->left != NULL && parent->left->type != VAR)
		reduced = reduced | reduce_tree(parent->left);


	//unpack_then(root->right);
	//nested_value_reduce(root->right, AND);
	

	if (parent->right != NULL && parent->right->type != VAR)
		reduced = reduced | reduce_tree(parent->right);

	//assert(parent->left == root || parent->right == root);
	//if (parent->left == root)
	//	reduced = reduced | nested_value_reduce(root, parent->right, OR);
	//else
	//	reduced = reduced | nested_value_reduce(root, parent->left, OR);

	return reduced;
}


bool reduce_branch(Node* root){
	bool reduced = false;
	assert(root != NULL);

	printf("type: %d - (%p)\n", root->type, root);
	assert(root->parent != NULL);

	while (root->parent->type == NOT)
		root = root->parent;

	if (root->parent->type == OR || root->parent->type == AND){
		printf("%d - %d\n", root->parent->type, root->type);
		Node* accessible_operators[5];
		int options = accessible(accessible_operators, root);

		printf("%p('%c') has `%d` options [operator = %d]\n", root, root->value, options, root->parent->type);

		for (int i = 0; i < options; i++){
			printf("%d\n", accessible_operators[i]->type);
			Node* other = accessible_operators[i]->left == root ? 
				accessible_operators[i]->right : accessible_operators[i]->left;

			printf("%p - %p %d\n", root, other, other->type);
			if (same_value_reduce(root, other)){
				printf("%p - %p %d\n", root, other, other->type);
				return true;
			}
			

			switch (root->parent->type){
				case AND:
					reduced = reduced | nested_value_reduce(root, other, OR);
					break;
				case OR:
					reduced = reduced | nested_value_reduce(root, other, AND);
					break;
			}

			// if (root->left != NULL && root->left->type != VAR)
			//	reduced = reduced | reduce_tree(root->left);

			//if (root->right != NULL && root->right->type != VAR)
			//	reduced = reduced | reduce_tree(root->right);
		}

	} else {
		assert(root->parent->type == THEN || root->parent->type == BTHEN);
		reduced = reduced | reduce_then_bthen(root);
	}

	if (root->left != NULL && root->left->type != VAR)
		reduced = reduced | reduce_tree(root->left);

	if (root->right != NULL && root->right->type != VAR)
		reduced = reduced | reduce_tree(root->right);

	return reduced;
}
Node* init;
bool reduce_tree(Node* root){
	printf("reduce_tree\n");
	if (root->parent == NULL && root->type == OPEN){
		init = root;
		root = root->left;
	}
	treeprint(init);

	assert(root != NULL);
	assert(root->type != VAR);
	assert(root->type != OPEN);
	assert(root->type != CLOSE);
	assert(root->left != NULL);

	bool reduced = false;
		
	if (root->left != NULL)
		reduced = reduced | reduce_branch(root->left);
	if (root->right != NULL)
		reduced = reduced | reduce_branch(root->right);

	return reduced;
}
