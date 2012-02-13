#include "GuiBox.h"

namespace Gui {

Box::Box(BoxOrientation orient) : Container(),
	m_orient(orient),
	m_dirty(false)
{
}
	
Metrics Box::GetMetrics()
{
	if (!m_dirty) return m_metrics;

	// interrogate children and determine metrics as follows
	// - minimum: sum(min[orient]),   max(min[non-orient])
	// - ideal:   sum(ideal[orient]), max(ideal[non-orient])
	// - maximum: sum(max[orient]),   max(max[non-orient)
	
	m_metrics = Metrics(0,0,0);

	for (std::list<Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end(); ++i) {
		const Metrics childMetrics = (*i)->GetMetrics();

		switch (m_orient) {
			case BOX_HORIZONTAL:
				m_metrics.minimum.x += childMetrics.minimum.x;
				m_metrics.minimum.y = std::max(m_metrics.minimum.y, childMetrics.minimum.y);

				m_metrics.ideal.x += childMetrics.ideal.x;
				m_metrics.ideal.y = std::max(m_metrics.ideal.y, childMetrics.ideal.y);

				m_metrics.maximum.x += childMetrics.maximum.x;
				m_metrics.maximum.y = std::max(m_metrics.maximum.y, childMetrics.maximum.y);

				break;

			case BOX_VERTICAL:
				m_metrics.minimum.x = std::max(m_metrics.minimum.x, childMetrics.minimum.x);
				m_metrics.minimum.y += childMetrics.minimum.y;

				m_metrics.ideal.x = std::max(m_metrics.ideal.x, childMetrics.ideal.x);
				m_metrics.ideal.y += childMetrics.ideal.y;

				m_metrics.maximum.x = std::max(m_metrics.maximum.x, childMetrics.maximum.x);
				m_metrics.maximum.y += childMetrics.maximum.y;

				break;

			default:
				assert(0);
		}
	}

	m_dirty = false;
	return m_metrics;
}

void Box::Layout()
{
	const vector2f boxSize = GetSize();

	vector2f childPos;

	switch (m_orient) {
		case BOX_HORIZONTAL: {
			vector2f childSize(boxSize.x/m_widgets.size(), boxSize.y);
			for (std::list<Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end(); ++i) {
				SetWidgetDimensions((*i), childPos, childSize);
				childPos.x += childSize.x;
			}
			break;
		}

		case BOX_VERTICAL: {
			vector2f childSize(boxSize.x, boxSize.y/m_widgets.size());
			for (std::list<Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end(); ++i) {
				SetWidgetDimensions((*i), childPos, childSize);
				childPos.y += childSize.y;
			}
			break;
		}

		default:
			assert(0);
	}

	LayoutChildren();
}

void Box::PackStart(Widget *widget)
{
	AddWidget(widget);
	m_widgets.push_front(widget);
	m_dirty = true;
}

void Box::PackEnd(Widget *widget)
{
	AddWidget(widget);
	m_widgets.push_back(widget);
	m_dirty = true;
}

void Box::Remove(Widget *widget)
{
	RemoveWidget(widget);
	for (std::list<Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end(); ++i)
		if ((*i) == widget) {
			m_widgets.erase(i);
			m_dirty = true;
			return;
		}
	assert(0);
}

}
