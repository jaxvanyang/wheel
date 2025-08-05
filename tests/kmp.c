#include <assert.h>
#include <data/str.h>

int main() {
	{
		char a[] = "aaabaaaab";
		isize next[] = {-1, 0, 1, 2, 0, 1, 2, 3, 3};
		IsizeList list = build_next(a);

		for (usize i = 0; i < list.length; ++i) {
			assert(isize_list_get(&list, i) == next[i]);
		}
	}

	{
		char a[] = "aaaab";
		isize nextval[] = {-1, -1, -1, -1, 3};
		IsizeList list = build_nextval(a);

		for (usize i = 0; i < list.length; ++i) {
			assert(isize_list_get(&list, i) == nextval[i]);
		}
	}

	{
		char a[] = "aaabaaaab";
		char b[] = "aaaab";

		assert(kmp(a, b) == 4);
	}

	{
		char a[] = "aaabaaaab";
		char b[] = "aaacb";

		assert(kmp(a, b) == -1);
	}
}
