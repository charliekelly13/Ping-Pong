struct Button_State
{
	bool is_down; 
	bool changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN, 
	BUTTON_S,
	BUTTON_W,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
	BUTTON_P,
	BUTTON_R,
	BUTTON_Q,
	BUTTON_COUNT
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};