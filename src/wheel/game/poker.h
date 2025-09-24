#pragma once

#include <raylib.h>

#include "../core.h"
#include "../xray.h"

extern const f32 CARD_SCALE;
extern const u8 CARD_WIDTH;
extern const u8 CARD_HEIGHT;
extern const u8 MINICARD_WIDTH;
extern const u8 MINICARD_HEIGHT;

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
	usize len;
} Deck;

typedef struct {
	Card cards[5];
	usize len;
} PubCards;

typedef struct {
	Card cards[2];
	usize len;
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

typedef struct {
	Texture2D cards;
	Texture2D minicards;
} ResManager;

typedef struct {
	ResManager manager;
	PubCards pub_cards;
	Hand hand;
} Game;

Card card_from_num(u8 num);
u8 card_to_num(Card card);
// Return a debug string representation of the card
// NOTE: need to be freed
char *card_debug(Card card);
Deck new_deck();
// Return a - b, ACE is higher than KING
i8 cmp_rank(Rank a, Rank b);
// Sort cards from highest to lowest
void sort_cards(Card *cards, usize len);
Selection new_selection(const Card cards[5]);
// Return negative if a < b, 0 if a == b, positive if a > b
i8 cmp_selection(const Selection *a, const Selection *b);
Selection get_best_selection(const PubCards *pub, const Hand *hand);
// Return a string representation of the kind
// NOTE: no need to free
const char *kind_display(Kind kind);
void print_selection(const Selection *selection);

ResManager new_res_manager();
Game new_game();
void refresh(Game *game);

void handle_input(Game *game);

void draw_card(const ResManager *manager, Card card, Vector2 pos);
void draw_minicard(const ResManager *manager, Card card, Vector2 pos);
void draw_back(const ResManager *manager, u8 color, u8 style, Vector2 pos);
void draw_table();
void draw_pub_cards(const ResManager *manager, const PubCards *cards);
void draw_hand(const ResManager *manager, const Hand *hand);
void draw_kind(Kind kind, i32 x, i32 y, i32 font_size);
void draw_selection(const ResManager *manager, const Selection *selection);
void draw(const Game *game);
