#ifndef _GUIIMAGE_H
#define _GUIIMAGE_H

#include "GuiWidget.h"
#include "SmartPtr.h"

class UITexture;

namespace Gui {

class Image: public Widget {

public:
	Image(const std::string &filename);

	virtual Metrics GetMetrics();
	virtual void Draw();

private:
	ScopedPtr<UITexture> m_texture;
};

}

#endif
