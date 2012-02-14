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
	virtual Metrics GetMetrics();
	virtual void Layout();

	void PackStart(Widget *child);
	void PackEnd(Widget *child);
	void Remove(Widget *child);

private:
	BoxOrientation m_orient;
	float m_spacing;

	struct BoxChild {
		BoxChild(Widget *_widget) : widget(_widget) {}
		Widget  *widget;
		Metrics metrics;
	};

	std::list<BoxChild> m_children;

	void CalculateMetrics();
	Metrics m_metrics;
	bool m_needMetricsRecalc;
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

