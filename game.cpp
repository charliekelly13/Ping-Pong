#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 75, ball_dp_y, ball_half_size = 1;
float player_1_score, player_2_score;
int selected_player, selected_difficulty = 0;
bool game_complete = false;


enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
	GM_PAUSED,
	GM_WON,
	GM_PLAYER_SELECT,
	GM_DIFFICULTY_SELECT,
};

int hot_button;
bool is_enemy_ai;

Gamemode current_gamemode;


void simulate_game(Input* input, float dt) {

	if (pressed(BUTTON_Q)) {
		gameRunning = false;
	}
	else if (pressed(BUTTON_R)) {
		game_restart();
	}

	if (current_gamemode != GM_PAUSED) {
		draw_rectangle(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);
		draw_arena_borders(arena_half_size_x, arena_half_size_y, 0x000000);
		draw_arena_edges(arena_half_size_x, arena_half_size_y, 0xffffff);
	}
	if (current_gamemode == GM_PAUSED) {
		game_paused(input);
	}
	else if (current_gamemode == GM_GAMEPLAY) {
		gameplay(input, dt);
	}
	else if (current_gamemode == GM_WON) {
		player_won(input);
	}
	else if (current_gamemode == GM_PLAYER_SELECT) {
		choose_player(input);
	}
	else if (current_gamemode == GM_DIFFICULTY_SELECT) {
		choose_difficulty(input);
	}
	else {
		choose_multiplayer(input);
	}
}

void gameplay(Input* input, float dt) {
	float player_1_ddp = 0.f;
	float player_2_ddp = 0.f;

	if (selected_player == 1)
		paddle_movement_input(input, &player_2_ddp, &player_1_ddp);
	else
		paddle_movement_input(input, &player_1_ddp, &player_2_ddp);

	simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
	simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

	ball_p_x += ball_dp_x * dt;
	ball_p_y += ball_dp_y * dt;

	draw_rectangle(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);

	if (aabb_collision(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y)) {
		collision_response(player_1_dp, ball_dp_x > -200, 80);
	}
	else if (aabb_collision(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y)) {
		collision_response(player_2_dp, ball_dp_x < 200, -80);
	}

	if (ball_p_y + ball_half_size > arena_half_size_y) {
		ball_p_y = arena_half_size_y - ball_half_size;
		ball_dp_y *= -1;
	}
	else if (ball_p_y - ball_half_size < -arena_half_size_y) {
		ball_p_y = -arena_half_size_y + ball_half_size;
		ball_dp_y *= -1;
	}

	if ((ball_p_x + ball_half_size > arena_half_size_x) || ball_p_x - ball_half_size < -arena_half_size_x) {
		if (ball_dp_x > 0) {
			player_1_score++;
			ball_dp_x = -75;
		}
		else {
			player_2_score++;
			ball_dp_x = 75;
		}
		if (player_2_score == 10 || player_1_score == 10) {
			current_gamemode = GM_WON;
		}
		ball_dp_y = 0;
		ball_p_x = 0;
		ball_p_y = 0;
	}

	draw_number(player_1_score, -10, 40, 1.f, 0xffffff);
	draw_number(player_2_score, 10, 40, 1.f, 0xffffff);

	draw_rectangle(80, player_1_p, player_half_size_x, player_half_size_y, 0xffffff);
	draw_rectangle(-80, player_2_p, player_half_size_x, player_half_size_y, 0xffffff);

	if (pressed(BUTTON_P)) {
		current_gamemode = GM_PAUSED;
	}
}

void game_paused(Input* input) {
	draw_rectangle(-2, 0, 1, 5, 0xffffff);
	draw_rectangle(2, 0, 1, 5, 0xffffff);
	if (pressed(BUTTON_P)) {
		current_gamemode = GM_GAMEPLAY;
	}
}

void game_restart() {
	player_1_score = 0;
	player_2_score = 0;
	player_1_p = 0;
	player_2_p = 0;
	player_1_dp = 0;
	player_1_dp = 0;
	ball_p_x = 0;
	ball_p_y = 0;
	ball_dp_y = 0;
	ball_dp_x = 100;
	hot_button = 0;
	selected_player = 1;
	current_gamemode = GM_MENU;
}

void choose_player(Input* input) {
	if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
		hot_button = !hot_button;
	}

	if (pressed(BUTTON_ENTER)) {
		if (is_enemy_ai) {
			current_gamemode = GM_DIFFICULTY_SELECT;
		}
		else {
			current_gamemode = GM_GAMEPLAY;
		}
		selected_player = hot_button ? 0 : 1;
	}

	if (hot_button == 0) {
		draw_player_select_text(0xaaaaaa, 0xffffff);
	}
	else {
		draw_player_select_text(0xffffff, 0xaaaaaa);
	}
}

void choose_difficulty(Input* input) {
	if (pressed(BUTTON_LEFT)) {
		selected_difficulty--;
	}
	else if (pressed(BUTTON_RIGHT)) {
		selected_difficulty++;
	}

	if (pressed(BUTTON_ENTER)) {
		current_gamemode = GM_GAMEPLAY;
	}

	draw_text("CHOOSE AI DIFFICULTY ", -60, 20, 1, 0xaaaaaa);

	if (selected_difficulty == 0 || selected_difficulty == 3) {
		draw_difficulty_select_text(0xffffff, 0xaaaaaa, 0xaaaaaa);
		selected_difficulty = 0;
	}
	else if (selected_difficulty == 1) {
		draw_difficulty_select_text(0xaaaaaa, 0xffffff, 0xaaaaaa);
	}
	else {
		draw_difficulty_select_text(0xaaaaaa, 0xaaaaaa, 0xffffff);
		selected_difficulty = 2;
	}
}

void player_won(Input* input) {
	if (!game_complete) {
		PlaySound(TEXT("victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
		game_complete = true;
	}
	if (player_1_score > player_2_score) {
		draw_player_victory_text(1);
	}
	else {
		draw_player_victory_text(2);
	}
}

void choose_multiplayer(Input* input) {
	if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
		hot_button = !hot_button;
	}

	if (pressed(BUTTON_ENTER)) {
		current_gamemode = GM_PLAYER_SELECT;
		is_enemy_ai = hot_button ? 0 : 1;
		hot_button = 1;
	}

	if (hot_button == 0) {
		draw_text("SINGLE PLAYER", -80, -10, 1, 0xffffff);
		draw_text("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
	}
	else {
		draw_text("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
		draw_text("MULTIPLAYER", 20, -10, 1, 0xffffff);
	}
}