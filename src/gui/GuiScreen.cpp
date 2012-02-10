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

const Metrics &Screen::GetMetrics()
{
	static const Metrics metrics(vector2f(m_width,m_height), vector2f(m_width,m_height), vector2f(m_width,m_height));
	return metrics;
}

}
