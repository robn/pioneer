#ifndef _GUITEXTLAYOUT_H
#define _GUITEXTLAYOUT_H

#include "vector2.h"
#include <string>

namespace Gui {

class TextLayout {
public:
	TextLayout(const std::string &text);

	vector2f ComputeSize(const vector2f &hint);

private:
	std::string m_text;
};

}

#endif
