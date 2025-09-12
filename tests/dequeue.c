#include <assert.h>
#include <wheel/dequeue.h>

int main() {
	{
		Dequeue *queue = dequeue_new();

		for (isize i = 0; i <= 100; ++i) {
			dequeue_push_front(queue, i);

			if (queue->size) {
				assert(queue->head->prev == NULL);
				assert(queue->tail->next == NULL);
			}
		}

		for (isize i = 100; i >= 0; --i) {
			isize value = dequeue_pop_front(queue);
			assert(value == i);

			if (queue->size) {
				assert(queue->head->prev == NULL);
				assert(queue->tail->next == NULL);
			}
		}

		dequeue_free(queue);
	}

	{
		Dequeue *queue = dequeue_new();

		for (isize i = 0; i <= 100; ++i) {
			dequeue_push_back(queue, i);

			if (queue->size) {
				assert(queue->head->prev == NULL);
				assert(queue->tail->next == NULL);
			}
		}

		for (isize i = 100; i >= 0; --i) {
			isize value = dequeue_pop_back(queue);
			assert(value == i);

			if (queue->size) {
				assert(queue->head->prev == NULL);
				assert(queue->tail->next == NULL);
			}
		}

		dequeue_free(queue);
	}
}
