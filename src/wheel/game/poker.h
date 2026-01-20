#pragma once

#include <pthread.h>
#include <raylib.h>

#include "../core.h"
#include "../net.h"
#include "../xray.h"
#include "card.h"
#include "conf.h"
#include "consts.h"

extern const f32 CARD_SCALE;
extern const u8 CARD_WIDTH;
extern const u8 CARD_HEIGHT;
extern const u8 MINICARD_WIDTH;
extern const u8 MINICARD_HEIGHT;
extern const u8 CHIP_WIDTH;
extern const u8 CHIP_HEIGHT;

typedef struct {
	Texture2D cards;
	Texture2D minicards;
	Texture2D chips;
	Texture2D button;
	Music bgm;
	Sound sound_win;
	Sound sound_lose;
} ResManager;

typedef enum {
	STATE_OFFLINE,
	STATE_LOGGED, // logged but not joined any room
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
	char name[32];
	Hand hand;
	usize chips;
	usize bet;
	usize gain;
	bool is_valid;
	PlayerState state;
} Player;

typedef struct {
	ResManager manager;
	PubCards pub_cards;
	Player players[SEAT_CNT];
	pthread_t countdown_thread;
	usize my_seat;
	usize cur; // current player's seat number
	usize dealer;
	usize bet;
	usize pot;
	usize slider; // must guarantee it is <= min(chips, 100)
	bool is_freezing;
} Game;

typedef struct {
	SockAddr server;
	Socket socket;

	Version server_version;
	TimeVal t0; // used to calculate latency
	double latency;
	// loss rate = 1 - received / sent
	float packets_received;
	float packets_sent;

	// communication thread
	pthread_t task;
	float task_timer;
	TimeVal last_refresh;

	Music bgm;

	int id; // user ID, which is only valid when > 0
	bool is_logged;
	bool is_seated;
	int room_id; // default is -1, which indicates no room selected
	int rooms[ROOM_CNT]; // player counts in rooms
} Poker;

// Return next player's seat number. Only consider players that need to take action in
// this round. Return -1 if no such players.
isize get_next_player(const Game *game, usize seat);
// Return count of players that didn't fold.
usize get_player_count(const Game *game);

ResManager new_res_manager();

// Return a static string of the player's state.
const char *format_player_state(PlayerState state);
Player new_player(usize chips, bool is_valid);
bool is_player_on_table(const Player *player);

Game new_game();
void refresh(Game *game);
void wait_for_new_game(Game *game);
void start_new_game(Game *game);

void handle_input(Game *game);

// Choose action as a bot for current player.
void bot_update(Game *game);
// Choose action based on the input for myself.
void manual_update(Game *game);
void update(Game *game);

Poker new_poker(u32 server_ip, u16 server_port, int id);
void init_poker(Poker *poker);

// MODULE: net

bool poker_send_str(Poker *poker, const char *str);
// Return NULL if failed.
Str *poker_recv_str(Poker *poker);

// MODULE: task

void *login(void *arg);
void *join(void *arg);
void *refresh_rooms(void *arg);
void poker_update(Poker *poker);

// MODULE: input

void poker_input(Poker *poker);

// MODULE: draw

// NOTE: the seat is relative to my seat
Rectangle get_player_widget(usize seat);
Rectangle get_room_rect(int id);

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
void poker_draw(const Poker *poker);
