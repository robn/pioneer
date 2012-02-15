#ifndef _GUISINGLE_H
#define _GUISINGLE_H

#include "GuiContainer.h"

namespace Gui {

class Single : public Container {
public:
	virtual Metrics GetMetrics(const vector2f &hint);
	virtual void Layout();

	void SetInnerContainer(Container *container);
	void RemoveInnerContainer();
	Container *GetInnerContainer() const { return m_innerContainer; }

protected:
	Single() : m_innerContainer(0) {}

private:
	Container *m_innerContainer;
};

}

#endif
