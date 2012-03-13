#ifndef _GUITEXTLAYOUT_H
#define _GUITEXTLAYOUT_H

#include "vector2.h"
#include "RefCounted.h"
#include <string>
#include <vector>

class TextureFont;
namespace Graphics { class Renderer; }

namespace Gui {

class TextLayout {
public:
	TextLayout(const RefCountedPtr<TextureFont> &font, const std::string &text);

	vector2f ComputeSize(const vector2f &hint);

	void Draw(Graphics::Renderer *r, const vector2f &size);

private:
	struct Word {
		Word(const std::string &_text) : text(_text) {}
		std::string text;
		vector2f    pos;
	};
	std::vector<Word> m_words;

	vector2f m_lastRequested;   // the layout area we were asked to compute size for
	vector2f m_lastSize;        // and the resulting size

	RefCountedPtr<TextureFont> m_font;
};

}

#endif
