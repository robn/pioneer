// Copyright © 2008-2015 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "UIView.h"
#include "Pi.h"
#include "ui/Context.h"
#include "gameui/Panel.h"

void UIView::OnSwitchTo()
{
	Pi::ui->DropAllLayers();

	UI::VBox *box = Pi::ui->VBox();
	UI::Expand *expander = Pi::ui->Expand();

	if (m_templateName) {
		UI::Widget *view = Pi::ui->CallTemplate(m_templateName);
		if (view)
			expander->SetInnerWidget(view);
	}

	box->PackEnd(expander);
	box->PackEnd(new GameUI::Panel(Pi::ui.Get()));

	UI::Layer *layer = Pi::ui->NewLayer();
	layer->SetInnerWidget(box);

	Pi::ui->Layout();
}

void UIView::OnSwitchFrom()
{
	Pi::ui->DropAllLayers();
	Pi::ui->Layout(); // UI does important things on layout, like updating keyboard shortcuts
}
