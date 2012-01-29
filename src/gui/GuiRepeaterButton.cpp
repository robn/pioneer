#include "Gui.h"
#include "GuiContext.h"
#include "GuiRepeaterButton.h"

namespace Gui {

RepeaterButton::RepeaterButton(int msDelay, int msRepeat)
{
	m_delay = msDelay;
	m_repeat = msRepeat;

	onPress.connect(sigc::mem_fun(this, &RepeaterButton::OnPress));
	onRelease.connect(sigc::mem_fun(this, &RepeaterButton::OnRelease));
}

RepeaterButton::~RepeaterButton()
{
	m_repeatCon.disconnect();
}

void RepeaterButton::OnPress()
{
	m_repeatCon = GetContext()->AddTimer(m_delay, sigc::mem_fun(this, &RepeaterButton::OnRepeat));
}

void RepeaterButton::OnRelease()
{
	m_repeatCon.disconnect();
}

void RepeaterButton::OnRepeat()
{
	m_repeatCon = GetContext()->AddTimer(m_repeat, sigc::mem_fun(this, &RepeaterButton::OnRepeat));

	onClick.emit();
}

} /* namespace Gui */
