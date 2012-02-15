#include "GuiTextLayout.h"

namespace Gui {

TextLayout::TextLayout(const std::string &text) : m_text(text)
{
}

vector2f TextLayout::ComputeSize(const vector2f &hint)
{
	return vector2f();
}

}
