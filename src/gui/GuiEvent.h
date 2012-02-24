#ifndef _GUIEVENT_H
#define _GUIEVENT_H

namespace Gui {

struct Event {};

// data for various events
struct KeyboardEvent : Event {
	KeyboardEvent(SDL_keysym _keysym) : keysym(_keysym) {}
	const SDL_keysym keysym;
};

struct MouseButtonEvent : Event {
	enum ButtonType {
		BUTTON_LEFT,
		BUTTON_MIDDLE,
		BUTTON_RIGHT
	};
	MouseButtonEvent(ButtonType _button, float _x, float _y) : button(_button), x(_x), y(_y) {}
	const ButtonType button;
	const float x, y; // relative to widget
};

struct MouseMotionEvent : Event {
	MouseMotionEvent(float _x, float _y) : x(_x), y(_y) {}
	const float x, y; // relative to widget
};

struct MouseWheelEvent : Event {
	enum WheelDirection {
		WHEEL_UP,
		WHEEL_DOWN
	};
	MouseWheelEvent(WheelDirection _direction) : direction(_direction) {}
	WheelDirection direction;
};

}

#endif
