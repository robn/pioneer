#include "GuiBox.h"

namespace Gui {

Box::Box(BoxOrientation orient) : Container(),
	m_orient(orient),
	m_countExpanded(0),
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

void Box::CalculateMetrics(const vector2f &hint)
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
		const Metrics childMetrics = (*i).metrics = (*i).widget->GetMetrics(hint);

		m_metrics.minimum[vc] += childMetrics.minimum[vc];
		m_metrics.minimum[fc] = std::max(m_metrics.minimum[fc], childMetrics.minimum[fc]);

		m_metrics.ideal[vc] += childMetrics.ideal[vc];
		m_metrics.ideal[fc] = std::max(m_metrics.ideal[fc], childMetrics.ideal[fc]);

		m_metrics.maximum[vc] += childMetrics.maximum[vc];
		m_metrics.maximum[fc] = std::max(m_metrics.maximum[fc], childMetrics.maximum[fc]);
	}

	m_needMetricsRecalc = false;
}

Metrics Box::GetMetrics(const vector2f &hint)
{
	vector2f::Component vc, fc;
	GetComponentsForOrient(m_orient == BOX_HORIZONTAL, vc, fc);

	vector2f boxHint = hint;
	boxHint[vc] = 0;

	CalculateMetrics(boxHint);
	return m_metrics;
}

void Box::Layout()
{
	const vector2f boxSize = GetSize();

	vector2f::Component vc, fc;
	GetComponentsForOrient(m_orient == BOX_HORIZONTAL, vc, fc);

	vector2f boxHint = boxSize;
	boxHint[vc] = 0;
	CalculateMetrics(boxHint);

	float sizeRemaining = boxSize[vc];

	vector2f childPos(0);

	for (std::list<Child>::iterator i = m_children.begin(); i != m_children.end(); ++i) {
		(*i).padding = 0;

		float childSize = 0;

		if (boxSize[vc] >= m_metrics.ideal[vc])
			childSize = (*i).metrics.ideal[vc];
		else
			childSize = boxSize[vc]/m_children.size();

		(*i).size[vc] = childSize;
		(*i).size[fc] = boxSize[fc];

		sizeRemaining -= childSize;

		if (m_countExpanded == 0) {
			SetWidgetDimensions((*i).widget, childPos, (*i).size);
			childPos[vc] += childSize;
		}
	}

	if (m_countExpanded > 0) {
		int candidates = m_countExpanded;

		while (candidates > 0 && sizeRemaining > 0) {
			float allocation = sizeRemaining / candidates;

			for (std::list<Child>::iterator i = m_children.begin(); i != m_children.end(); ++i) {
				if (!(*i).attrs.expand) continue;

				float amountAdded;
				if (!(*i).attrs.fill) {
					(*i).padding += allocation * 0.5;
					amountAdded = allocation;
				}
				else if ((*i).size[vc] + allocation > (*i).metrics.maximum[vc]) {
					candidates--;
					amountAdded = (*i).metrics.maximum[vc] - (*i).size[vc];
					(*i).size[vc] = (*i).metrics.maximum[vc];
				}
				else {
					(*i).size[vc] += allocation;
					amountAdded = allocation;
				}

				sizeRemaining -= amountAdded;
			}
		}

		for (std::list<Child>::iterator i = m_children.begin(); i != m_children.end(); ++i) {
			vector2f pos = childPos;
			pos[vc] += (*i).padding;

			SetWidgetDimensions((*i).widget, pos, (*i).size);

			childPos[vc] += pos[vc] + (*i).size[vc] + (*i).padding;
		}
	}

	LayoutChildren();

	m_needMetricsRecalc = true;
}

void Box::PackStart(Widget *widget, const ChildAttrs &attrs)
{
	AddWidget(widget);
	m_children.push_front(Child(widget, attrs));
	if (attrs.expand) m_countExpanded++;
}

void Box::PackEnd(Widget *widget, const ChildAttrs &attrs)
{
	AddWidget(widget);
	m_children.push_back(Child(widget, attrs));
	if (attrs.expand) m_countExpanded++;
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
