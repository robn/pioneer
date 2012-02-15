#include "GuiScreen.h"

namespace Gui {

Screen::Screen(Context *context, int width, int height) : Single(),
	m_context(context),
	m_width(float(width)),
	m_height(float(height))
{
	SetContext(context);
	SetSize(vector2f(m_width,m_height));
}

Metrics Screen::GetMetrics(const vector2f &hint)
{
	return Metrics(vector2f(m_width,m_height), vector2f(m_width,m_height), vector2f(m_width,m_height));
}

}
