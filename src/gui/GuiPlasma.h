#ifndef _GUIPLASMA_H
#define _GUIPLASMA_H 1

namespace Gui {

class Plasma : public Widget {
public:
	Plasma(unsigned int width, unsigned int height);
	virtual ~Plasma();

	virtual void Draw();
	virtual void GetSizeRequested(float size[2]);

private:
	unsigned int m_width, m_height;
};

}

#endif
