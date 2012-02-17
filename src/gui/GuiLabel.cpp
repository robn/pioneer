#include "GuiLabel.h"
#include "GuiContext.h"
#include "GuiTextLayout.h"

namespace Gui {

Label::Label(const std::string &text) : Widget(), m_text(text)
{
}

Metrics Label::GetMetrics(const vector2f &hint)
{
	if (!m_layout) m_layout.Reset(new TextLayout(GetContext()->GetFont(), m_text));

	vector2f want(m_layout->ComputeSize(hint));
	printf("label wants size %f,%f\n", want.x, want.y);
	return Metrics(want, want, want);
}

void Label::Update()
{
	assert(0);
}

void Label::Draw()
{
	assert(0);
}

}
