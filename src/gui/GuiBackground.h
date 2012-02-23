#ifndef _GUIBACKGROUND_H
#define _GUIBACKGROUND_H

#include "GuiSingle.h"
#include "Color.h"

namespace Gui {

class Background : public Single {
public:
	Background(Context *context, const Color &color) : Single(context), m_color(color) {}

	virtual void Draw();

private:
	Color m_color;
};

}

#endif
