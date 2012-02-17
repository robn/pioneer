#ifndef _GUITEXTLAYOUT_H
#define _GUITEXTLAYOUT_H

#include "vector2.h"
#include "RefCounted.h"
#include <string>
#include <vector>

class TextureFont;

namespace Gui {

class TextLayout {
public:
	TextLayout(const RefCountedPtr<TextureFont> &font, const std::string &text);

	vector2f ComputeSize(const vector2f &hint);

private:
	struct Word {
		Word(const std::string &_text) : text(_text) {}
		std::string text;
	};
	std::vector<Word> m_words;

	RefCountedPtr<TextureFont> m_font;
};

}

#endif
