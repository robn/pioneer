#include "GuiSingle.h"

namespace Gui {

Metrics Single::GetMetrics(const vector2f &hint)
{
	if (!m_innerWidget) return Metrics();
	return m_innerWidget->GetMetrics(hint);
}

void Single::Layout()
{
	if (!m_innerWidget) return;
	SetWidgetDimensions(m_innerWidget, GetPosition(), GetSize());
	m_innerWidget->Layout();
}

Single *Single::SetInnerWidget(Widget *widget)
{
	assert(widget);
	assert(!m_innerWidget);

	AddWidget(widget);
	m_innerWidget = widget;

	return this;
}

void Single::RemoveInnerWidget()
{
	assert(m_innerWidget);

	RemoveWidget(m_innerWidget);
	m_innerWidget = 0;
}

}
