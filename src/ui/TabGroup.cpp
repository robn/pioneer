// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "TabGroup.h"
#include "Context.h"

namespace UI {

TabGroup::TabGroup(Context *context) : Container(context),
	m_selected(nullptr)
{
}

Point TabGroup::PreferredSize()
{
	return Point(SIZE_EXPAND);
}

void TabGroup::Layout()
{
	Point size(GetSize());

	int labelX = 0;
	Point tabPos(0);
	for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i) {
		auto tab = (*i);

		auto label = tab->GetLabel();
		Point labelPreferred(label->PreferredSize());
		tabPos.y = std::max(tabPos.y, labelPreferred.y);

		SetWidgetDimensions(label, Point(labelX, 0), labelPreferred);
		labelX += labelPreferred.x;
	}

	if (m_selected) {
		Point tabSize(size.x, std::max(size.y-tabPos.y, 0));
		SetWidgetDimensions(m_selected, tabPos, tabSize);
	}

	LayoutChildren();
}

void TabGroup::Draw()
{
	Container::Draw();
	return;

	for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i) {
		printf("%s %d,%d\n", (*i)->GetLabel()->GetText().c_str(), (*i)->GetLabel()->GetPosition().x, (*i)->GetLabel()->GetPosition().y);
		(*i)->GetLabel()->Draw();
	}

	if (m_selected)
		m_selected->Draw();
}

TabGroup::Tab *TabGroup::NewTab(const std::string &title)
{
	Tab *tab = new Tab(GetContext(), title);
	m_tabs.push_back(tab);

	Container::AddWidget(tab->GetLabel());

	if (!m_selected) {
		m_selected = tab;
		Container::AddWidget(tab);
	}

	return tab;
}

void TabGroup::RemoveTab(Tab *tab)
{
	m_tabs.remove(tab);

	Container::RemoveWidget(tab->GetLabel());

	if (m_selected == tab) {
		Container::RemoveWidget(tab);
		if (m_tabs.empty())
			m_selected = nullptr;
		else {
			m_selected = *(m_tabs.begin());
			Container::AddWidget(m_selected);
		}
	}
}

TabGroup::Tab::Tab(Context *context, const std::string &title): Single(context),
	m_label(context->Label(title))
{
}

}
