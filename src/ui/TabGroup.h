// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef UI_TABGROUP_H
#define UI_TABGROUP_H

#include "Container.h"
#include "Single.h"
#include "Label.h"

namespace UI {

class TabGroup: public Container {
protected:
	friend class Context;
	TabGroup(Context *context);

public:
	virtual Point PreferredSize();
	virtual void Layout();
	virtual void Draw();

	class Tab: public Single {
	protected:
		friend class TabGroup;
		Tab(Context *context, Widget *headerWidget);
		void SetHeaderPosition(const Point &pos) { m_pos = pos; }
		void SetHeaderSize(const Point &size) { m_size = size; }
		Point GetHeaderPosition() const { return m_pos; }
		Point GetHeaderSize() const { return m_size; }

	public:
		UI::Widget *GetHeaderWidget() const { return m_headerWidget.Get(); }

		sigc::signal<void> onSelect;

	private:
		RefCountedPtr<UI::Widget> m_headerWidget;
		Point m_pos;
		Point m_size;
	};

	TabGroup *SetTransparent(bool transparent) {
		m_transparent = transparent;
		return this;
	}

	TabGroup *SetHeaderCollapsible(bool collapsible) {
		m_collapsible = collapsible;
		return this;
	}

	Tab *NewTab(Widget *headerWidget);
	Tab *NewTab(const std::string &headerText);
	void RemoveTab(Tab *tab);

	void SelectTab(Tab *tab);
	Tab *GetSelectedTab() const { return m_selected; }

	void SetCollapsed(bool collapsed);
	void ToggleCollapsed() { SetCollapsed(!m_collapsed); }
	bool IsCollapsed() const { return m_collapsed; }

	sigc::signal<void> onCollapse;
	sigc::signal<void> onExpand;

protected:
	virtual void HandleClick();
	virtual void HandleMouseMove(const MouseMotionEvent &event);
	virtual void HandleMouseOut();

private:
	void CalcHeader();
	Tab *GetTabAt(const Point &pos);

	std::list<RefCountedPtr<Tab>> m_tabs;
	Tab *m_selected;
	Tab *m_hover;

	Point m_tabPos;
	Point m_tabSize;
	Point m_paddingPos;
	Point m_paddingSize;

    bool m_transparent;

	bool m_collapsible;
	bool m_collapsed;
	bool m_dirty;
};

}

#endif
