#ifndef _GUIIMAGE_H
#define _GUIIMAGE_H

#include "GuiWidget.h"
#include "SmartPtr.h"
#include "GuiTexturedQuad.h"

class UITexture;

namespace Gui {

class Image: public Widget {
public:
	enum StretchMode {
		STRETCH_PRESERVE,   // preserve ratio
		STRETCH_MAX         // stretch to entire area allocated by container
	};

	virtual Metrics GetMetrics(const vector2f &hint);
	virtual void Update();
	virtual void Draw();

protected:
	friend class Context;
	Image(Context *context, const std::string &filename, StretchMode stretchMode);

private:
	ScopedPtr<TexturedQuad> m_quad;
	StretchMode m_stretchMode;
	vector2f m_initialSize;
	vector2f m_scaledSize;
};

}

#endif
