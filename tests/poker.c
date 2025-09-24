#include <assert.h>
#include <wheel/game/poker.h>

int main() {
	{
		Card cards[7] = {
			{0, 0},
			{1, 1},
			{2, 2},
			{3, 3},
			{0, 4},
			{1, 5},
			{2, 6},
		};
		Card sorted[7] = {
			{0, 0},
			{2, 6},
			{1, 5},
			{0, 4},
			{3, 3},
			{2, 2},
			{1, 1},
		};
		sort_cards(cards, 7);

		for (usize i = 0; i < 7; ++i) {
			assert(cards[i].rank == sorted[i].rank);
			assert(cards[i].suit == sorted[i].suit);
		}
	}

	{
		PubCards pub = {
			.cards =
				{
					{CLUB, FIVE},
					{HEART, ACE},
					{DIAMOND, SIX},
					{DIAMOND, THREE},
					{CLUB, QUEEN},
				},
			.len = 5,
		};
		Hand hand = {
			.cards = {
				{DIAMOND, TWO},
				{CLUB, TEN},
			},
		};
		Selection best = {
			.cards =
				{
					{HEART, ACE},
					{CLUB, QUEEN},
					{CLUB, TEN},
					{DIAMOND, SIX},
					{CLUB, FIVE},
				},
			.kind = HIGH_CARD,
		};
		Selection sel = get_best_selection(&pub, &hand);

		assert(cmp_selection(&best, &sel) == 0);
	}
}
