#ifndef _GUIBUTTON_H
#define _GUIBUTTON_H

#include "GuiSingle.h"

namespace Gui {

class Button: public Single {
public:
	virtual Metrics GetMetrics(const vector2f &hint);
	virtual void Draw();

protected:
	friend class Context;
	Button(Context *context): Single(context) {}
};

}

#endif
