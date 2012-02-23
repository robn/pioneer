#ifndef _GUIBACKGROUND_H
#define _GUIBACKGROUND_H

#include "GuiSingle.h"
#include "Color.h"

namespace Gui {

class Background : public Single {
public:
	virtual void Draw();

protected:
	friend class Context;
	Background(Context *context, const Color &color) : Single(context), m_color(color) {}

private:
	Color m_color;
};

}

#endif
