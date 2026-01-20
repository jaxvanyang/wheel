#pragma once

#include "../core.h"

typedef enum {
	HEART = 0, // ♥️
	DIAMOND = 1, // ♦️
	SPADE = 2, // ♠️
	CLUB = 3, // ♣️
} Suit;

typedef enum {
	ACE = 0,
	TWO = 1,
	THREE = 2,
	FOUR = 3,
	FIVE = 4,
	SIX = 5,
	SEVEN = 6,
	EIGHT = 7,
	NINE = 8,
	TEN = 9,
	JACK = 10,
	QUEEN = 11,
	KING = 12,
} Rank;

typedef struct {
	Suit suit;
	Rank rank;
} Card;

typedef struct {
	Card cards[52];
	size_t len;
} Deck;

typedef struct {
	Card cards[5];
	usize len;
} PubCards;

typedef struct {
	Card cards[2];
	// show cards[i] if mask[i] == true
	bool mask[2];
} Hand;

typedef enum {
	HIGH_CARD = 0,
	PAIR = 1,
	TWO_PAIRS = 2,
	THREE_OF_A_KIND = 3,
	STRAIGHT = 4,
	FLUSH = 5,
	FULL_HOUSE = 6,
	FOUR_OF_A_KIND = 7,
	STRAIGHT_FLUSH = 8,
	ROYAL_FLUSH = 9,
} Kind;

// If kind is the same, compare cards from left to right
typedef struct {
	Card cards[5];
	Kind kind;
} Selection;

Card card_from_num(u8 num);
u8 card_to_num(Card card);
// Return a debug string representation of the card
// NOTE: need to be freed
char *card_debug(Card card);
// Return a string representation of the card
// NOTE: need to be freed
char *card_display(Card card);
Deck new_deck();
Card deck_pop(Deck *deck);
Hand new_empty_hand();
void deal_pub_cards(Deck *deck, PubCards *pub);
void deal_hand(Deck *deck, Hand *hand);

// Return a - b, ACE is higher than KING
i8 cmp_rank(Rank a, Rank b);
// Sort cards from highest to lowest, ACE to 2
void sort_cards(Card *cards, usize len);
// NOTE: cards must have been sorted
Selection new_selection(const Card cards[5]);
// Return negative if a < b, 0 if a == b, positive if a > b
i8 cmp_selection(const Selection *a, const Selection *b);
Selection get_best_selection(const PubCards *pub, const Hand *hand);
// Return a string representation of the kind
// NOTE: no need to free
const char *kind_display(Kind kind);
void print_selection(const Selection *selection);
