#include <assert.h>
#include <raylib.h>

#include "../lol.h"
#include "../str.h"
#include "card.h"

Card card_from_num(u8 num) {
	assert(num < 52);

	Suit suit = num / 13;
	Rank rank = num % 13;

	return (Card){suit, rank};
}

u8 card_to_num(Card card) {
	u8 ret = card.suit * 13 + card.rank;

	assert(ret < 52);

	return ret;
}

char *card_debug(Card card) {
	assert(card.suit < 4);
	assert(card.rank < 13);

	char *ret = malloc(3);
	ret[2] = '\0';

	if (card.suit == HEART) {
		ret[0] = 'H';
	} else if (card.suit == DIAMOND) {
		ret[0] = 'D';
	} else if (card.suit == CLUB) {
		ret[0] = 'C';
	} else {
		ret[0] = 'S';
	}

	if (card.rank == ACE) {
		ret[1] = 'A';
	} else if (card.rank == KING) {
		ret[1] = 'K';
	} else if (card.rank == QUEEN) {
		ret[1] = 'Q';
	} else if (card.rank == JACK) {
		ret[1] = 'J';
	} else if (card.rank == TEN) {
		ret[1] = 'T';
	} else {
		ret[1] = card.rank + '1';
	}

	return ret;
}

char *card_display(Card card) {
	assert(card.suit < 4);
	assert(card.rank < 13);

	Str *tmp = str_new();

	if (card.suit == HEART) {
		str_push_str(tmp, "♥️ ");
	} else if (card.suit == DIAMOND) {
		str_push_str(tmp, "♦️ ");
	} else if (card.suit == CLUB) {
		str_push_str(tmp, "♠️ ");
	} else {
		str_push_str(tmp, "♣️ ");
	}

	if (card.rank == ACE) {
		str_push(tmp, 'A');
	} else if (card.rank == KING) {
		str_push(tmp, 'K');
	} else if (card.rank == QUEEN) {
		str_push(tmp, 'Q');
	} else if (card.rank == JACK) {
		str_push(tmp, 'J');
	} else if (card.rank == TEN) {
		str_push(tmp, 'T');
	} else {
		str_push(tmp, '1' + card.rank);
	}

	char *ret = tmp->data;
	free(tmp);

	return ret;
}

Deck new_deck() {
	Deck deck = {
		.cards = {},
		.len = 52,
	};

	for (usize i = 0; i < deck.len; ++i) {
		deck.cards[i] = card_from_num(i);
	}

	// shuffle the deck
	for (usize i = 0; i < deck.len; ++i) {
		usize j = GetRandomValue(0, deck.len - 1);
		memswap(&deck.cards[i], &deck.cards[j], sizeof(Card));
	}

	return deck;
}

Card deck_pop(Deck *deck) { return deck->cards[--deck->len]; }

Hand new_empty_hand() {
	return (Hand){
		.mask = {false, false},
	};
}

void deal_pub_cards(Deck *deck, PubCards *pub) {
	for (usize i = 0; i < 5; ++i) {
		pub->cards[i] = deck_pop(deck);
	}
}

void deal_hand(Deck *deck, Hand *hand) {
	assert(deck->len >= 2);
	hand->cards[0] = deck_pop(deck);
	hand->cards[1] = deck_pop(deck);
}

i8 cmp_rank(Rank a, Rank b) {
	if (a == ACE) {
		a = KING + 1;
	}
	if (b == ACE) {
		b = KING + 1;
	}
	return (i8)a - (i8)b;
}

void sort_cards(Card *cards, usize len) {
	for (usize i = 0; i < len; ++i) {
		for (usize j = i + 1; j < len; ++j) {
			if (cmp_rank(cards[i].rank, cards[j].rank) < 0) {
				memswap(&cards[i], &cards[j], sizeof(Card));
			}
		}
	}
}

Selection new_selection(const Card cards[5]) {
	for (usize i = 0; i < 4; ++i) {
		assert(cmp_rank(cards[i].rank, cards[i + 1].rank) >= 0);
	}

	Selection ret = {
		.cards = {},
		.kind = HIGH_CARD,
	};
	for (usize i = 0; i < 5; ++i) {
		ret.cards[i] = cards[i];
	}

	bool is_flush = true;
	bool is_straight = true;
	u8 cnts[13] = {};
	u8 first_cnt = 0, second_cnt = 0;

	for (usize i = 0; i < 5; ++i) {
		++cnts[cards[i].rank];
	}
	for (usize i = 0; i < 13; ++i) {
		if (cnts[i] > first_cnt) {
			second_cnt = first_cnt;
			first_cnt = cnts[i];
		} else if (cnts[i] > second_cnt) {
			second_cnt = cnts[i];
		}
	}

	for (usize i = 0; i < 4; ++i) {
		if (cards[i].suit != cards[i + 1].suit) {
			is_flush = false;
		}

		// special case: A 5 4 3 2
		// if A is in the middle, is_straight would be set before
		if ((cards[i].rank != ACE || cards[i + 1].rank != FIVE) &&
				cmp_rank(cards[i].rank, cards[i + 1].rank) != 1) {
			is_straight = false;
		}
	}

	// A 5 4 3 2 -> 5 4 3 2 A
	if (is_straight && cards[0].rank == ACE && cards[1].rank == FIVE) {
		for (usize i = 0; i < 4; ++i) {
			ret.cards[i] = cards[i + 1];
		}
		ret.cards[4] = cards[0];
	}

	if (is_straight && is_flush) {
		if (ret.cards[0].rank == ACE) {
			ret.kind = ROYAL_FLUSH;
		} else {
			ret.kind = STRAIGHT_FLUSH;
		}
	} else if (first_cnt == 4) {
		ret.kind = FOUR_OF_A_KIND;

		// example: 4 3 3 3 3 -> 3 3 3 3 4
		if (ret.cards[0].rank != ret.cards[1].rank) {
			memswap(&ret.cards[0], &ret.cards[4], sizeof(Card));
		}
	} else if (first_cnt == 3 && second_cnt == 2) {
		ret.kind = FULL_HOUSE;

		// example: 3 3 2 2 2 -> 2 2 2 3 3
		if (ret.cards[1].rank == ret.cards[2].rank) {
			memswap(&ret.cards[0], &ret.cards[4], sizeof(Card));
			memswap(&ret.cards[1], &ret.cards[3], sizeof(Card));
		}
	} else if (is_flush) {
		ret.kind = FLUSH;
	} else if (is_straight) {
		ret.kind = STRAIGHT;
	} else if (first_cnt == 3) {
		ret.kind = THREE_OF_A_KIND;

		// example: 3 2 2 2 3 -> 2 2 2 3 3
		if (ret.cards[0].rank != ret.cards[2].rank) {
			memswap(&ret.cards[0], &ret.cards[3], sizeof(Card));
		}
		// example: 2 3 2 3 2 -> 2 2 2 3 3
		if (ret.cards[1].rank != ret.cards[2].rank) {
			memswap(&ret.cards[1], &ret.cards[4], sizeof(Card));
		}
	} else if (first_cnt == 2 && second_cnt == 2) {
		ret.kind = TWO_PAIRS;

		for (usize i = 0; i < 4; ++i) {
			if (ret.cards[i].rank == ret.cards[i + 1].rank) {
				// skip pair
				++i;
			} else {
				// found the single card
				for (usize j = i; j < 4; ++j) {
					ret.cards[j] = ret.cards[j + 1];
				}
				ret.cards[4] = cards[i];
				break;
			}
		}
	} else if (first_cnt == 2) {
		ret.kind = PAIR;

		for (usize i = 0; i < 4; ++i) {
			// find the pair first
			if (ret.cards[i].rank == ret.cards[i + 1].rank) {
				// find all single cards
				for (usize j = 0, k = 2; j < 5; ++j) {
					if (j != i && j != i + 1) {
						ret.cards[k++] = cards[j];
					}
				}

				ret.cards[0] = cards[i];
				ret.cards[1] = cards[i + 1];

				break;
			}
		}
	}

	return ret;
}

i8 cmp_selection(const Selection *a, const Selection *b) {
	if (a->kind != b->kind) {
		return (i8)a->kind - (i8)b->kind;
	}

	for (usize i = 0; i < 5; ++i) {
		i8 diff = cmp_rank(a->cards[i].rank, b->cards[i].rank);
		if (diff != 0) {
			return diff;
		}
	}

	return 0;
}

Selection get_best_selection(const PubCards *pub, const Hand *hand) {
	assert(pub->len == 5);

	Card all[7];
	Card cards[5];

	for (usize i = 0; i < 5; ++i) {
		all[i] = pub->cards[i];
	}
	for (usize i = 0; i < 2; ++i) {
		all[5 + i] = hand->cards[i];
	}
	sort_cards(all, 7);

	for (usize i = 0; i < 5; ++i) {
		cards[i] = all[i];
	}
	Selection ret = new_selection(cards);

	// iterate all combinations
	for (usize i = 0; i < 7; ++i) {
		for (usize j = i + 1; j < 7; ++j) {
			for (usize k = 0, l = 0; k < 7; ++k) {
				if (k != i && k != j) {
					cards[l++] = all[k];
				}
			}

			Selection sel = new_selection(cards);
			if (cmp_selection(&sel, &ret) > 0) {
				ret = sel;
			}
		}
	}

	return ret;
}

const char *kind_display(Kind kind) {
	switch (kind) {
	case HIGH_CARD:
		return "High Card";
	case PAIR:
		return "Pair";
	case TWO_PAIRS:
		return "Two Pair";
	case THREE_OF_A_KIND:
		return "Three of a Kind";
	case STRAIGHT:
		return "Straight";
	case FLUSH:
		return "Flush";
	case FULL_HOUSE:
		return "Full House";
	case FOUR_OF_A_KIND:
		return "Four of a Kind";
	case STRAIGHT_FLUSH:
		return "Straight Flush";
	case ROYAL_FLUSH:
		return "Royal Flush";
	default:
		lol_term("expected a valid kind");
	}
}

void print_selection(const Selection *selection) {
	lol_info("%s:", kind_display(selection->kind));
	for (usize i = 0; i < 5; ++i) {
		char *card = card_debug(selection->cards[i]);
		lol_info(" %s", card);
		FREE(card);
	}
	putchar('\n');
}
