#include "GuiContainer.h"

namespace Gui {

Container::~Container()
{
	for (std::list<Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end(); ++i) {
		(*i)->Detach();
		delete (*i);
	}
}

void Container::Update()
{
	for (std::list<Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end(); ++i)
		(*i)->Update();
}

void Container::Draw()
{
	// XXX set scissor region

	for (std::list<Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end(); ++i) {
		const vector2f &pos = (*i)->GetPosition();
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0);
		(*i)->Draw();
		glPopMatrix();
	}
}

void Container::LayoutChildren()
{
	for (std::list<Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end(); ++i)
		(*i)->Layout();
}

void Container::AddWidget(Widget *widget)
{
	assert(!widget->GetContainer());

	std::list<Widget*>::iterator i;
	for (i = m_widgets.begin(); i != m_widgets.end(); ++i)
		if (*i == widget) break;
	assert(i == m_widgets.end());

	widget->Attach(this);
	m_widgets.push_back(widget);
}

void Container::RemoveWidget(Widget *widget)
{
	assert(widget->GetContainer() == this);

	std::list<Widget*>::iterator i;
	for (i = m_widgets.begin(); i != m_widgets.end(); ++i)
		if (*i == widget) break;
	assert(i != m_widgets.end());

	widget->Detach();
	m_widgets.erase(i);
}

void Container::SetWidgetDimensions(Widget *widget, const vector2f &position, const vector2f &size)
{
	assert(widget->GetContainer() == this);

	widget->SetDimensions(position, size);
}

bool Container::HandleKeyDown(const KeyboardEvent &event)
{
	return Widget::HandleKeyDown(event);
}

bool Container::HandleKeyUp(const KeyboardEvent &event)
{
	return Widget::HandleKeyUp(event);
}

bool Container::HandleMouseDown(const MouseButtonEvent &event)
{
	if (!Contains(event.pos)) return false;

	MouseButtonEvent translatedEvent = MouseButtonEvent(event.action, event.button, event.pos-GetPosition());

	bool handled = false;

	for (WidgetIterator i = WidgetsBegin(); i != WidgetsEnd(); ++i)
		if ((*i)->HandleMouseDown(translatedEvent))
			handled = true;

	return handled ? true : Widget::HandleMouseDown(event);
}

bool Container::HandleMouseUp(const MouseButtonEvent &event)
{
	if (!Contains(event.pos)) return false;

	MouseButtonEvent translatedEvent = MouseButtonEvent(event.action, event.button, event.pos-GetPosition());

	bool handled = false;

	for (WidgetIterator i = WidgetsBegin(); i != WidgetsEnd(); ++i)
		if ((*i)->HandleMouseUp(translatedEvent))
			handled = true;

	return handled ? true : Widget::HandleMouseUp(event);
}

bool Container::HandleMouseMove(const MouseMotionEvent &event)
{
	if (!Contains(event.pos)) return false;

	MouseMotionEvent translatedEvent = MouseMotionEvent(event.pos-GetPosition());

	bool handled = false;

	for (WidgetIterator i = WidgetsBegin(); i != WidgetsEnd(); ++i)
		if ((*i)->HandleMouseMove(translatedEvent))
			handled = true;

	return handled ? true : Widget::HandleMouseMove(event);
}

bool Container::HandleMouseWheel(const MouseWheelEvent &event)
{
	return Widget::HandleMouseWheel(event);
}

}
