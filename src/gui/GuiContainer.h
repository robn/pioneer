#ifndef _GUICONTAINER_H
#define _GUICONTAINER_H

#include "GuiWidget.h"
#include <list>

// Container is the base class for all UI containers. Containers must
// provide a Layout() method that implements its layout strategy. Layout()
// will typically call GetMetrics() on its children to request their sizings,
// then call SetSize() on its children to set their size appropriately.
// Containers should then call LayoutChildren() to make its children do their
// layout.
//
// Containers don't have provide Update() or Draw(). If they do they should
// make sure that they call the baseclass methods so that child widgets will
// also receive these methods.

namespace Gui {

class Container: public Widget {

protected:
	// can't instantiate a base container directly
	Container(Context *context) : Widget(context) {}

public:
	virtual ~Container();

	virtual void Layout() = 0;
	virtual void Update();
	virtual void Draw();

protected:
	void LayoutChildren();

	void AddWidget(Widget *);
	void RemoveWidget(Widget *);
	void SetWidgetDimensions(Widget *widget, const vector2f &position, const vector2f &size);

	typedef std::list<Widget*>::const_iterator WidgetIterator;
	const WidgetIterator WidgetsBegin() const { return m_widgets.begin(); }
	const WidgetIterator WidgetsEnd() const { return m_widgets.end(); }

private:
	std::list<Widget*> m_widgets;
};

}

#endif /* _GUICONTAINER_H */

