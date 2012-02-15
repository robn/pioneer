#include "GuiTextLayout.h"

namespace Gui {

TextLayout::TextLayout(const std::string &text)
{
	size_t start = 0, end = 0;

	while (end != std::string::npos) {
		start = text.find_first_not_of(' ', end);
		end = text.find_first_of(' ', start);

		std::string word = text.substr(start, (end == std::string::npos) ? std::string::npos : end - start);
		m_words.push_back(Word(word));
	}
}

vector2f TextLayout::ComputeSize(const vector2f &hint)
{
	return vector2f();
}

}
