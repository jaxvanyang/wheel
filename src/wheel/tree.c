#include "tree.h"

#include <stdlib.h>
#include <string.h>

#include "wheel/dequeue.h"
#include "wheel/list.h"
#include "wheel/math.h"

Tree *tree_new(isize value) {
	Tree *tree = (Tree *)malloc(sizeof(Tree));
	tree->value = value;
	tree->parent = tree->left = tree->right = NULL;

	return tree;
}

void tree_free(Tree *tree) {
	if (tree == NULL)
		return;

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
	if (tree == NULL)
		return NULL;

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

Ilist *tree_levelorder(Tree *tree) {
	Ilist *list = ilist_new();
	Dequeue *queue = dequeue_new();

	if (tree) {
		dequeue_push_back(queue, (isize)tree);
	}

	while (!dequeue_empty(queue)) {
		Tree *node = (Tree *)dequeue_pop_front(queue);
		ilist_push(list, node->value);

		if (node->left) {
			dequeue_push_back(queue, (isize)node->left);
		}
		if (node->right) {
			dequeue_push_back(queue, (isize)node->right);
		}
	}

	return list;
}

usize tree_size(Tree *tree) {
	if (tree == NULL)
		return 0;

	return 1 + tree_size(tree->left) + tree_size(tree->right);
}

void tree_insert(Tree *parent, Tree *node, bool is_left) {
	if (parent == NULL)
		return;

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
		error("expected equaled lengths: %zu != %zu\n", preorder->length, inorder->length);
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
	if (tree == NULL)
		return 0;

	usize h = 1 + max(tree_height(tree->left), tree_height(tree->right));

	return h;
}

bool tree_equal(Tree *a, Tree *b) {
	Ilist *a_preorder = tree_preorder(a);
	Ilist *a_inorder = tree_inorder(a);
	Ilist *b_preorder = tree_preorder(b);
	Ilist *b_inorder = tree_inorder(b);

	bool ret = ilist_equal(a_preorder, b_preorder) && ilist_equal(a_inorder, b_inorder);

	ilist_free(a_preorder);
	ilist_free(a_inorder);
	ilist_free(b_preorder);
	ilist_free(b_inorder);

	return ret;
}

char _tree_char(isize n) {
	if (n < 0 || n > 62) {
		error("expected 0 <= n <= 62, but found: n = %ld\n", n);
	} else if (n < 10) {
		return '0' + n;
	} else if (n < 36) {
		return 'a' + n - 10;
	} else {
		return 'A' + n - 36;
	}
}

//      1          h = 1, w
//
//      1
//     / \
//    2   3        h = 2, w = 5
//
//     _1_         h = 3, w = 11
//    /   \
//   2     3
//  / \   / \
// 4   5 6   7
//
//        ____1____      h = 4, w = 23
//       /         \
//     _2_         _3_
//    /   \       /   \
//   4     5     6     7
//  / \   / \   / \   / \
// 8   9 a   b c   d e   f
void tree_print(Tree *tree) {
	usize h = tree_height(tree);

	if (h == 0) {
		return;
	}

	if (h == 1) {
		printf("%ld\n", tree->value);
		return;
	}

	usize H = 2 * h - 1;
	usize c = 1 << (h - 1);
	usize w = 3 * c - 1;
	usize W = w + 1;
	usize len = 1 << h;

	usize *cols = malloc(sizeof(usize) * len);

	for (usize i = c; i < len; ++i) {
		usize j = i - c;

		if (j & 1) {
			cols[i] = 4 + 6 * (j / 2);
		} else {
			cols[i] = 3 * j;
		}
	}

	for (usize i = c - 1; i > 0; --i) {
		usize l = i << 1;
		usize r = i << 1 | 1;
		cols[i] = (cols[l] + cols[r]) / 2;
	}

	Dequeue *nodes = dequeue_new();
	Dequeue *number_queue = dequeue_new();
	Ilist *values = ilist_new();
	Ulist *numbers = ulist_new();

	dequeue_push_back(nodes, (isize)tree);
	dequeue_push_back(number_queue, 1);

	while (!dequeue_empty(nodes)) {
		Tree *node = (Tree *)dequeue_pop_front(nodes);
		isize number = dequeue_pop_front(number_queue);

		ilist_push(values, node->value);
		ulist_push(numbers, number);

		if (node->left) {
			dequeue_push_back(nodes, (isize)node->left);
			dequeue_push_back(number_queue, number << 1);
		}
		if (node->right) {
			dequeue_push_back(nodes, (isize)node->right);
			dequeue_push_back(number_queue, number << 1 | 1);
		}
	}

	char *matrix = malloc(sizeof(char) * H * W);

	memset(matrix, ' ', sizeof(char) * H * W);
	for (usize i = 0; i < H - 1; ++i) {
		matrix[i * W + w] = '\n';
	}
	matrix[W * H - 1] = '\0';

	for (isize i = numbers->length - 1; i >= 0; --i) {
		usize number = ulist_get(numbers, i);
		isize value = ilist_get(values, i);

		usize layer = usize_log2(number);
		usize row = layer << 1;
		usize col = cols[number];

		if (layer != 0) {
			if (number & 1) {
				matrix[(row - 1) * W + col - 1] = '\\';
				for (usize k = cols[number >> 1] + 1; k < col - 1; k++) {
					matrix[(row - 2) * W + k] = '_';
				}
			} else {
				matrix[(row - 1) * W + col + 1] = '/';
				for (usize k = col + 2; k < cols[number >> 1]; k++) {
					matrix[(row - 2) * W + k] = '_';
				}
			}
		}

		matrix[row * W + col] = _tree_char(value);
	}

	printf("%s\n", matrix);

	dequeue_free(nodes);
	dequeue_free(number_queue);
	ilist_free(values);
	ulist_free(numbers);

	free(matrix);
	free(cols);
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
	if (tree == NULL)
		return NULL;

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
