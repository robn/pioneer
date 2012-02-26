#include "GuiEvent.h"
#include <cstdio>
#include <typeinfo>

namespace Gui {

static inline MouseButtonEvent::ButtonType MouseButtonFromSDLButton(Uint8 sdlButton) {
	return
		sdlButton == SDL_BUTTON_LEFT   ? MouseButtonEvent::BUTTON_LEFT   :
		sdlButton == SDL_BUTTON_MIDDLE ? MouseButtonEvent::BUTTON_MIDDLE :
		                                 MouseButtonEvent::BUTTON_RIGHT;
}

Event Event::CreateFromSDLEvent(const SDL_Event &event)
{
	switch (event.type) {
		case SDL_KEYDOWN:
			return KeyboardEvent(KeyboardEvent::KEY_DOWN, event.key.keysym);

		case SDL_KEYUP:
			return KeyboardEvent(KeyboardEvent::KEY_UP, event.key.keysym);

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_WHEELUP || event.button.button == SDL_BUTTON_WHEELDOWN)
				return MouseWheelEvent(event.button.button == SDL_BUTTON_WHEELUP ? MouseWheelEvent::WHEEL_UP : MouseWheelEvent::WHEEL_DOWN);
			return MouseButtonEvent(MouseButtonEvent::BUTTON_DOWN, MouseButtonFromSDLButton(event.button.button), event.button.x, event.button.y);

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_WHEELUP || event.button.button == SDL_BUTTON_WHEELDOWN)
				return NoEvent();
			return MouseButtonEvent(MouseButtonEvent::BUTTON_UP, MouseButtonFromSDLButton(event.button.button), event.button.x, event.button.y);

		case SDL_MOUSEMOTION:
			return MouseMotionEvent(event.motion.x, event.motion.y);
			break;
	}

	return NoEvent();
}

bool Event::Dispatch(const Event &event, Widget *target)
{
	switch (event.type) {
		case Event::KEYBOARD: {
			const KeyboardEvent &keyEvent = static_cast<const KeyboardEvent&>(event);
			switch (keyEvent.action) {
				case KeyboardEvent::KEY_DOWN: return KeyDownDispatch(keyEvent, target);
				case KeyboardEvent::KEY_UP:   return KeyUpDispatch(keyEvent, target);
			}
			return false;
		}

		case Event::MOUSE_BUTTON: {
			const MouseButtonEvent &mouseButtonEvent = static_cast<const MouseButtonEvent&>(event);
			switch (mouseButtonEvent.action) {
				case MouseButtonEvent::BUTTON_DOWN: return MouseDownDispatch(mouseButtonEvent, target);
				case MouseButtonEvent::BUTTON_UP:   return MouseUpDispatch(mouseButtonEvent, target);
			}
			return false;
		}

		case Event::MOUSE_MOTION: {
			const MouseMotionEvent &mouseMotionEvent = static_cast<const MouseMotionEvent&>(event);
			return MouseMoveDispatch(mouseMotionEvent, target);
		}

		case Event::MOUSE_WHEEL: {
			const MouseWheelEvent &mouseWheelEvent = static_cast<const MouseWheelEvent&>(event);
			return MouseWheelDispatch(mouseWheelEvent, target);
		}

		default:
			return false;
	}

	return false;
}

bool Event::KeyDownDispatch(const KeyboardEvent &event, Widget *target)
{
	return false;
}

bool Event::KeyUpDispatch(const KeyboardEvent &event, Widget *target)
{
	return false;
}

bool Event::MouseDownDispatch(const MouseButtonEvent &event, Widget *target)
{
	return false;
}

bool Event::MouseUpDispatch(const MouseButtonEvent &event, Widget *target)
{
	return false;
}

bool Event::MouseMoveDispatch(const MouseMotionEvent &event, Widget *target)
{
	return false;
}

bool Event::MouseWheelDispatch(const MouseWheelEvent &event, Widget *target)
{
	return false;
}

}
