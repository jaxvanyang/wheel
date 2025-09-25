#include <assert.h>

#include "poker.h"

const f32 CARD_SCALE = 2;
// scaled by CARD_SCALE
const u8 CARD_WIDTH = 96;
const u8 CARD_HEIGHT = 128;
const u8 MINICARD_WIDTH = 30;
const u8 MINICARD_HEIGHT = 44;
const u8 CHIP_WIDTH = 32;
const u8 CHIP_HEIGHT = 44;

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

Hand new_empty_hand() {
	return (Hand){
		.mask = {false, false},
	};
}

void deal_hand(Deck *deck, Hand *hand) {
	assert(deck->len >= 2);
	hand->cards[0] = deck->cards[--deck->len];
	hand->cards[1] = deck->cards[--deck->len];
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
		if (!(cards[i].rank == ACE && cards[i + 1].rank == KING) &&
				cards[i].rank - 1 != cards[i + 1].rank) {
			is_straight = false;
		}
	}

	if (cards[0].rank == ACE && cards[1].rank == FIVE) {
		is_straight = true;
		for (usize i = 1; i < 4; ++i) {
			if (cards[i].rank - 1 != cards[i + 1].rank) {
				is_straight = false;
				break;
			}
		}

		// A 5 4 3 2 -> 5 4 3 2 A
		if (is_straight) {
			for (usize i = 0; i < 4; ++i) {
				ret.cards[i] = cards[i + 1];
			}
			ret.cards[0] = cards[0];
		}
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
	// this must be initialized after sorted
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
		error("expected a valid kind");
	}
}

void print_selection(const Selection *selection) {
	printf("%s:", kind_display(selection->kind));
	for (usize i = 0; i < 5; ++i) {
		char *card = card_debug(selection->cards[i]);
		printf(" %s", card);
		FREE(card);
	}
	putchar('\n');
}

ResManager new_res_manager() {
	ResManager manager = {
		.cards = load_texture("assets/poker/poker_cards.png"),
		.minicards = load_texture("assets/poker/minicards.png"),
		.chips = load_texture("assets/poker/fiches_addon.png"),
		.button = load_texture("assets/poker/button.png"),
	};

	return manager;
}

Player new_player(const char *name, usize chips, bool is_valid) {
	return (Player){
		.name = name,
		.chips = chips,
		.hand = new_empty_hand(),
		.is_valid = is_valid,
	};
}

Game new_game() {
	Game game = {
		.manager = new_res_manager(),
		.pub_cards =
			{
				.len = 5,
			},
		.my_seat = 0,
		.dealer = SEAT_CNT - 3, // make myself after BB
	};

	// FIXME: this is for testing
	SetRandomSeed(0);

	for (usize i = 0; i < SEAT_CNT; ++i) {
		char *name = malloc(sizeof(char) * 32);
		sprintf(name, "P%d", (int)i + 1);
		game.players[i] = new_player(name, 100, true);
	}

	refresh(&game);

	return game;
}

void refresh(Game *game) {
	Deck deck = new_deck();

	for (usize i = 0; i < 5; ++i) {
		game->pub_cards.cards[i] = deck.cards[--deck.len];
	}

	for (usize i = 0; i < SEAT_CNT; ++i) {
		deal_hand(&deck, &game->players[i].hand);

		if (i == game->my_seat) {
			for (usize j = 0; j < 2; ++j) {
				game->players[i].hand.mask[j] = true;
			}
		}
	}
}

void handle_input(Game *game) {
	if (IsKeyPressed(KEY_SPACE)) {
		refresh(game);

		game->dealer = (game->dealer + 1) % SEAT_CNT;
	}

	if (IsKeyPressed(KEY_D)) {
		game->pub_cards.len = (game->pub_cards.len + 1) % 6;
	}

	if (IsKeyPressed(KEY_H)) {
		for (usize i = 0; i < SEAT_CNT; ++i) {
			if (i == game->my_seat) {
				continue;
			}

			game->players[i].hand.mask[0] = !game->players[i].hand.mask[0];
			game->players[i].hand.mask[1] = !game->players[i].hand.mask[1];
		}
	}
}

Rectangle get_player_widget(usize seat) {
	assert(seat < 5);

	Vector2 screen_size = get_screen_size();
	i32 small_margin = 5;
	i32 large_margin = small_margin * 5;
	i32 font_size = 20;
	i32 card_width = CARD_WIDTH * 2 + small_margin;
	i32 width = CHIP_WIDTH + large_margin + card_width;
	i32 height = font_size + small_margin + CARD_HEIGHT;
	Rectangle ret = {
		.width = width,
		.height = height,
	};

	switch (seat) {
	case 0:
		ret.x = screen_size.x / 2 - (CHIP_WIDTH + large_margin + (f32)card_width / 2);
		ret.y = screen_size.y - small_margin * 10 - height;
		break;
	case 1:
		ret.x = large_margin;
		ret.y = screen_size.y / 2 + large_margin * 2;
		break;
	case 2:
		ret.x = large_margin;
		ret.y = screen_size.y / 2 - large_margin * 2 - height;
		break;
	case 3:
		ret.x = screen_size.x - large_margin - width;
		ret.y = screen_size.y / 2 - large_margin * 2 - height;
		break;
	case 4:
		ret.x = screen_size.x - large_margin - width;
		ret.y = screen_size.y / 2 + large_margin * 2;
		break;
	}

	return ret;
}

void draw_card(const ResManager *manager, Card card, Vector2 pos) {
	Rectangle source = {
		.x = card.rank * 48, .y = card.suit * 64, .width = 48, .height = 64
	};
	draw_texture_rec_scale(manager->cards, source, pos, WHITE, CARD_SCALE);
}

void draw_minicard(const ResManager *manager, Card card, Vector2 pos) {
	Rectangle source = {
		.x = 17 + card.rank * 32, .y = 7 + card.suit * 32, .width = 15, .height = 22
	};
	draw_texture_rec_scale(manager->minicards, source, pos, WHITE, CARD_SCALE);
}

void draw_back(const ResManager *manager, u8 color, u8 style, Vector2 pos) {
	assert(color < 4);
	assert(style < 2);

	Rectangle source = {
		.x = (color * 2 + style) * 48, .y = 4 * 64, .width = 48, .height = 64
	};
	draw_texture_rec_scale(manager->cards, source, pos, WHITE, CARD_SCALE);
}

void draw_chip(const ResManager *manager, u8 color, u8 amount, Vector2 pos) {
	assert(color < 8);
	assert(amount < 4);

	i32 col = (color / 4) * 4 + amount;
	i32 row = color % 4;

	Rectangle source = {
		.x = col * 48, .y = 4 + row * 48, .width = CHIP_WIDTH, .height = CHIP_HEIGHT
	};
	DrawTextureRec(manager->chips, source, pos, WHITE);
}

void draw_button(const ResManager *manager, i32 x, i32 y) {
	DrawTexture(manager->button, x, y, WHITE);
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

void draw_hand(const ResManager *manager, const Hand *hand, Vector2 pos) {
	i32 margin = 5;

	for (usize i = 0; i < 2; ++i) {
		Vector2 card_pos = {pos.x + i * (margin + CARD_WIDTH), pos.y};
		if (hand->mask[i]) {
			draw_card(manager, hand->cards[i], card_pos);
		} else {
			draw_back(manager, 0, 0, card_pos);
		}
	}
}

void draw_player(
	const ResManager *manager, const Player *player, usize seat, bool card_on_left,
	bool is_dealer
) {
	Rectangle widget = get_player_widget(seat);
	i32 small_margin = 5;
	i32 large_margin = small_margin * 5;
	i32 font_size = 20;

	Vector2 chip_pos = {
		.y = widget.y + widget.height - CHIP_HEIGHT - small_margin,
	};
	Vector2 hand_pos = {
		.y = widget.y + font_size + small_margin,
	};

	if (card_on_left) {
		chip_pos.x = widget.x + CARD_WIDTH * 2 + small_margin;
		hand_pos.x = widget.x;
	} else {
		chip_pos.x = widget.x;
		hand_pos.x = widget.x + CHIP_WIDTH + large_margin;
	}

	i32 name_x = hand_pos.x + ((f32)CARD_WIDTH * 2 + small_margin) / 2;

	draw_chip(manager, 0, 3, chip_pos);
	draw_text_center(
		TextFormat("%" USIZE_FMT, player->chips),
		chip_pos.x + (f32)CHIP_WIDTH / 2,
		chip_pos.y - small_margin - font_size,
		font_size,
		RAYWHITE
	);

	if (is_dealer) {
		draw_button(manager, hand_pos.x, widget.y - (f32)font_size / 2);
	}

	draw_hand(manager, &player->hand, hand_pos);
	// FIXME: this is beyond the widget box
	draw_text_center(player->name, name_x, widget.y, font_size, RAYWHITE);
}

void draw_kind(Kind kind, i32 x, i32 y, i32 font_size) {
	const char *text = kind_display(kind);

	DrawText(text, x, y, font_size, RAYWHITE);
}

void draw_selection(const ResManager *manager, const Selection *selection) {
	Vector2 screen_size = get_screen_size();
	i32 margin = 5;
	i32 font_size = 20;
	i32 width = MINICARD_WIDTH * 5 + margin * 4;
	i32 height = font_size + margin + MINICARD_HEIGHT;
	i32 x = screen_size.x - 2 * margin - width;
	i32 y = screen_size.y - 2 * margin - height;

	draw_kind(selection->kind, x, y, font_size);
	for (usize i = 0; i < 5; ++i) {
		draw_minicard(
			manager,
			selection->cards[i],
			(Vector2){x + i * (MINICARD_WIDTH + 5), y + font_size + margin}
		);
	}
}

void draw(const Game *game) {
	ResManager m = game->manager;

	draw_table();
	draw_pub_cards(&m, &game->pub_cards);

	for (usize i = 0, seat = game->my_seat; i < 5; ++i, seat = (seat + 1) % SEAT_CNT) {
		draw_player(&m, game->players + seat, seat, false, seat == game->dealer);
	}

	if (game->pub_cards.len == 5) {
		Selection sel =
			get_best_selection(&game->pub_cards, &game->players[game->my_seat].hand);
		draw_selection(&m, &sel);
	}

	DrawFPS(5, 5);
}
