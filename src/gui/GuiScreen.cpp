#include "GuiScreen.h"

namespace Gui {

Screen::Screen(Context *context, int width, int height) :
	m_context(context),
	m_width(float(width)),
	m_height(float(height)),
	m_innerContainer(0)
{
}

const Metrics &Screen::GetMetrics()
{
	static const Metrics metrics(vector2f(m_width,m_height), vector2f(m_width,m_height), vector2f(m_width,m_height));
	return metrics;
}

void Screen::Layout()
{
	if (!m_innerContainer) return;

	SetWidgetDimensions(m_innerContainer, vector2f(0,0), vector2f(m_width,m_height));
	LayoutChildren();
}

void Screen::SetInnerContainer(Container *container)
{
	assert(container);
	assert(!m_innerContainer);

	AddWidget(container);
	m_innerContainer = container;
}

void Screen::RemoveInnerContainer()
{
	assert(m_innerContainer);

	RemoveWidget(m_innerContainer);
	m_innerContainer = 0;
}

}
