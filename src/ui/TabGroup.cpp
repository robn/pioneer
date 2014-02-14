// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "TabGroup.h"
#include "Context.h"

namespace UI {

TabGroup::TabGroup(Context *context) : Container(context),
	m_selected(nullptr),
	m_hover(nullptr),
	m_collapsible(false),
	m_collapsed(false),
	m_dirty(true)
{
}

void TabGroup::CalcHeader()
{
	if (!m_dirty) return;

	const Skin &skin(GetContext()->GetSkin());
	const Skin::BorderedRectElement &elem(skin.TabHeaderNormal());
	const unsigned int minInnerSize = skin.TabHeaderMinInnerSize();

	int tabX = 0;
	m_tabPos = Point(0);
	for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i) {
		auto tab = (*i).Get();

		auto headerWidget = tab->GetHeaderWidget();
		Point headerWidgetPreferred(headerWidget->PreferredSize());

		headerWidgetPreferred.x = std::max(headerWidgetPreferred.x, int(minInnerSize));
		headerWidgetPreferred.y = std::max(headerWidgetPreferred.y, int(minInnerSize));
		m_tabPos.y = std::max(m_tabPos.y, int(std::max(headerWidgetPreferred.y + elem.paddingY*2, elem.borderHeight*2)));

		Point tabHeaderPos(tabX, 0);
		Point tabHeaderSize(headerWidgetPreferred.x + elem.paddingX*2, 0); // no Y, we don't know it yet
		tabX += tabHeaderSize.x;

		tab->SetHeaderPosition(tabHeaderPos);
		tab->SetHeaderSize(tabHeaderSize);
	}

	for (auto i =  m_tabs.begin(); i != m_tabs.end(); ++i) {
		auto tab = (*i).Get();
		tab->SetHeaderSize(Point(tab->GetHeaderSize().x, m_tabPos.y));
	}

	m_paddingPos  = Point(tabX, 0);

	m_dirty = false;
}

Point TabGroup::PreferredSize()
{
	CalcHeader();

	Point preferred(m_paddingPos.x, SIZE_EXPAND);

	if (m_collapsed)
		preferred.y = m_tabPos.y;

	return preferred;
}

void TabGroup::Layout()
{
	CalcHeader();

	const Skin &skin(GetContext()->GetSkin());
	const Skin::BorderedRectElement &elem(skin.TabHeaderNormal());
	const unsigned int headerBorderWidth = elem.borderWidth;
	const unsigned int headerBorderHeight = elem.borderHeight;

	for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i) {
		Tab *tab = (*i).Get();
		const Point &pos(tab->GetHeaderPosition());
		Widget *w = tab->GetHeaderWidget();
		const Point &preferred(w->PreferredSize());
		SetWidgetDimensions(w, Point(pos.x + headerBorderWidth, headerBorderHeight), preferred);
	}

	const Point &size(GetSize());

	m_paddingSize = Point(size.x-m_paddingPos.x, m_tabPos.y);

	if (!m_collapsed) {
		m_tabSize = Point(size.x, std::max(size.y-m_tabPos.y, 0));
		if (m_selected) {
			const Skin::BorderedRectElement &background(skin.TabBackground());

			const Point tabInnerPos(m_tabPos.x+background.paddingX, m_tabPos.y+background.paddingY);
			const Point tabInnerSize(m_tabSize.x-background.paddingX*2, m_tabSize.y-background.paddingY*2);
			SetWidgetDimensions(m_selected, tabInnerPos, tabInnerSize);
		}
	}

	LayoutChildren();
}

void TabGroup::Draw()
{
	const Skin &skin = GetContext()->GetSkin();

	if (m_collapsed) {
		for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i) {
			auto tab = (*i).Get();
			if (tab == m_selected)
				skin.DrawTabHeaderCollapsedActive(tab->GetHeaderPosition(), tab->GetHeaderSize());
			else if (tab == m_hover)
				skin.DrawTabHeaderCollapsedHover(tab->GetHeaderPosition(), tab->GetHeaderSize());
			else
				skin.DrawTabHeaderCollapsedNormal(tab->GetHeaderPosition(), tab->GetHeaderSize());
		}
	}
	else {
		for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i) {
			auto tab = (*i).Get();
			if (tab == m_selected)
				skin.DrawTabHeaderActive(tab->GetHeaderPosition(), tab->GetHeaderSize());
			else if (tab == m_hover)
				skin.DrawTabHeaderHover(tab->GetHeaderPosition(), tab->GetHeaderSize());
			else
				skin.DrawTabHeaderNormal(tab->GetHeaderPosition(), tab->GetHeaderSize());
		}
	}

    skin.DrawTabHeaderPadding(m_paddingPos, m_paddingSize);

	if (!m_collapsed && m_selected)
		skin.DrawTabBackground(m_tabPos, m_tabSize);

	Container::Draw();
	return;
}

TabGroup::Tab *TabGroup::NewTab(Widget *headerWidget)
{
	Tab *tab = new Tab(GetContext(), headerWidget);
	m_tabs.push_back(RefCountedPtr<Tab>(tab));

	Container::AddWidget(tab->GetHeaderWidget());

	if (!m_selected) {
		m_selected = tab;
		if (!m_collapsed)
			Container::AddWidget(tab);
	}

	m_dirty = true;

	return tab;
}

TabGroup::Tab *TabGroup::NewTab(const std::string &headerText)
{
	return NewTab(GetContext()->Label(headerText));
}

void TabGroup::RemoveTab(Tab *tab)
{
	auto i = m_tabs.begin();
	for (; i != m_tabs.end(); ++i)
		if ((*i).Get() == tab) break;
	if (i == m_tabs.end())
		return;
	m_tabs.erase(i);

	Container::RemoveWidget(tab->GetHeaderWidget());

	if (m_selected == tab) {
		if (!m_collapsed)
			Container::RemoveWidget(tab);

		if (m_tabs.empty())
			m_selected = nullptr;
		else {
			m_selected = m_tabs.begin()->Get();
			if (!m_collapsed)
				Container::AddWidget(m_selected);
		}
	}

	m_dirty = true;
}

void TabGroup::SelectTab(Tab *tab)
{
	if (tab == m_selected)
		return;

	if (!m_collapsed) {
		if (m_selected)
			Container::RemoveWidget(m_selected);

		Container::AddWidget(tab);
	}

	m_selected = tab;
}

TabGroup::Tab *TabGroup::GetTabAt(const Point &pos)
{
	for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i) {
		auto tab = (*i).Get();
		const Point hpos(tab->GetHeaderPosition());
		const Point hsize(tab->GetHeaderSize());
		if (pos.x >= hpos.x && pos.y >= hpos.y && pos.x < hpos.x+hsize.x && pos.y < hpos.y+hsize.y)
			return tab;
	}
	return nullptr;
}

void TabGroup::HandleClick()
{
	Tab *tab = GetTabAt(GetMousePos());
	if (!tab) return;

	if (tab == m_selected) {
		if (m_collapsible) ToggleCollapsed();
	}
	else {
		SelectTab(tab);
		if (m_collapsible) SetCollapsed(false);
	}
}

void TabGroup::HandleMouseMove(const MouseMotionEvent &event)
{
	m_hover = GetTabAt(event.pos);
}

void TabGroup::HandleMouseOut()
{
	m_hover = GetTabAt(GetMousePos());
}

void TabGroup::SetCollapsed(bool collapsed)
{
	if (m_collapsed == collapsed) return;

	if (collapsed)
		Container::RemoveWidget(m_selected);
	else
		Container::AddWidget(m_selected);

	GetContext()->RequestLayout();

	m_collapsed = collapsed;
	m_dirty = true;
}

TabGroup::Tab::Tab(Context *context, Widget *headerWidget): Single(context),
	m_headerWidget(headerWidget)
{
}

}
