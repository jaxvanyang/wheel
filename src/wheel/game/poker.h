#pragma once

#include <raylib.h>

#include "../core.h"
#include "../xray.h"

extern const f32 CARD_SCALE;
extern const u8 CARD_WIDTH;
extern const u8 CARD_HEIGHT;
extern const u8 MINICARD_WIDTH;
extern const u8 MINICARD_HEIGHT;
extern const u8 CHIP_WIDTH;
extern const u8 CHIP_HEIGHT;

#define SEAT_CNT 5

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

typedef struct {
	Texture2D cards;
	Texture2D minicards;
	Texture2D chips;
	Texture2D button;
} ResManager;

typedef enum {
	STATE_STANDBY, // waiting to join the game
	STATE_WAITING, // waiting in the game
	STATE_THINKING,
	STATE_FOLD,
	STATE_CHECK,
	STATE_CALL,
	STATE_RAISE,
	STATE_ALLIN,
} PlayerState;

typedef struct {
	Hand hand;
	const char *name;
	usize chips;
	usize bet;
	bool is_valid;
	PlayerState state;
} Player;

typedef struct {
	ResManager manager;
	PubCards pub_cards;
	Player players[SEAT_CNT];
	usize my_seat;
	usize cur; // current player's seat number
	usize dealer;
	usize pot;
	usize slider;
} Game;

Card card_from_num(u8 num);
u8 card_to_num(Card card);
// Return a debug string representation of the card
// NOTE: need to be freed
char *card_debug(Card card);
Deck new_deck();
Hand new_empty_hand();
void deal_hand(Deck *deck, Hand *hand);
// Return next player's seat number.
usize get_next_player(const Game *game, usize seat);

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

// Return a one-time string of the player's state.
const char *format_player_state(PlayerState state);
Player new_player(const char *name, usize chips, bool is_valid);

Game new_game();
void refresh(Game *game);

void handle_input(Game *game);

// NOTE: the seat is relative to my seat
Rectangle get_player_widget(usize seat);

void draw_card(const ResManager *manager, Card card, Vector2 pos);
void draw_minicard(const ResManager *manager, Card card, Vector2 pos);
void draw_back(const ResManager *manager, u8 color, u8 style, Vector2 pos);
void draw_chip(const ResManager *manager, u8 color, u8 amount, Vector2 pos);
void draw_pot(const ResManager *manager, usize pot);
void draw_button(const ResManager *manager, i32 x, i32 y);
void draw_table();
void draw_pub_cards(const ResManager *manager, const PubCards *cards);
void draw_hand(const ResManager *manager, const Hand *hand, Vector2 pos);
// NOTE: the seat is relative to my seat
void draw_player(
	const ResManager *manager, const Player *player, usize seat, bool card_on_left,
	bool is_dealer
);
void draw_kind(Kind kind, i32 x, i32 y, i32 font_size);
void draw_selection(const ResManager *manager, const Selection *selection);
void draw_slider(usize slider);
void draw(const Game *game);
