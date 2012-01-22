#ifndef _GUIPLASMA_H
#define _GUIPLASMA_H 1

namespace Gui {

class Plasma : public Widget {
public:
	Plasma();

	virtual void Update();
	virtual void Draw();
	virtual void GetSizeRequested(float size[2]);
};

}

#endif
