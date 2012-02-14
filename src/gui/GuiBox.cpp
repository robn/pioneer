#include "GuiBox.h"

namespace Gui {

Box::Box(BoxOrientation orient) : Container(),
	m_orient(orient),
	m_needMetricsRecalc(true)
{
}

static inline void GetComponentsForOrient(bool horiz, vector2f::Component &variableComponent, vector2f::Component &fixedComponent)
{
	if (horiz) {
		variableComponent = vector2f::X;
		fixedComponent = vector2f::Y;
	}
	else {
		variableComponent = vector2f::Y;
		fixedComponent = vector2f::X;
	}
}

void Box::CalculateMetrics()
{
	if (!m_needMetricsRecalc) return;

	// interrogate children and determine metrics as follows
	// - minimum: sum(min[orient]),   max(min[non-orient])
	// - ideal:   sum(ideal[orient]), max(ideal[non-orient])
	// - maximum: sum(max[orient]),   max(max[non-orient)
	
	vector2f::Component vc, fc;
	GetComponentsForOrient(m_orient == BOX_HORIZONTAL, vc, fc);
	
	m_metrics = Metrics(0,0,0);

	for (std::list<Child>::iterator i = m_children.begin(); i != m_children.end(); ++i) {
		const Metrics childMetrics = (*i).metrics = (*i).widget->GetMetrics();

		m_metrics.minimum[vc] += childMetrics.minimum[vc];
		m_metrics.minimum[fc] = std::max(m_metrics.minimum[fc], childMetrics.minimum[fc]);

		m_metrics.ideal[vc] += childMetrics.ideal[vc];
		m_metrics.ideal[fc] = std::max(m_metrics.ideal[fc], childMetrics.ideal[fc]);

		m_metrics.maximum[vc] += childMetrics.maximum[vc];
		m_metrics.maximum[fc] = std::max(m_metrics.maximum[fc], childMetrics.maximum[fc]);
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

	vector2f::Component vc, fc;
	GetComponentsForOrient(m_orient == BOX_HORIZONTAL, vc, fc);

	vector2f childPos(0);

	for (std::list<Child>::iterator i = m_children.begin(); i != m_children.end(); ++i) {
		vector2f childSize;
		if (boxSize[vc] >= m_metrics.ideal[vc]) {
			childSize[vc] = (*i).metrics.ideal[vc];
			childSize[fc] = boxSize[fc];
		}
		else {
			childSize[vc] = boxSize[vc]/m_children.size();
			childSize[fc] = boxSize[fc];
		}

		SetWidgetDimensions((*i).widget, childPos, childSize);

		childPos[vc] += childSize[vc];
	}

	LayoutChildren();

	m_needMetricsRecalc = true;
}

void Box::PackStart(Widget *widget, const ChildAttrs &attrs)
{
	AddWidget(widget);
	m_children.push_front(Child(widget, attrs));
}

void Box::PackEnd(Widget *widget, const ChildAttrs &attrs)
{
	AddWidget(widget);
	m_children.push_back(Child(widget, attrs));
}

void Box::Remove(Widget *widget)
{
	RemoveWidget(widget);
	for (std::list<Child>::iterator i = m_children.begin(); i != m_children.end(); ++i)
		if ((*i).widget == widget) {
			m_children.erase(i);
			return;
		}
	assert(0);
}

}
