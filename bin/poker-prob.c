/*
 * poker-prob - simulate probabilities of different kinds in Poker
 *
 * poker-prob [n]
 */

#include <time.h>
#include <wheel.h>
#include <wheel/game/poker.h>

void simulate(usize n);
Kind random_kind();
void print_progress(usize cur, usize n);

int main(const int argc, const char *argv[]) {
	SetRandomSeed((unsigned int)time(NULL));

	usize n = 1000000;
	if (argc >= 2) {
		sscanf(argv[1], "%" USIZE_FMT, &n);
	}

	simulate(n);
}

void simulate(usize n) {
	usize cnts[10] = {};
	f32 probs[10] = {};

	print_progress(0, n);

	for (usize i = 0; i < n; ++i) {
		Kind kind = random_kind();
		++cnts[kind];

		term_clear_line();
		print_progress(i + 1, n);
	}

	term_clear_line();

	printf("| Kind            |                Count |       Prob |\n");
	printf("|-----------------|----------------------|------------|\n");

	for (Kind i = 0; i < 10; ++i) {
		probs[i] = (f32)cnts[i] / (f32)n * 100;
		printf(
			"| %-15s | %20" USIZE_FMT " | %9.5f%% |\n", kind_display(i), cnts[i], probs[i]
		);
	}

	printf("|-----------------|----------------------|------------|\n");
	printf("| Total           | %20" USIZE_FMT " | %9.5f%% |\n", n, 100.0);
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
