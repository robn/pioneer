#ifndef _GUISINGLE_H
#define _GUISINGLE_H

#include "GuiContainer.h"

namespace Gui {

class Single : public Container {
public:
	virtual Metrics GetMetrics(const vector2f &hint);
	virtual void Layout();

	void SetInnerWidget(Widget *widget);
	void RemoveInnerWidget();
	Widget *GetInnerWidget() const { return m_innerWidget; }

protected:
	Single() : m_innerWidget(0) {}

private:
	Widget *m_innerWidget;
};

}

#endif
