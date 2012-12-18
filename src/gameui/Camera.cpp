// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "gameui/Camera.h"
#include "../Camera.h"
#include "Body.h"

namespace GameUI {

Camera::Camera(UI::Context *context, Body *body) : Single(context), m_body(body) {
	m_onBodyDeletedConnection = m_body->onDelete.connect(sigc::mem_fun(this, &GameUI::Camera::OnBodyDeleted));
}

Camera::~Camera()
{
	m_onBodyDeletedConnection.disconnect();
}

void Camera::OnBodyDeleted()
{
	m_body = 0;
	m_camera.Reset();
}

UI::Point Camera::PreferredSize()
{
	SetSizeControlFlags(EXPAND_WIDTH | EXPAND_HEIGHT);
	return GetInnerWidget() ? CalcLayoutContribution(GetInnerWidget()) : UI::Point();
}

void Camera::Layout()
{
	if (m_body) {
		const UI::Point size(GetActiveArea());

		float znear;
		float zfar;
		GetContext()->GetRenderer()->GetNearFarRange(znear, zfar);

		//const float fovY = Pi::config->Float("FOVVertical");

		m_camera.Reset(new ::Camera(m_body, size.x, size.y, 65.0f, znear, zfar)); // XXX bring fov through from config
        m_camera->SetPosition(vector3d(0,0,200)); // XXX positioning
	}

	Single::Layout();
}

void Camera::Update()
{
	m_camera->Update();
}

void Camera::Draw()
{
	if (!m_camera) {
		// XXX static or something
		return;
	}

	const UI::Point pos(GetAbsolutePosition() + GetActiveOffset());
	const UI::Point size(GetActiveArea());

	Graphics::Renderer *r = GetContext()->GetRenderer();

	Graphics::Renderer::StateTicket ticket(r);

	r->SetViewport(pos.x, GetContext()->GetSize().y - pos.y - size.y, size.x, size.y);

	m_camera->Draw(r);
}

}
