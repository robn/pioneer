#ifndef _UI_BULLETLABEL_H
#define _UI_BULLETLABEL_H

#include "Gwen/Controls/LabelClickable.h"

namespace Gwen {
	namespace Controls {
		class Button;
	}
}

namespace UI {

class BulletLabel : public Gwen::Controls::LabelClickable {
public:
	GWEN_CONTROL(BulletLabel, LabelClickable);

	virtual void SizeToContents();

private:
	Gwen::Controls::Button *m_button;
};

}

#endif
