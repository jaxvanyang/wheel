#include "tree.h"
#include "basics/list.h"
#include <stdlib.h>

Tree *tree_new(isize value) {
	Tree *tree = (Tree *)malloc(sizeof(Tree));
	tree->value = value;
	tree->parent = tree->left = tree->right =  NULL;

	return tree;
}

void tree_free(Tree *tree) {
	if (tree == NULL) return;

	if (tree->parent) {
		if (tree->parent->left == tree) {
			tree->parent->left = NULL;
		}
		if (tree->parent->right == tree) {
			tree->parent->right = NULL;
		}
		tree->parent = NULL;
	}

	if (tree->left && tree->left->parent == tree) {
		tree_free(tree->left);
	}
	tree->left = NULL;
	if (tree->right && tree->right->parent == tree) {
		tree_free(tree->right);
	}
	tree->right = NULL;

	tree->value = 0;
	free(tree);
}

Tree *tree_root(Tree *tree) {
	if (tree == NULL) return NULL;

	while (tree->parent) {
		tree = tree->parent;
	}

	return tree;
}

void _tree_preorder(Ilist *list, Tree *node) {
	if (node) {
		ilist_push(list, node->value);
		_tree_preorder(list, node->left);
		_tree_preorder(list, node->right);
	}
}

Ilist *tree_preorder(Tree *tree) {
	Ilist *list = ilist_new();
	_tree_preorder(list, tree);

	return list;
}

void _tree_inorder(Ilist *list, Tree *node) {
	if (node) {
		_tree_inorder(list, node->left);
		ilist_push(list, node->value);
		_tree_inorder(list, node->right);
	}
}

Ilist *tree_inorder(Tree *tree) {
	Ilist *list = ilist_new();
	_tree_inorder(list, tree);

	return list;
}

void _tree_postorder(Ilist *list, Tree *node) {
	if (node) {
		_tree_postorder(list, node->left);
		_tree_postorder(list, node->right);
		ilist_push(list, node->value);
	}
}

Ilist *tree_postorder(Tree *tree) {
	Ilist *list = ilist_new();
	_tree_postorder(list, tree);

	return list;
}

usize tree_size(Tree *tree) {
	if (tree == NULL) return 0;

	return 1 + tree_size(tree->left) + tree_size(tree->right);
}

void tree_insert(Tree *parent, Tree *node, bool is_left) {
	if (parent == NULL) return;

	if (node) {
		if (node->parent) {
			if (node->parent->left == node) {
				node->parent->left = NULL;
			}
			if (node->parent->right == node) {
				node->parent->right = NULL;
			}
		}
		node->parent = parent;
	}

	if (is_left) {
		tree_free(parent->left);
		parent->left = node;
	} else {
		tree_free(parent->right);
		parent->right = node;
	}
}

Tree *tree_build(Ilist *preorder, Ilist *inorder) {
	if (preorder->length != inorder->length) {
		error("expected equaled lengths: %llu != %llu\n", preorder->length, inorder->length);
	}

	// TODO: check if each element is unique

	if (preorder->length == 0) {
		return NULL;
	}

	Tree *tree = tree_new(ilist_get(preorder, 0));
	usize p = 0;

	while (p < inorder->length && ilist_get(inorder, p) != tree->value) {
		++p;
	}

	Ilist *left_inorder = ilist_new_with_size(p);
	Ilist *right_inorder = ilist_new_with_size(inorder->length - p - 1);
	for (usize i = 0; i < p; ++i) {
		ilist_push(left_inorder, ilist_get(inorder, i));
	}
	for (usize i = p + 1; i < inorder->length; ++i) {
		ilist_push(right_inorder, ilist_get(inorder, i));
	}

	Ilist *left_preorder = ilist_new_with_size(left_inorder->length);
	Ilist *right_preorder = ilist_new_with_size(right_inorder->length);
	for (usize i = 1; i < preorder->length; ++i) {
		isize val = ilist_get(preorder, i);
		bool is_left = false;

		// TODO: add find functions to list
		for (usize j = 0; j < left_inorder->length; ++j) {
			if (val == ilist_get(left_inorder, j)) {
				is_left = true;
				break;
			}
		}

		if (is_left) {
			ilist_push(left_preorder, val);
		} else {
			ilist_push(right_preorder, val);
		}
	}

	tree_insert(tree, tree_build(left_preorder, left_inorder), true);
	tree_insert(tree, tree_build(right_preorder, right_inorder), false);

	return tree;
}

usize tree_height(Tree *tree) {
	if (tree == NULL) return 0;

	usize h = 1 + max(tree_height(tree->left), tree_height(tree->right));

	return h;
}

BST *bst_new() {
	BST *tree = malloc(sizeof(BST));
	tree->size = 0;
	tree->root = NULL;

	return tree;
}

void bst_free(BST *tree) {
	tree_free(tree->root);
	tree->size = 0;
	tree->root = NULL;

	free(tree);
}

void bst_insert(BST *tree, isize value) {
	Tree *node = tree_new(value);

	if (tree->root == NULL) {
		tree->root = node;
		return;
	}

	Tree *p = tree->root;

	while (true) {
		if (value < p->value) {
			if (p->left) {
				p = p->left;
				continue;
			} else {
				tree_insert(p, node, true);
				return;
			}
		} else {
			if (p->right) {
				p = p->right;
				continue;
			} else {
				tree_insert(p, node, false);
				return;
			}
		}
	}
}

Tree *bst_search(BST *tree, isize value) {
	if (tree == NULL) return NULL;

	Tree *p = tree->root;

	while (p && p->value != value) {
		if (value < p->value) {
			p = p->left;
		} else {
			p = p->right;
		}
	}

	return p;
}
