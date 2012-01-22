#ifndef _GUIPLASMA_H
#define _GUIPLASMA_H 1

#include "SmartPtr.h"

class UITexture;

namespace Gui {

class Plasma : public Widget {
public:
	Plasma(unsigned int width, unsigned int height);

	virtual void Draw();
	virtual void GetSizeRequested(float size[2]);

private:
	unsigned int m_width, m_height;
	double m_radians;
	ScopedPtr<UITexture> m_texture;
};

}

#endif
