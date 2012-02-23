#ifndef _GUILABEL_H
#define _GUILABEL_H

#include "GuiWidget.h"
#include "SmartPtr.h"

namespace Gui {

class TextLayout;

class Label: public Widget {
public:
	Label(Context *context, const std::string &text);

	virtual Metrics GetMetrics(const vector2f &hint);
	virtual void Draw();

private:
	std::string m_text;
	ScopedPtr<TextLayout> m_layout;
};

}

#endif
