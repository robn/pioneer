#include "GuiTextLayout.h"
#include "GuiWidget.h"
#include "RefCounted.h"
#include "TextureFont.h"

namespace Gui {

TextLayout::TextLayout(const RefCountedPtr<TextureFont> &font, const std::string &text) :
	m_font(font)
{
	size_t start = 0, end = 0;

	while (end != std::string::npos) {
		start = text.find_first_not_of(' ', end);
		end = text.find_first_of(' ', start);

		std::string word = text.substr(start, (end == std::string::npos) ? std::string::npos : end - start);
		m_words.push_back(Word(word));
	}
}

// XXX using the old TextureFont methods for now. they should be moved here.
// TextureFont should only handle individual glyph functions (I think)
vector2f TextLayout::ComputeSize(const vector2f &hint)
{
	for (std::vector<Word>::iterator i = m_words.begin(); i != m_words.end(); ++i) {
		vector2f wordSize;
		m_font->MeasureString((*i).text.c_str(), wordSize.x, wordSize.y);
		printf("[%f,%f] %s\n", wordSize.x, wordSize.y, (*i).text.c_str());
	}

	return vector2f();
}

}
