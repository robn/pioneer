#ifndef _GUIWIDGET_H
#define _GUIWIDGET_H

#include "libs.h"
#include "vector2.h"

// Widget is the base class for all UI elements. There's a couple of things it
// must implement, and a few more it might want to implement if it wants to do
// something fancy.
//
// At minimum, a widget must implement GetMetrics() and Draw().
//
// - GetMetrics() returns a Gui::Metrics object that tells the layout manager
//   how much space the widget wants/needs. It takes a single vector2 offering
//   a hint at how much space the widget might be allocated. A value of 0 in
//   one of the vector2 components means that the container has nothing useful
//   to say about that axis. Note that the container is under absolutely no
//   obligation to allocate that amount of space to the widget, and the widget
//   should not expect it. If the widget has a more concrete idea of the
//   space it needs it should use that, even if it would be larger than the
//   hint.
//
// - Draw() actually draws the widget, using regular GL calls. The GL state
//   will be set such that widget's top-left corner is at [0,0] with a scissor
//   region to prevent drawing outside of the widget's allocated space.
//
// More advanced widgets can implement Layout() and Draw() to do more advanced
// things.
//
// - Layout() will usually only be implemented by container widgets. Its job is
//   to ask its children for the wanted size, position and size them according
//   to its layout strategy, and then get them to lay out their children. See
//   GuiContainer.h for more information about implementing a container.
//
// - Update() will be called after Layout() but before Draw(). The widget may
//   get its allocated size by calling GetSize(), and can do any preparation
//   for the actual draw at this point. Update() will always be called even if
//   if the widget is disabled.
//
// The GUI mainloop runs in the following order:
//
//  - input handlers
//  - Layout() (calls GetMetrics())
//  - Update()
//  - Draw()
//
// Container widgets will call Layout()/Update()/Draw() for their children.
// Thus, the GUI is a hierarchy of widgets.
//
// Input is handled via the various signals in the Widget class. A widget must
// connect to any input signals that it is interested in. External users may
// also connect to signals so they can do things (eg register button click
// handlers).
//
// Input handlers are called before Layout(), which gives a widget an
// opportunity to modify its metrics based on input. Handlers return a bool to
// indicate if the event was "handled" or not. If a handler returns true, then
// it handled the event and no more handlers will be called for it. If it
// returns false, the next handler is called.
//
// Input handlers are called against the "leaf" widgets first. If that widget
// has no handlers for the event, or they all return false, then handlers for
// the widget's parent widget are called, and so on until either a handler
// returns true or the root widget is reached. Note that no guarantees are
// made about the order that multiple handlers attached to a single widget
// event will be called, so attaching more than one handler to an individual
// widget event is highly discouraged.

namespace Gui {

class Context;
class Container;
class Metrics;
	
class Widget {
protected:
	// can't instantiate a base widget directly
	Widget() : m_context(0), m_container(0), m_position(0), m_size(0) {}

public:
	virtual ~Widget();

	virtual Metrics GetMetrics(const vector2f &hint) = 0;
	virtual void Layout() {}
	virtual void Update() {}
	virtual void Draw() = 0;

	// gui context
	Context *GetContext() const { return m_context; }

	// enclosing container
	Container *GetContainer() const { return m_container; }

	// size allocated to widget by container
	const vector2f &GetSize() const { return m_size; }

	// position relative to container
	const vector2f &GetPosition() const { return m_position; }

	// position relative to top container
	vector2f GetAbsolutePosition() const;


protected:
	// this sigc accumulator checks the return from each event handler. if
	// the handler returns false, the accumulator returns true and the
	// next handler is called.
	//
	// declared protected so that widget subclasses can make their own
	// event signals
	struct EventHandlerResultAccumulator {
		typedef bool result_type;
		template <typename T>
		result_type operator()(T first, T last) const {
			for (; first != last; ++first)
				if (*first) return false;
			return true;
		}
	};

public:
	
	// data for various events
	struct KeyboardEvent {
		SDL_keysym keysym;
	};
	struct MouseButtonEvent {
            enum ButtonType {
                BUTTON_LEFT,
                BUTTON_MIDDLE,
                BUTTON_RIGHT
            };
            ButtonType button;
		float x, y; // relative to widget
	};
	struct MouseMotionEvent {
		float x, y; // relative to widget
	};
	struct MouseWheelEvent {
		enum WheelDirection {
			WHEEL_UP,
			WHEEL_DOWN
		};
		WheelDirection direction;
	};

	// raw key events
	sigc::signal<bool,const KeyboardEvent &>::accumulated<EventHandlerResultAccumulator> onKeyDown;
	sigc::signal<bool,const KeyboardEvent &>::accumulated<EventHandlerResultAccumulator> onKeyUp;

	// synthesised for non-control keys. repeats when key is held down
	sigc::signal<bool,const KeyboardEvent &>::accumulated<EventHandlerResultAccumulator> onKeyPress;

	// mouse button presses
	sigc::signal<bool,const MouseButtonEvent &>::accumulated<EventHandlerResultAccumulator> onMouseDown;
	sigc::signal<bool,const MouseButtonEvent &>::accumulated<EventHandlerResultAccumulator> onMouseUp;

	// mouse movement
	sigc::signal<bool,const MouseMotionEvent &>::accumulated<EventHandlerResultAccumulator> onMouseMove;

	// mouse wheel moving
	sigc::signal<bool,const MouseWheelEvent &>::accumulated<EventHandlerResultAccumulator> onMouseWheel;

	// mouse entering or exiting widget area
	sigc::signal<bool>::accumulated<EventHandlerResultAccumulator> onMouseOver;
	sigc::signal<bool>::accumulated<EventHandlerResultAccumulator> onMouseOut;

	// click - primary mouse button press/release over widget. also
	// synthesised when keyboard shortcut is used
	sigc::signal<bool>::accumulated<EventHandlerResultAccumulator> onClick;


private:

	// let container set our attributes. none of them make any sense if
	// we're not in a container
	friend class Container;

	// things for the container to call to attach, detach and position the
	// widget. it could modify our data directly but that's ugly
	void Attach(Context *context, Container *container);
	void Detach();
	void SetDimensions(const vector2f &position, const vector2f &size);

	// Screen is the top-level container and needs to set its own context
	// and size directly
	friend class Screen;
	void SetContext(Context *context) { m_context = context; }
	void SetSize(const vector2f &size) { m_size = size; }

	Context *m_context;
	Container *m_container;
	vector2f m_position;
	vector2f m_size;
};


// A widget's GetMetrics() method returns a Metrics object. It tells the
// calling container what size the widget wants to be when drawn.
class Metrics {
public:
	Metrics() : maximum(FLT_MAX) {}
	Metrics(const vector2f &_ideal) : ideal(_ideal), maximum(FLT_MAX) {}
	Metrics(const vector2f &_ideal, const vector2f &_minimum) : ideal(_ideal), minimum(_minimum), maximum(FLT_MAX) {}
	Metrics(const vector2f &_ideal, const vector2f &_minimum, const vector2f &_maximum) : ideal(_ideal), minimum(_minimum), maximum(_maximum) {}
	
	// ideal dimensions   (widget functions optimally in this space)
	vector2f ideal;

	// minimum dimensions (widget needs at least this much to function)
	vector2f minimum;;
	
	// maximum dimensions (please don't give me more than this)
	vector2f maximum;
};

}

#endif /* _GUIWIDGET_H */
