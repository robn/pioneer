#ifndef _GUIIMAGE_H
#define _GUIIMAGE_H

#include "GuiWidget.h"
#include "SmartPtr.h"

class UITexture;

namespace Gui {

class Image: public Widget {
public:

	enum StretchMode {
		STRETCH_PRESERVE,   // preserve ratio
		STRETCH_MAX         // stretch to entire area allocated by container
	};

	Image(Context *context, const std::string &filename, StretchMode stretchMode);

	virtual Metrics GetMetrics(const vector2f &hint);
	virtual void Update();
	virtual void Draw();

private:
	ScopedPtr<UITexture> m_texture;
	StretchMode m_stretchMode;
	vector2f m_scaledSize;
};

}

#endif
