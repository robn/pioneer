// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef GAMEUI_CAMERA_H
#define GAMEUI_CAMERA_H

#include "ui/Context.h"
#include "../Camera.h"
#include <memory>

class Body;

namespace GameUI {

class Camera : public UI::Widget {
public:
	Camera(UI::Context *context);

	virtual UI::Point PreferredSize() { return UI::Point(UI::Widget::SIZE_EXPAND); }
	virtual void Update();
	virtual void Draw();

private:
	std::unique_ptr<::Camera> m_camera;
	Body *m_body;
};

}

#endif

