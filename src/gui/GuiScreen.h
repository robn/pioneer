#ifndef _GUISCREEN_H
#define _GUISCREEN_H

#include "GuiSingle.h"

// Screen is the top-level container. It has the simplest layout manager
// possible - it will only accept a single container widget and will override
// its metrics to force it to be the full size of the screen.
//
// Its slightly different to other containers internally to allow it to be a
// "live" widget but without a container (because its the top-level container)

namespace Gui {

class Context;
class Metrics;

class Screen : public Single {
public:
	Screen(Context *context, int width, int height);

	virtual Metrics GetMetrics(const vector2f &hint);

private:
	Context *m_context;
	float m_width;
	float m_height;
};

}

#endif /* _GUISCREEN_H */
