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

	for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i)
		SetWidgetDimensions(*i, Point(0), size);

	LayoutChildren();
}

void TabGroup::Draw()
{
	if (m_selected)
		m_selected->Draw();
}

TabGroup::Tab *TabGroup::NewTab(const std::string &title)
{
	Tab *tab = new Tab(GetContext(), title);
	Container::AddWidget(tab);
	m_tabs.push_back(tab);

	if (!m_selected)
		m_selected = tab;

	return tab;
}

void TabGroup::RemoveTab(Tab *tab)
{
	Container::RemoveWidget(tab);
	m_tabs.remove(tab);

	if (m_selected == tab)
		m_selected = m_tabs.empty() ? nullptr : *(m_tabs.begin());
}

}
