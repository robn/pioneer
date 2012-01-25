#include "libs.h"
#include "Gui.h"

#define BUTTON_SIZE	16

namespace Gui {
ToggleButton::ToggleButton()
{
	m_pressed = false;
	SetSize(BUTTON_SIZE, BUTTON_SIZE);
}
bool ToggleButton::OnMouseDown(GuiExtra::MouseButtonEvent *e)
{
	if (e->button == 1) {
		onPress.emit();
		m_pressed = !m_pressed;
		if (m_pressed) {
			onChange.emit(this, true);
		} else {
			onChange.emit(this, false);
		}
	}
	return false;
}

void ToggleButton::OnActivate()
{
	m_pressed = !m_pressed;
	if (m_pressed) {
		onChange.emit(this, true);
	} else {
		onChange.emit(this, false);
	}
}

void ToggleButton::GetSizeRequested(float size[2])
{
	size[0] = BUTTON_SIZE;
	size[1] = BUTTON_SIZE;
}

void ToggleButton::Draw()
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
