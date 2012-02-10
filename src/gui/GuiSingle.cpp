#include "GuiSingle.h"

namespace Gui {

const Metrics &Single::GetMetrics()
{
	static const Metrics dontcare;
	if (!m_innerContainer) return dontcare;
	return m_innerContainer->GetMetrics();
}

void Single::Layout()
{
	if (!m_innerContainer) return;
	SetWidgetDimensions(m_innerContainer, GetPosition(), GetSize());
	m_innerContainer->Layout();
}

void Single::SetInnerContainer(Container *container)
{
	assert(container);
	assert(!m_innerContainer);

	AddWidget(container);
	m_innerContainer = container;
}

void Single::RemoveInnerContainer()
{
	assert(m_innerContainer);

	RemoveWidget(m_innerContainer);
	m_innerContainer = 0;
}

}
