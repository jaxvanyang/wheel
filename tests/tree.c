#include <assert.h>
#include <basics/tree.h>
#include <stdio.h>

int main() {
	{
		//   1
		// // \\
		// 2   3
		//  \ //
		//   4
		Tree *a = tree_new(1);
		Tree *b = tree_new(2);
		Tree *c = tree_new(3);
		Tree *d = tree_new(4);

		tree_insert(a, b, true);
		tree_insert(a, c, false);
		tree_insert(b, d, false);
		tree_insert(c, d, true);
		b->right = d;

		tree_free(a);
	}

	{
		//     1
		//    / \
		//   2   3
		//  / \
		// 4   5
		Ilist *preorder = ilist_from((isize[]){1, 2, 4, 5, 3}, 5);
		Ilist *inorder = ilist_from((isize[]){4, 2, 5, 1, 3}, 5);
		Ilist *postorder = ilist_from((isize[]){4, 5, 2, 3, 1}, 5);

		Tree *tree = tree_build(preorder, inorder);

		Tree *n1 = tree_new(1);
		Tree *n2 = tree_new(2);
		Tree *n3 = tree_new(3);
		Tree *n4 = tree_new(4);
		Tree *n5 = tree_new(5);

		tree_insert(n1, n2, true);
		tree_insert(n1, n3, false);
		tree_insert(n2, n4, true);
		tree_insert(n2, n5, false);

		assert(ilist_equal(preorder, tree_preorder(tree)));
		assert(ilist_equal(inorder, tree_inorder(tree)));
		assert(ilist_equal(postorder, tree_postorder(tree)));
		assert(tree_height(tree) == 3);

		// TODO:
		// assert(tree_equal(n1, tree));
		assert(ilist_equal(preorder, tree_preorder(n1)));
		assert(ilist_equal(inorder, tree_inorder(n1)));
		assert(ilist_equal(postorder, tree_postorder(n1)));

		tree_free(tree);
		tree_free(n1);
	}

	{
		//     _4_
		//    /   \
		//   2     6
		//  / \   / \
		// 1   3 5   7
		Ilist *preorder = ilist_from((isize[]){4, 2, 1, 3, 6, 5, 7}, 7);
		Ilist *inorder = ilist_from((isize[]){1, 2, 3, 4, 5, 6, 7}, 7);

		BST *tree = bst_new();
		for (usize i = 0; i < preorder->length; ++i) {
			bst_insert(tree, ilist_get(preorder, i));
		}

		assert(ilist_equal(preorder, tree_preorder(tree->root)));
		assert(ilist_equal(inorder, tree_inorder(tree->root)));

		assert(tree_height(tree->root) == 3);

		bst_free(tree);
	}
}
