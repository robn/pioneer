#include "GuiLabel.h"
#include "GuiTextLayout.h"

namespace Gui {

Label::Label(const std::string &text) : m_layout(new TextLayout(text))
{
}

Metrics Label::GetMetrics(const vector2f &hint)
{
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
