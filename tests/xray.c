#include <assert.h>
#include <wheel/xray.h>

#include <raymath.h>

int main() {
	{
		Vector2List *list = v2list_new();

		for (f32 i = 0; i <= 100; ++i) {
			v2list_push_front(list, (Vector2){i, i});

			if (list->size) {
				assert(list->head->prev == NULL);
				assert(list->tail->next == NULL);
			}
		}

		for (f32 i = 0; i <= 100; ++i) {
			Vector2 value = v2list_pop_back(list);
			assert(Vector2Equals(value, (Vector2){i, i}));

			if (list->size) {
				assert(list->head->prev == NULL);
				assert(list->tail->next == NULL);
			}
		}

		v2list_free(list);
	}

	{
		Vector2List *list = v2list_new();

		for (f32 i = 0; i <= 100; ++i) {
			v2list_push_back(list, (Vector2){i, i});

			if (list->size) {
				assert(list->head->prev == NULL);
				assert(list->tail->next == NULL);
			}
		}

		for (f32 i = 0; i <= 100; ++i) {
			Vector2 value = v2list_pop_front(list);
			assert(Vector2Equals(value, (Vector2){i, i}));

			if (list->size) {
				assert(list->head->prev == NULL);
				assert(list->tail->next == NULL);
			}
		}

		v2list_free(list);
	}
}
