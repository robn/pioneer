#ifndef _GUIEVENT_H
#define _GUIEVENT_H

#include "libs.h"

namespace Gui {

class Widget;

class Event {
public:
	enum Type {
		NONE,
		KEYBOARD,
		MOUSE_BUTTON,
		MOUSE_MOTION,
		MOUSE_WHEEL
	};
	const Type type;

	static Event CreateFromSDLEvent(const SDL_Event &event);
	static bool Dispatch(const Event &event, Widget *target);

protected:
	Event(Type _type) : type(_type) {}
};

class NoEvent : public Event {
public:
	NoEvent() : Event(Event::NONE) {}
};

// data for various events
class KeyboardEvent : public Event {
public:
	enum Action {
		KEY_DOWN,
		KEY_UP
	};
	KeyboardEvent(Action _action, SDL_keysym _keysym) : Event(Event::KEYBOARD), action(_action), keysym(_keysym) {}
	const Action action;
	const SDL_keysym keysym;
};

class MouseButtonEvent : public Event {
public:
	enum Action {
		BUTTON_DOWN,
		BUTTON_UP
	};
	enum ButtonType {
		BUTTON_LEFT,
		BUTTON_MIDDLE,
		BUTTON_RIGHT
	};
	MouseButtonEvent(Action _action, ButtonType _button, float _x, float _y) : Event(Event::MOUSE_BUTTON), action(_action), button(_button), x(_x), y(_y) {}
	const Action action;
	const ButtonType button;
	const float x, y; // relative to widget
};

class MouseMotionEvent : public Event {
public:
	MouseMotionEvent(float _x, float _y) : Event(Event::MOUSE_MOTION), x(_x), y(_y) {}
	const float x, y; // relative to widget
};

class MouseWheelEvent : public Event {
public:
	enum WheelDirection {
		WHEEL_UP,
		WHEEL_DOWN
	};
	MouseWheelEvent(WheelDirection _direction) : Event(Event::MOUSE_WHEEL), direction(_direction) {}
	WheelDirection direction;
};

}

#endif
