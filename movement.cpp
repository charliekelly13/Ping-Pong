void simulate_player(float* p, float* dp, float ddp, float dt) {
	ddp -= *dp * 10.f;
	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

bool aabb_collision(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

float calculate_ai_ddp(float p, int mod, int con) {
	float ddp = (ball_p_y - p) * mod;
	if (ddp > con) ddp = con;
	if (ddp < -con) ddp = -con;
}

void ai_move(float* player_ddp, bool player1) {
	float player_p = player1 ? player_1_p : player_2_p;
	if (selected_difficulty == 0) {
		*player_ddp = calculate_ai_ddp(player_p, 30, 1000);
	}
	else if (selected_difficulty == 1) {
		*player_ddp = calculate_ai_ddp(player_p, 100, 1300);
	}
	else {
		*player_ddp = calculate_ai_ddp((player_p - (player_half_size_y / 2)), 150, 2000);
	}
}

void paddle_movement_input(Input* input, float* ddp_1, float* ddp_2) {

	if (!is_enemy_ai) {
		if (is_down(BUTTON_UP))
			*ddp_1 += 2000;
		if (is_down(BUTTON_DOWN))
			*ddp_1 -= 2000;
	}
	else {
		ai_move(&*ddp_1, false);
	}

	if (is_down(BUTTON_W))
		*ddp_2 += 2000;
	if (is_down(BUTTON_S))
		*ddp_2 -= 2000;
}

void collision_response(float player_dp, bool ball_pos_check, int mod) {
	ball_p_x = mod + player_half_size_x + ball_half_size;
	ball_dp_x *= -1;
	if (ball_pos_check) { ball_dp_x *= 1.1; }
	ball_dp_y = (ball_p_y - player_dp) * 2 + player_dp * .75f;

	PlaySound(TEXT("ball_hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
}