#pragma once

#include <raylib.h>

#include "../core.h"
#include "../xray.h"

extern const f32 CARD_SCALE;
extern const u8 CARD_HEIGHT;
extern const u8 CARD_WIDTH;

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

typedef struct {
	Texture2D cards;
} ResManager;

typedef struct {
	ResManager manager;
	PubCards pub_cards;
	Hand hand;
} Game;

Card card_from_num(u8 num);
u8 card_to_num(Card card);
Deck new_deck();

ResManager new_res_manager();
Game new_game();
void refresh(Game *game);

void handle_input(Game *game);

void draw_card(const ResManager *manager, Card card, Vector2 pos);
void draw_back(const ResManager *manager, u8 color, u8 style, Vector2 pos);
void draw_table();
void draw_pub_cards(const ResManager *manager, const PubCards *cards);
void draw_hand(const ResManager *manager, const Hand *hand);
void draw(const Game *game);
