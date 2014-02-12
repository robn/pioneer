// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "TabGroup.h"
#include "Context.h"

namespace UI {

TabGroup::TabGroup(Context *context) : Container(context),
	m_selected(nullptr),
	m_hover(nullptr)
{
}

Point TabGroup::PreferredSize()
{
	return Point(SIZE_EXPAND);
}

void TabGroup::Layout()
{
	Point size(GetSize());

	const Skin &skin(GetContext()->GetSkin());
	const Skin::BorderedRectElement &elem(skin.TabHeaderNormal());
	const unsigned int minInnerSize = skin.TabHeaderMinInnerSize();

	int tabX = 0;
	m_tabPos = Point(0);
	for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i) {
		auto tab = (*i).Get();

		auto label = tab->GetLabel();
		Point labelPreferred(label->PreferredSize());

		labelPreferred.x = std::max(labelPreferred.x, int(minInnerSize));
		labelPreferred.y = std::max(labelPreferred.y, int(minInnerSize));
		m_tabPos.y = std::max(m_tabPos.y, int(std::max(labelPreferred.y + elem.paddingY*2, elem.borderHeight*2)));

		Point tabHeaderPos(tabX, 0);
		Point tabHeaderSize(labelPreferred.x + elem.paddingX*2, 0); // no Y, we don't know it yet

		SetWidgetDimensions(label, Point(tabX + elem.paddingX, elem.paddingY), labelPreferred);
		tabX += tabHeaderSize.x;

		tab->SetHeaderPosition(tabHeaderPos);
		tab->SetHeaderSize(tabHeaderSize);
	}

	m_paddingPos  = Point(tabX, 0);
	m_paddingSize = Point(size.x-m_paddingPos.x, m_tabPos.y);

	for (auto i =  m_tabs.begin(); i != m_tabs.end(); ++i) {
		auto tab = (*i).Get();
		tab->SetHeaderSize(Point(tab->GetHeaderSize().x, m_tabPos.y));
	}

	m_tabSize = Point(size.x, std::max(size.y-m_tabPos.y, 0));
	if (m_selected) {
		const Skin::BorderedRectElement &background(skin.TabBackground());

		const Point tabInnerPos(m_tabPos.x+background.paddingX, m_tabPos.y+background.paddingY);
		const Point tabInnerSize(m_tabSize.x-background.paddingX*2, m_tabSize.y-background.paddingY*2);
		SetWidgetDimensions(m_selected, tabInnerPos, tabInnerSize);
	}

	LayoutChildren();
}

void TabGroup::Draw()
{
	const Skin &skin = GetContext()->GetSkin();

	for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i) {
		auto tab = (*i).Get();
		if (tab == m_selected)
			skin.DrawTabHeaderActive(tab->GetHeaderPosition(), tab->GetHeaderSize());
		else if (tab == m_hover)
			skin.DrawTabHeaderHover(tab->GetHeaderPosition(), tab->GetHeaderSize());
		else
			skin.DrawTabHeaderNormal(tab->GetHeaderPosition(), tab->GetHeaderSize());
	}

    skin.DrawTabHeaderPadding(m_paddingPos, m_paddingSize);

	if (m_selected)
		skin.DrawTabBackground(m_tabPos, m_tabSize);

	Container::Draw();
	return;
}

TabGroup::Tab *TabGroup::NewTab(const std::string &title)
{
	Tab *tab = new Tab(GetContext(), title);
	tab->SetHeaderFont(m_headerFont);
	m_tabs.push_back(RefCountedPtr<Tab>(tab));

	Container::AddWidget(tab->GetLabel());

	if (!m_selected) {
		m_selected = tab;
		Container::AddWidget(tab);
	}

	return tab;
}

void TabGroup::RemoveTab(Tab *tab)
{
	auto i = m_tabs.begin();
	for (; i != m_tabs.end(); ++i)
		if ((*i).Get() == tab) break;
	if (i == m_tabs.end())
		return;
	m_tabs.erase(i);

	Container::RemoveWidget(tab->GetLabel());

	if (m_selected == tab) {
		Container::RemoveWidget(tab);
		if (m_tabs.empty())
			m_selected = nullptr;
		else {
			m_selected = m_tabs.begin()->Get();
			Container::AddWidget(m_selected);
		}
	}
}

void TabGroup::SelectTab(Tab *tab)
{
	if (tab == m_selected)
		return;

	if (m_selected)
		Container::RemoveWidget(m_selected);

	Container::AddWidget(tab);

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
	if (tab)
		SelectTab(tab);
}

void TabGroup::HandleMouseMove(const MouseMotionEvent &event)
{
	m_hover = GetTabAt(event.pos);
}

void TabGroup::HandleMouseOut()
{
	m_hover = GetTabAt(GetMousePos());
}

TabGroup *TabGroup::SetHeaderFont(Font font)
{
	m_headerFont = font;
	for (auto i = m_tabs.begin(); i != m_tabs.end(); ++i)
		(*i)->SetHeaderFont(font);
	return this;
}

TabGroup::Tab::Tab(Context *context, const std::string &title): Single(context),
	m_label(context->Label(title))
{
}

}
