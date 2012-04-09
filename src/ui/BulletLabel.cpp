#include "BulletLabel.h"

#include "Gwen/Controls/Button.h"

namespace UI {

GWEN_CONTROL_CONSTRUCTOR(BulletLabel)
{
	m_button = new Gwen::Controls::Button(this);
	m_button->Dock(Gwen::Pos::Left);
	m_button->SetPadding(Gwen::Padding(10));
	m_button->SetTabable(false);

	Dock(Gwen::Pos::Fill);
	SetTabable(false);

	m_button->onPress.Add(this, &LabelClickable::OnPress);
}

void BulletLabel::SizeToContents()
{
	LabelClickable::SizeToContents();
	m_button->SetWidth(Height());
}

}
