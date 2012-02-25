#ifndef _GUIEVENT_H
#define _GUIEVENT_H

#include "libs.h"

namespace Gui {

class Widget;

class Event {
public:
	enum Type {
		KEYBOARD,
		MOUSE_BUTTON,
		MOUSE_MOTION,
		MOUSE_WHEEL
	};
	const Type type;

	static Event CreateFromSDLEvent(const SDL_Event *event);
	static bool Dispatch(const Event &event, Widget *target);

protected:
	Event(Type _type) : type(_type) {}
};

// data for various events
class KeyboardEvent : public Event {
public:
	KeyboardEvent(SDL_keysym _keysym) : Event(Event::KEYBOARD), keysym(_keysym) {}
	const SDL_keysym keysym;
};

class MouseButtonEvent : public Event {
public:
	enum ButtonType {
		BUTTON_LEFT,
		BUTTON_MIDDLE,
		BUTTON_RIGHT
	};
	MouseButtonEvent(ButtonType _button, float _x, float _y) : Event(Event::MOUSE_BUTTON), button(_button), x(_x), y(_y) {}
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
