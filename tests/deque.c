#include <assert.h>
#include <wheel/deque.h>

int main() {
	{
		Deque *queue = deque_new();

		for (isize i = 0; i <= 100; ++i) {
			deque_push_front(queue, i);

			if (queue->size) {
				assert(queue->head->prev == NULL);
				assert(queue->tail->next == NULL);
			}
		}

		for (isize i = 100; i >= 0; --i) {
			isize value = deque_pop_front(queue);
			assert(value == i);

			if (queue->size) {
				assert(queue->head->prev == NULL);
				assert(queue->tail->next == NULL);
			}
		}

		deque_free(queue);
	}

	{
		Deque *queue = deque_new();

		for (isize i = 0; i <= 100; ++i) {
			deque_push_back(queue, i);

			if (queue->size) {
				assert(queue->head->prev == NULL);
				assert(queue->tail->next == NULL);
			}
		}

		for (isize i = 100; i >= 0; --i) {
			isize value = deque_pop_back(queue);
			assert(value == i);

			if (queue->size) {
				assert(queue->head->prev == NULL);
				assert(queue->tail->next == NULL);
			}
		}

		deque_free(queue);
	}
}
