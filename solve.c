#include "solve.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>


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

			tautology(parent);
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

			contradiction(parent);
			return true;
		case OR:
			// POSITIVE
			// p | !p = T

			// NEGATIVE
			// !p | p = T

			// The left and right should be deleted, and the root should be replaced with a TAUTOLOGY

			tautology(parent);
			return true;
		case THEN:
			assert(left->parent == right->parent);
			// POSITIVE
			// p -> !p = !p|!p = !p

			// NEGATIVE
			// !p -> p = p|p = p

			// The root and right should be deleted, and replaced with RIGHT

			replace_parent_with_child(right->parent, right);
			return true;
		case BTHEN:
			assert(left->parent == right->parent);
			// POSITIVE
			// p <-> !p = (p -> !p) & (!p -> p) = (!p|!p) & (p|p) = !p & p = F

			// NEGATIVE
			// !p <-> p = (!p -> p) & (p -> !p) = (p|p) & (!p|!p)= p & !p = F

			// The left and right should be deleted, and the root should be replaced with a CONTRADICTION

			contradiction(parent);
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

	assert(left != NULL);
	assert(right != NULL);
	assert(left->parent != NULL);
	assert(right->parent != NULL);

	Node* parent = left->parent;

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
			contradiction(parent);
			return true;
		} else if (left->type == OPEN){
			// replace OPERATOR with RIGHT
			if (left->parent == right->parent || right->parent->right == right)
				replace_parent_with_child(right->parent, right);
			else
				replace_parent_with_child(right->parent, right->parent->left);
		} else if (right->type == OPEN){
			// replace OPERATOR with LEFT
			if (left->parent == right->parent || right->parent->left == left)
				replace_parent_with_child(right->parent, left);
			else
				replace_parent_with_child(right->parent, right->parent->right);
		}
		break;
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
			tautology(parent);
			return true;
		} else if (left->type == CLOSE){
			// replace OPERATOR with RIGHT
			if (left->parent == right->parent || right->parent->right == right)
				replace_parent_with_child(right->parent, right);
			else
				replace_parent_with_child(right->parent, right->parent->left);
		} else if (right->type == CLOSE){
			// replace OPERATOR with LEFT
			if (left->parent == right->parent || right->parent->left == left)
				replace_parent_with_child(right->parent, left);
			else
				replace_parent_with_child(right->parent, right->parent->right);
		}
		break;
	case THEN:
		//printf("%p - %d <> %p - %d\n", left->parent, left->parent->type, right->parent, right->parent->type);
		printf("%p <> %p\n", left->parent, right->parent);
		assert(left->parent == right->parent);
		// THEN
		// T -> p = p
		// T -> T = T
		// T -> F = F

		// F -> p = T
		// F -> F = T
		// F -> T = T

		if (left->type == CLOSE || right->type == CLOSE){
			// replace OPERATOR with OPEN
			tautology(parent);
			return true;
		} else if (left->type == OPEN){
			// replace OPERATOR with RIGHT
			if (left->parent == right->parent || right->parent->right == right)
				replace_parent_with_child(right->parent, right);
			else
				replace_parent_with_child(right->parent, right->parent->left);
		} else if (right->type == OPEN){
			// replace OPERATOR with LEFT
			if (left->parent == right->parent || right->parent->left == left)
				replace_parent_with_child(right->parent, left);
			else
				replace_parent_with_child(right->parent, right->parent->right);
		}
		break;
	case BTHEN:
		assert(left->parent == right->parent);
		// BIDERECTIONAL THEN
		// T <-> p = (T -> p) & (p -> T) = p & T = p
		// F <-> p = (F -> p) & (p -> F) = T & !p = !p
		// T <-> T = (T -> T) & (T -> T) = T
		// F <-> F = (F -> F) & (F -> F) = T & T = T
		// T <-> F = (T -> F) & (F -> T) = F & T = F
		if (left->type == CLOSE || right->type == CLOSE){
			// replace OPERATOR with OPEN
			tautology(parent);
			return true;
		} else if (left->type == OPEN){
			// replace OPERATOR with RIGHT
			if (left->parent == right->parent || right->parent->right == right)
				replace_parent_with_child(right->parent, right);
			else
				replace_parent_with_child(right->parent, right->parent->left);
		} else if (right->type == OPEN){
			// replace OPERATOR with LEFT
			if (left->parent == right->parent || right->parent->left == left)
				replace_parent_with_child(right->parent, left);
			else
				replace_parent_with_child(right->parent, right->parent->right);
		} else if (left->type == CLOSE){
			
			// replace OPERATOR with NOT RIGHT
		} else if (right->type == CLOSE){
			// replace OPERATOR with NOT LEFT
		}
		break;
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
	if (parent->left != NULL && parent->left->type != VAR && parent->left->type != OPEN && parent->left->type != CLOSE)
		reduced = reduced | reduce_tree(parent->left);

	if (parent->right != NULL && parent->right->type != VAR && parent->right->type != OPEN && parent->right->type != CLOSE)
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
	if (parent->left != NULL && parent->left->type != VAR && parent->left->type != OPEN && parent->left->type != CLOSE){
		reduced = reduce_tree(parent->left);

		if (reduced 
				&& parent != NULL 
				&& (parent->type == OPEN || parent->type == CLOSE)
				&& parent->parent != NULL 
				&& reduce_tree(parent->parent)){
			return true;
		}
	}


	//unpack_then(root->right);
	//nested_value_reduce(root->right, AND);
	

	if (parent->right != NULL && parent->right->type != VAR && parent->right->type != OPEN && parent->right->type != CLOSE){
		reduced = reduce_tree(parent->right);

		if (reduced 
				&& parent != NULL 
				&& (parent->type == OPEN || parent->type == CLOSE)
				&& parent->parent != NULL 
				&& reduce_tree(parent->parent)){
			return true;
		}
	}

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
	assert(root->parent->type != VAR);
	assert(root->parent->type != OPEN);
	assert(root->parent->type != CLOSE);

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
			if (same_value_reduce(root, other) || bool_value_reduce(root, other)){
				printf("%p - %p %d\n", root, other, other->type);

				if (root->parent != NULL && root->parent->parent != NULL)
					reduce_branch(root->parent);
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
		}

	} else if (root->parent->type == NOT) {
		return collapse_negation(root->parent);
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
