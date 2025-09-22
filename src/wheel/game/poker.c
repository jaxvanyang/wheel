#include <assert.h>

#include "poker.h"

const f32 CARD_SCALE = 2;
// scaled by CARD_SCALE
const u8 CARD_HEIGHT = 128;
const u8 CARD_WIDTH = 96;

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
		swap(deck.cards[i], deck.cards[j]);
	}

	return deck;
}

ResManager new_res_manager() {
	ResManager manager = {
		.cards = load_texture("assets/poker/poker_cards.png"),
	};

	return manager;
}

Game new_game() {
	Game game = {
		.manager = new_res_manager(),
	};

	refresh(&game);
	game.pub_cards.len = 5;
	game.hand.len = 2;

	return game;
}

void refresh(Game *game) {
	Deck deck = new_deck();

	for (usize i = 0; i < 5; ++i) {
		game->pub_cards.cards[i] = deck.cards[--deck.len];
	}

	for (usize i = 0; i < 2; ++i) {
		game->hand.cards[i] = deck.cards[--deck.len];
	}
}

void handle_input(Game *game) {
	if (IsKeyPressed(KEY_SPACE)) {
		refresh(game);
	}

	if (IsKeyPressed(KEY_D)) {
		game->pub_cards.len = (game->pub_cards.len + 1) % 6;
	}
}

void draw_card(const ResManager *manager, Card card, Vector2 pos) {
	Rectangle source = {
		.x = card.rank * 48, .y = card.suit * 64, .width = 48, .height = 64
	};
	draw_texture_rec_scale(manager->cards, source, pos, WHITE, CARD_SCALE);
}

void draw_back(const ResManager *manager, u8 color, u8 style, Vector2 pos) {
	assert(color < 4);
	assert(style < 2);

	Rectangle source = {
		.x = (color * 2 + style) * 48, .y = 4 * 64, .width = 48, .height = 64
	};
	draw_texture_rec_scale(manager->cards, source, pos, WHITE, CARD_SCALE);
}

void draw_table() {
	Color trans_brown = DARKBROWN;
	trans_brown.a = 0xa0;
	Color trans_gray = BLACK;
	trans_gray.a = 0x20;

	i32 width = GetScreenWidth();
	i32 height = GetScreenHeight();

	i32 margin = 5;
	f32 thick = 5;
	i32 pub_width = margin * 8 + (CARD_WIDTH + 2 * margin) * 5;
	i32 pub_height = CARD_HEIGHT + 6 * margin;
	Rectangle pub_rect = {
		.x = (width - pub_width) / 2.0,
		.y = (height - pub_height) / 2.0,
		.width = pub_width,
		.height = pub_height
	};

	DrawRectangleLinesEx(pub_rect, thick, trans_brown);

	for (usize i = 0; i < 5; ++i) {
		DrawRectangle(
			pub_rect.x + 2 * margin + i * (CARD_WIDTH + 3 * margin),
			pub_rect.y + 2 * margin,
			CARD_WIDTH + 2 * margin,
			CARD_HEIGHT + 2 * margin,
			trans_gray
		);
	}
}

void draw_pub_cards(const ResManager *manager, const PubCards *cards) {
	assert(cards->len <= 5);

	i32 width = GetScreenWidth();
	i32 height = GetScreenHeight();
	i32 margin = 5;
	i32 pub_width = margin * 8 + (CARD_WIDTH + 2 * margin) * 5;
	i32 pub_height = CARD_HEIGHT + 6 * margin;
	Vector2 pub_pos = {
		.x = (width - pub_width) / 2.0,
		.y = (height - pub_height) / 2.0,
	};

	for (usize i = 0; i < cards->len; ++i) {
		draw_card(
			manager,
			cards->cards[i],
			(Vector2){pub_pos.x + 3 * margin + i * (CARD_WIDTH + 3 * margin),
								pub_pos.y + 3 * margin}
		);
	}
}

void draw_hand(const ResManager *manager, const Hand *hand) {
	assert(hand->len == 2);

	i32 width = GetScreenWidth();
	i32 height = GetScreenHeight();
	i32 margin = 5;
	i32 hand_width = margin + CARD_WIDTH * 2;
	i32 x = (width - hand_width) / 2;
	i32 y = height - 10 * margin - CARD_HEIGHT;

	for (usize i = 0; i < hand->len; ++i) {
		draw_card(manager, hand->cards[i], (Vector2){x + i * (margin + CARD_WIDTH), y});
	}
}

void draw(const Game *game) {
	ResManager m = game->manager;

	draw_table();
	draw_pub_cards(&m, &game->pub_cards);
	draw_hand(&m, &game->hand);
}
