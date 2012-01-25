#include "libs.h"
#include "Gui.h"

#define BUTTON_SIZE	16

namespace Gui {
RadioButton::RadioButton(Gui::RadioGroup *g)
{
	m_pressed = false;
	SetSize(BUTTON_SIZE, BUTTON_SIZE);
	if (g) g->Add(this);
}
RadioButton::~RadioButton()
{
	
}
bool RadioButton::OnMouseDown(GuiExtra::MouseButtonEvent *e)
{
	onPress.emit();
	OnActivate();
	return false;
}
void RadioButton::OnActivate()
{
//	if (!m_pressed) onSelect.emit();
	onSelect.emit();			// needs to emit even when pressed for time accel buttons
	m_pressed = true;			// does this break anything?
}
void RadioButton::GetSizeRequested(float size[2])
{
	size[0] = BUTTON_SIZE;
	size[1] = BUTTON_SIZE;
}

void RadioButton::Draw()
{
	float size[2];
	GetSize(size);
	if (m_pressed) {
		GuiExtra::Theme::DrawIndent(size);
	} else {
		GuiExtra::Theme::DrawOutdent(size);
	}
}
}
