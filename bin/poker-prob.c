/*
 * poker-prob - simulate probabilities of different kinds in Poker
 *
 * poker-prob [n]
 */

#include <math.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <wheel.h>
#include <wheel/game/poker.h>

typedef struct {
	usize divider;
	usize rem;
} Filter;

static usize theory_cnts[11] = {};
static f32 theory_probs[11] = {};
static usize simulate_cnts[10] = {};
static f32 simulate_probs[10] = {};

static pthread_mutex_t iterate_lock;

void calc();
void iterate();
void *iterate_thread(void *arg);
void simulate(usize n);
Kind random_kind();
void print_progress(usize cur, usize n);

int main(const int argc, const char *argv[]) {
	SetRandomSeed((unsigned int)time(NULL));

	usize n = 1000000;
	if (argc >= 2) {
		sscanf(argv[1], "%" USIZE_FMT, &n);
	}

	// calc();
	iterate();
	simulate(n);

	// clang-format off
	printf("|----------------------------------------------------------------------------------|\n");
	printf("|                 |                  Theory |                Simulate |            |\n");
	printf("|                 |-------------------------|-------------------------|            |\n");
	printf("| Kind            |      Count |       Prob |      Count |       Prob |      Error |\n");
	printf("|-----------------|------------|------------|------------|------------|------------|\n");
	// clang-format on

	for (Kind i = 0; i < 10; ++i) {
		printf(
			"| %-15s | %10" USIZE_FMT " | %9.5f%% | %10" USIZE_FMT
			" | %9.5f%% | %+9.5f%% |\n",
			kind_display(i),
			theory_cnts[i],
			theory_probs[i],
			simulate_cnts[i],
			simulate_probs[i],
			simulate_probs[i] - theory_probs[i]
		);
	}

	// clang-format off
	printf("|-----------------|------------|------------|------------|------------|------------|\n");
	// clang-format on
	printf(
		"| Total           | %10" USIZE_FMT " | %9.5f%% | %10" USIZE_FMT
		" | %9.5f%% | %+9.5f%% |\n",
		theory_cnts[10],
		theory_probs[10],
		n,
		100.0,
		100.0 - theory_probs[10]
	);
	// clang-format off
	printf("-----------------------------------------------------------------------------------|\n");
	// clang-format on
}

void calc() {
	theory_cnts[10] = binom(52, 7);

	// 0.003%
	theory_cnts[ROYAL_FLUSH] = 4 * binom(47, 2);
	// 0.027%
	theory_cnts[STRAIGHT_FLUSH] =
		4 * (2 * binom(46, 2) + 8 * binom(45, 2) + 2 * 45 + 7 * 44 + 8) -
		theory_cnts[ROYAL_FLUSH];
	// 0.168%
	theory_cnts[FOUR_OF_A_KIND] = 13 * binom(48, 3);
	// 2.59%
	theory_cnts[FULL_HOUSE] = binom(13, 2) * pow(4, 3) * 11 +
		13 * (4 * binom(12, 2) * pow(6, 2) + pow(4, 3) * 12 * binom(4, 2) * binom(11, 2));
	// 3.025%
	theory_cnts[FLUSH] =
		4 * (binom(13, 5) * binom(39, 2) + binom(13, 6) * 39 + binom(13, 7)) -
		theory_cnts[ROYAL_FLUSH] - theory_cnts[STRAIGHT_FLUSH];
	// 4.619%
	theory_cnts[STRAIGHT] = pow(4, 5) * (2 * binom(28, 2) + 8 * binom(24, 2)) +
		pow(4, 4) * binom(4, 2) * 5 * (2 * 28 + 8 * 24) +
		pow(4, 3) * pow(6, 2) * binom(5, 2) * 10 + pow(4, 5) * 5 * 10 +
		pow(4, 6) * (2 * 24 + 7 * 20) + pow(4, 5) * binom(4, 2) * 6 * 9 + pow(4, 7) * 8 -
		theory_cnts[ROYAL_FLUSH] - theory_cnts[STRAIGHT_FLUSH];
	// 4.829%
	theory_cnts[THREE_OF_A_KIND] = binom(13, 5) * 5 * 4 * pow(4, 4) -
		binom(13, 5) * 4 * 3 - 10 * 5 * 4 * pow(4, 4) + 10 * 4 * 4 * 3;
	// 23.495%
	theory_cnts[TWO_PAIRS] = binom(13, 4) * 4 * pow(binom(4, 2), 3) * 4 -
		binom(13, 4) * 4 * 4 +
		binom(13, 5) * binom(5, 2) * pow(binom(4, 2), 2) * pow(4, 3) -
		binom(13, 5) * binom(5, 2) * 4 -
		10 * binom(5, 2) * pow(binom(4, 2), 2) * pow(4, 2) +
		10 * binom(5, 2) * 4 * pow(3, 2);
	// FIXME: this includes straight & flush
	// 43.822%
	theory_cnts[PAIR] = binom(13, 1) * binom(4, 2) * binom(12, 5) * pow(binom(4, 1), 5);
	// FIXME: this includes straight & flush
	// 17.141%
	theory_cnts[HIGH_CARD] = binom(13, 7) * pow(binom(4, 1), 7);

	for (Kind i = 0; i < 10; ++i) {
		theory_probs[i] = (f32)theory_cnts[i] / (f32)theory_cnts[10] * 100;
		theory_probs[10] += theory_probs[i];
	}
}

void iterate() {
	memset(theory_cnts, 0, sizeof(theory_cnts));
	memset(theory_probs, 0, sizeof(theory_probs));

	usize n = binom(52, 7);
	print_progress(0, n);

	usize n_thread = nproc();
	pthread_t *threads = malloc(sizeof(pthread_t) * n_thread);
	Filter *args = malloc(sizeof(Filter) * n_thread);

	pthread_mutex_init(&iterate_lock, NULL);

	for (usize i = 0; i < n_thread; ++i) {
		args[i].divider = n_thread;
		args[i].rem = i;

		pthread_create(threads + i, NULL, iterate_thread, args + i);
	}

	for (usize i = 0; i < n_thread; ++i) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&iterate_lock);

	term_clear_line();

	for (usize i = 0; i < 11; ++i) {
		theory_probs[i] = (f32)theory_cnts[i] / (f32)theory_cnts[10] * 100;
	}

	FREE(args);
	FREE(threads);
}

void *iterate_thread(void *arg) {
	Filter filter = *(Filter *)arg;

	Deck deck = {
		.len = 52,
	};
	for (usize i = 0; i < deck.len; ++i) {
		deck.cards[i] = card_from_num(i);
	}

	Card *cards = (Card *)&deck.cards;
	usize n = binom(52, 7);

	usize threshold = 10000;
	usize cnts[11] = {};

	for (usize a = 0; a < 52; ++a) {
		if (a % filter.divider != filter.rem) {
			continue;
		}
		for (usize b = a + 1; b < 52; ++b) {
			for (usize c = b + 1; c < 52; ++c) {
				for (usize d = c + 1; d < 52; ++d) {
					for (usize e = d + 1; e < 52; ++e) {
						for (usize f = e + 1; f < 52; ++f) {
							for (usize g = f + 1; g < 52; ++g) {
								PubCards pub = {
									.cards = {cards[a], cards[b], cards[c], cards[d], cards[e]},
									.len = 5,
								};
								Hand hand = {
									.cards = {cards[f], cards[g]},
								};
								Selection sel = get_best_selection(&pub, &hand);

								++cnts[sel.kind];
								++cnts[10];

								if (cnts[10] == threshold) {
									pthread_mutex_lock(&iterate_lock);

									for (usize i = 0; i < 11; ++i) {
										theory_cnts[i] += cnts[i];
										cnts[i] = 0;
									}

									term_clear_line();
									print_progress(theory_cnts[10], n);

									pthread_mutex_unlock(&iterate_lock);
								}
							}
						}
					}
				}
			}
		}
	}

	if (cnts[10] != 0) {
		pthread_mutex_lock(&iterate_lock);

		for (usize i = 0; i < 11; ++i) {
			theory_cnts[i] += cnts[i];
			cnts[i] = 0;
		}

		term_clear_line();
		print_progress(theory_cnts[10], n);

		pthread_mutex_unlock(&iterate_lock);
	}

	return NULL;
}

void simulate(usize n) {

	print_progress(0, n);

	for (usize i = 0; i < n; ++i) {
		Kind kind = random_kind();
		++simulate_cnts[kind];

		term_clear_line();
		print_progress(i + 1, n);
	}

	term_clear_line();

	for (Kind i = 0; i < 10; ++i) {
		simulate_probs[i] = (f32)simulate_cnts[i] / (f32)n * 100;
	}
}

Kind random_kind() {
	Deck deck = new_deck();

	PubCards pub = {.len = 5};
	Hand hand;

	deal_pub_cards(&deck, &pub);
	deal_hand(&deck, &hand);
	Selection sel = get_best_selection(&pub, &hand);

	return sel.kind;
}

void print_progress(usize cur, usize n) {
	f32 percent = (f32)cur / (f32)n * 100;
	usize cnt = cur * 50 / n;

	printf("[");
	for (usize i = 0; i < cnt; ++i) {
		printf("#");
	}
	for (usize i = 0; i < 50 - cnt; ++i) {
		printf(" ");
	}
	printf("] %.1f%%\n", percent);
}
