// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef UI_TABGROUP_H
#define UI_TABGROUP_H

#include "Container.h"
#include "Single.h"

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
		Tab(Context *context, const std::string &title): Single(context), m_title(title) {}

	public:
		const std::string &GetTitle() const { return m_title; }

	private:
		std::string m_title;
	};

	Tab *NewTab(const std::string &title);
	void RemoveTab(Tab *tab);

private:
	std::list<Tab*> m_tabs;
	Tab *m_selected;
};

}

#endif
