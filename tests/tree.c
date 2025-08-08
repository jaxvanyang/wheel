#include <assert.h>
#include <basics/dequeue.h>
#include <basics/list.h>
#include <basics/random.h>
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
		Ilist *levelorder = ilist_from((isize[]){1, 2, 3, 4, 5}, 5);

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

		tree_print(tree);

		assert(tree_equal(n1, tree));
		assert(tree_height(tree) == 3);
		assert(ilist_equal(preorder, tree_preorder(tree)));
		assert(ilist_equal(inorder, tree_inorder(tree)));
		assert(ilist_equal(postorder, tree_postorder(tree)));
		assert(ilist_equal(levelorder, tree_levelorder(tree)));

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

		tree_print(tree->root);

		bst_free(tree);
	}

	{
		usize n = 1000;
		BST *tree = bst_new();

		for (usize i = 0; i < n; ++i) {
			bst_insert(tree, random_isize());
		}

		Ilist *inorder = tree_inorder(tree->root);

		for (usize i = 1; i < inorder->length; ++i) {
			assert(ilist_get(inorder, i - 1) <= ilist_get(inorder, i));
		}
	}

	{
		Tree *tree = tree_new(1);
		Dequeue *queue = dequeue_new();

		dequeue_push_back(queue, (isize)tree);

		while (!dequeue_empty(queue)) {
			Tree *node = (Tree *)dequeue_pop_front(queue);
			usize l = node->value << 1;
			usize r = l | 1;

			if (l < 62) {
				Tree *left = tree_new(l);
				tree_insert(node, left, true);
				dequeue_push_back(queue, (isize)left);
			}
			if (r < 62) {
				Tree *right = tree_new(r);
				tree_insert(node, right, false);
				dequeue_push_back(queue, (isize)right);
			}
		}

		tree_print(tree);

		dequeue_free(queue);
		tree_free(tree);
	}
}
