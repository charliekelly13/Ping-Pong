void render_background() {
    u32* currentPixel = (u32*)render_state.memory;
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *currentPixel++ = y * x;
        }
    }
}

void clear_screen(u32 colour) {
    u32* currentPixel = (u32*)render_state.memory;
    for (int y = 0; y < render_state.height; y++) {
        for (int x = 0; x < render_state.width; x++) {
            *currentPixel++ = colour;
        }
    }
}

void draw_rectangle_in_pixels(int x0, int y0, int x1, int y1, u32 colour) {
    x0 = clamp(0, x0, render_state.width);
    x1 = clamp(0, x1, render_state.width);
    y0 = clamp(0, y0, render_state.height);
    y1 = clamp(0, y1, render_state.height);

    for (int y = y0; y < y1; y++) {
        u32* currentPixel = (u32*)render_state.memory + x0 + y * render_state.width;
        for (int x = x0; x < x1; x++) {
            *currentPixel++ = colour;
        }
    }
}

void draw_rectangle(float x, float y, float half_size_x, float half_size_y, u32 colour) {
    x *= render_state.height*0.01f;
    y *= render_state.height * 0.01f;
    half_size_x *= render_state.height * 0.01f;
    half_size_y *= render_state.height * 0.01f;

    x += render_state.width / 2.f;
    y += render_state.height / 2.f; 

    int x0 = x - half_size_x;
    int x1 = x + half_size_x;
    int y0 = y - half_size_y;
    int y1 = y + half_size_y;


    draw_rectangle_in_pixels(x0, y0, x1, y1, colour);
}

float render_scale = 0.01f;

void draw_arena_borders(float arena_x, float arena_y, u32 color) {
	arena_x *= render_state.height * render_scale;
	arena_y *= render_state.height * render_scale;

	int x0 = (int)((float)render_state.width * .5f - arena_x);
	int x1 = (int)((float)render_state.width * .5f + arena_x);
	int y0 = (int)((float)render_state.height * .5f - arena_y);
	int y1 = (int)((float)render_state.height * .5f + arena_y);

	draw_rectangle_in_pixels(0, 0, render_state.width, y0, color);
	draw_rectangle_in_pixels(0, y1, x1, render_state.height, color);
	draw_rectangle_in_pixels(0, y0, x0, y1, color);
	draw_rectangle_in_pixels(x1, y0, render_state.width, render_state.height, color);
}

void draw_arena_edges(float arena_x, float arena_y, u32 color) {
	arena_x *= render_state.height * render_scale;
	arena_y *= render_state.height * render_scale;

	int x0 = (int)((float)render_state.width * .5f - arena_x);
	int x1 = (int)((float)render_state.width * .5f + arena_x);
	int y0 = (int)((float)render_state.height * .5f - arena_y);
	int y1 = (int)((float)render_state.height * .5f + arena_y);

	draw_rectangle_in_pixels(x0-1, y0-1, x0, y1+1, color);
	draw_rectangle_in_pixels(x1, y0-1, x1+1, y1+1, color);
	draw_rectangle_in_pixels(x0-1, y1, x1+1, y1+1, color);
	draw_rectangle_in_pixels(x0-1, y0-1, x1+1, y0, color);

}

void draw_text(const char* text_to_draw) {

}

const char* letters[][7] = {
	" 00",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

void draw_text(const char* text, float x, float y, float size, u32 color) {
	float half_size = size * .5f;
	float original_y = y;

	while (*text) {
		if (*text != 32) {
			const char** letter;
			if (*text == 47) letter = letters[27];
			else if (*text == 46) letter = letters[26];
			else letter = letters[*text - 'A'];
			float original_x = x;

			for (int i = 0; i < 7; i++) {
				const char* row = letter[i];
				while (*row) {
					if (*row == '0') {
						draw_rectangle(x, y, half_size, half_size, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		text++;
		x += size * 6.f;
		y = original_y;
	}
}

void draw_number(int number, float x, float y, float size, u32 color) {
	float half_size = size * .5f;

	bool drew_number = false;
	while (number || !drew_number) {
		drew_number = true;

		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			draw_rectangle(x - size, y, half_size, 2.5f * size, color);
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			draw_rectangle(x, y + size * 2.f, half_size, half_size, color);
			draw_rectangle(x, y - size * 2.f, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 1: {
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			x -= size * 2.f;
		} break;

		case 2: {
			draw_rectangle(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rectangle(x, y, 1.5f * size, half_size, color);
			draw_rectangle(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rectangle(x + size, y + size, half_size, half_size, color);
			draw_rectangle(x - size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 3: {
			draw_rectangle(x - half_size, y + size * 2.f, size, half_size, color);
			draw_rectangle(x - half_size, y, size, half_size, color);
			draw_rectangle(x - half_size, y - size * 2.f, size, half_size, color);
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			x -= size * 4.f;
		} break;

		case 4: {
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			draw_rectangle(x - size, y + size, half_size, 1.5f * size, color);
			draw_rectangle(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 5: {
			draw_rectangle(x, y + size * 2.f, 1.5f * size, half_size, color);
			draw_rectangle(x, y, 1.5f * size, half_size, color);
			draw_rectangle(x, y - size * 2.f, 1.5f * size, half_size, color);
			draw_rectangle(x - size, y + size, half_size, half_size, color);
			draw_rectangle(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 6: {
			draw_rectangle(x + half_size, y + size * 2.f, size, half_size, color);
			draw_rectangle(x + half_size, y, size, half_size, color);
			draw_rectangle(x + half_size, y - size * 2.f, size, half_size, color);
			draw_rectangle(x - size, y, half_size, 2.5f * size, color);
			draw_rectangle(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 7: {
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			draw_rectangle(x - half_size, y + size * 2.f, size, half_size, color);
			x -= size * 4.f;
		} break;

		case 8: {
			draw_rectangle(x - size, y, half_size, 2.5f * size, color);
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			draw_rectangle(x, y + size * 2.f, half_size, half_size, color);
			draw_rectangle(x, y - size * 2.f, half_size, half_size, color);
			draw_rectangle(x, y, half_size, half_size, color);
			x -= size * 4.f;
		} break;

		case 9: {
			draw_rectangle(x - half_size, y + size * 2.f, size, half_size, color);
			draw_rectangle(x - half_size, y, size, half_size, color);
			draw_rectangle(x - half_size, y - size * 2.f, size, half_size, color);
			draw_rectangle(x + size, y, half_size, 2.5f * size, color);
			draw_rectangle(x - size, y + size, half_size, half_size, color);
			x -= size * 4.f;
		} break;
		}

	}
}

void draw_player_select_text(u32 color1, u32 color2) {
	draw_text("PLAYER ", -60, 0, 1, color1);
	draw_number(1, -20, -2.5, 1.5f, color1);

	draw_text("PLAYER ", 20, 0, 1, color2);
	draw_number(2, 65, -2.5, 1.5f, color2);
}

void draw_difficulty_select_text(u32 colour1, u32 colour2, u32 colour3) {
	draw_number(1, -20, -2.5, 1.5f, colour1);

	draw_number(2, 0, -2.5, 1.5f, colour2);

	draw_number(3, 20, -2.5, 1.5f, colour3);
}

void draw_player_victory_text(int player_num) {
	draw_text("PLAYER ", -50, 0, 1, 0xffffff);
	draw_number(player_num, -10, -2.5, 1.5f, 0xffffff);
	draw_text("VICTORY ", 0, 0, 1, 0xffffff);
}