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

	struct ChildAttrs {
		ChildAttrs(bool _expand = true, bool _fill = true, float _padding = 0.0f) : expand(_expand), fill(_fill), padding(_padding) {}
		bool  expand;
		bool  fill;
		float padding;
	};
	void PackStart(Widget *child, const ChildAttrs &attrs = ChildAttrs());
	void PackEnd(Widget *child, const ChildAttrs &attrs = ChildAttrs());
	void Remove(Widget *child);

private:
	BoxOrientation m_orient;
	float m_spacing;

	struct Child {
		Child(Widget *_widget, const ChildAttrs &_attrs) : widget(_widget), attrs(_attrs) {}
		Widget           *widget;
		const ChildAttrs attrs;
		Metrics          metrics;
		vector2f         size;
		float            padding;
	};

	std::list<Child> m_children;
	int m_countExpanded;

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

