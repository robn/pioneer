#ifndef _GUIBOX_H
#define _GUIBOX_H

#include "GuiContainer.h"

namespace Gui {

class Box : public Container {
protected:
	enum BoxOrientation {
		BOX_HORIZONTAL,
		BOX_VERTICAL
	};

	Box(BoxOrientation orient);

public:
	virtual const Metrics &GetMetrics();
	virtual void Layout();

	void PackStart(Widget *child);
	void PackEnd(Widget *child);
	void Remove(Widget *child);

private:
	BoxOrientation m_orient;
	float m_spacing;

	bool m_dirty;
	Metrics m_metrics;

	std::list<Widget*> m_widgets;
};

class VBox: public Box {
public:
	VBox(): Box(BOX_VERTICAL) {}
};
	
class HBox: public Box {
public:
	HBox(): Box(BOX_HORIZONTAL) {}
};

}

#endif /* _GUIBOX_H */

