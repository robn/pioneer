// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef GAMEUI_CAMERA_H
#define GAMEUI_CAMERA_H

#include "ui/Context.h"

class Body;
class Camera;

namespace GameUI {

class Camera : public UI::Single {
public:
	Camera(UI::Context *context, Body *body);
	virtual ~Camera();

	virtual UI::Point PreferredSize();
	virtual void Layout();
	virtual void Update();
	virtual void Draw();

private:
	void OnBodyDeleted();

	Body *m_body;
	ScopedPtr< ::Camera> m_camera;

	sigc::connection m_onBodyDeletedConnection;
};

}

#endif
