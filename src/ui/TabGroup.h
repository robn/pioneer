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

	TabGroup *SetHeaderFont(Font font);

	class Tab: public Single {
	protected:
		friend class TabGroup;
		Tab(Context *context, const std::string &title);
		void SetHeaderPosition(const Point &pos) { m_pos = pos; }
		void SetHeaderSize(const Point &size) { m_size = size; }
		Point GetHeaderPosition() const { return m_pos; }
		Point GetHeaderSize() const { return m_size; }
		void SetHeaderFont(Font font) { m_label->SetFont(font); }

	public:
		UI::Label *GetLabel() const { return m_label.Get(); }

	private:
		RefCountedPtr<UI::Label> m_label;
		Point m_pos;
		Point m_size;
	};

	Tab *NewTab(const std::string &title);
	void RemoveTab(Tab *tab);

	void SelectTab(Tab *tab);

protected:
	virtual void HandleClick();
	virtual void HandleMouseMove(const MouseMotionEvent &event);
	virtual void HandleMouseOut();

private:
	Tab *GetTabAt(const Point &pos);

	std::list<RefCountedPtr<Tab>> m_tabs;
	Tab *m_selected;
	Tab *m_hover;
	Point m_tabPos;
	Point m_tabSize;
	Point m_paddingPos;
	Point m_paddingSize;
	Font m_headerFont;
};

}

#endif
