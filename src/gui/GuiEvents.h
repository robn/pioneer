#ifndef _GUIEVENTS_H
#define _GUIEVENTS_H

namespace GuiExtra {
	struct MouseButtonEvent {
		Uint8 isdown;
		Uint8 button;
		float x, y; // widget coords
		float screenX, screenY; // screen coords
	};
	struct MouseMotionEvent {
		float x, y; // widget coords
		float screenX, screenY; // screen coords
	};
}

#endif /* _GUIEVENTS_H */
