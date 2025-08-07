//! Binary Tree

#include "core.h"
#include "list.h"

typedef struct Tree {
	isize value;
	struct Tree *parent;
	struct Tree *left;
	struct Tree *right;
} Tree;

Tree *tree_new(isize value);
void tree_free(Tree *tree);

Tree *tree_root(Tree *tree);

Ilist *tree_preorder(Tree *tree);
Ilist *tree_inorder(Tree *tree);
Ilist *tree_postorder(Tree *tree);

usize tree_size(Tree *tree);
void tree_insert(Tree *parent, Tree *node, bool is_left);
