#include "GuiWidget.h"
#include "GuiContainer.h"

namespace Gui {

Widget::Widget(Context *context) : m_context(context), m_container(0), m_position(0), m_size(0), m_style(0)
{
	assert(m_context);
}

Widget::~Widget()
{
	// a container owns its widgets. this ensures that only the container can
	// delete the widget by requiring it to clear the widgets references
	// before deletion
	assert(!m_container);
}

vector2f Widget::GetAbsolutePosition() const
{
	if (!m_container) return 0;
	return m_container->GetAbsolutePosition() + m_position;
}

void Widget::Attach(Container *container)
{
	assert(m_context == container->GetContext());
	assert(container);
	m_container = container;
}

void Widget::Detach()
{
	m_container = 0;
	m_position = 0;
	m_size = 0;
}

void Widget::SetDimensions(const vector2f &position, const vector2f &size)
{
	m_position = position;
	m_size = size;
}

}
