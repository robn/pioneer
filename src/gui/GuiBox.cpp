#include "GuiBox.h"

namespace Gui {

Box::Box(BoxOrientation orient) : Container(),
	m_orient(orient),
	m_needMetricsRecalc(true)
{
}

void Box::CalculateMetrics()
{
	if (!m_needMetricsRecalc) return;

	// interrogate children and determine metrics as follows
	// - minimum: sum(min[orient]),   max(min[non-orient])
	// - ideal:   sum(ideal[orient]), max(ideal[non-orient])
	// - maximum: sum(max[orient]),   max(max[non-orient)
	
	m_metrics = Metrics(0,0,0);

	for (std::list<BoxChild>::iterator i = m_children.begin(); i != m_children.end(); ++i) {
		const Metrics childMetrics = (*i).metrics = (*i).widget->GetMetrics();

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

	m_needMetricsRecalc = false;
}

Metrics Box::GetMetrics()
{
	CalculateMetrics();
	return m_metrics;
}

void Box::Layout()
{
	CalculateMetrics();

	const vector2f boxSize = GetSize();

	switch (m_orient) {
		case BOX_HORIZONTAL: {
			vector2f childPos(0);

			for (std::list<BoxChild>::iterator i = m_children.begin(); i != m_children.end(); ++i) {
				vector2f childSize;
				if (boxSize.x >= m_metrics.ideal.x)
					childSize = vector2f((*i).metrics.ideal.x, boxSize.y);
				else
					childSize = vector2f(boxSize.x/m_children.size(), boxSize.y);

				SetWidgetDimensions((*i).widget, childPos, childSize);

				childPos.x += childSize.x;
			}
			break;
		}

		case BOX_VERTICAL: {
			vector2f childPos(0);

			for (std::list<BoxChild>::iterator i = m_children.begin(); i != m_children.end(); ++i) {
				vector2f childSize;
				if (boxSize.y >= m_metrics.ideal.y)
					childSize = vector2f(boxSize.x, (*i).metrics.ideal.y);
				else
					childSize = vector2f(boxSize.x, boxSize.y/m_children.size());

				SetWidgetDimensions((*i).widget, childPos, childSize);

				childPos.y += childSize.y;
			}
			break;
		}

		default:
			assert(0);
	}

	LayoutChildren();

	m_needMetricsRecalc = true;
}

void Box::PackStart(Widget *widget)
{
	AddWidget(widget);
	m_children.push_front(BoxChild(widget));
}

void Box::PackEnd(Widget *widget)
{
	AddWidget(widget);
	m_children.push_back(BoxChild(widget));
}

void Box::Remove(Widget *widget)
{
	RemoveWidget(widget);
	for (std::list<BoxChild>::iterator i = m_children.begin(); i != m_children.end(); ++i)
		if ((*i).widget == widget) {
			m_children.erase(i);
			return;
		}
	assert(0);
}

}
