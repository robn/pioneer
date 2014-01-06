// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "Camera.h"
#include "graphics/Renderer.h"
#include "Pi.h"
#include "GameConfig.h"
#include "Player.h"

namespace GameUI {

Camera::Camera(UI::Context *context) : UI::Widget(context)
{
	m_body = static_cast<Body*>(Pi::player);

	auto *r = GetContext()->GetRenderer();
	float zNear, zFar;
	r->GetNearFarRange(zNear, zFar);

	const float fovY = Pi::config->Float("FOVVertical");

	m_camera.reset(new ::Camera(Graphics::GetScreenWidth(), Graphics::GetScreenHeight(), fovY, zNear, zFar));
}

void Camera::Update()
{
	m_camera->SetFrame(m_body->GetFrame());

	// interpolate between last physics tick position and current one,
	// to remove temporal aliasing
	const matrix3x3d &m = m_body->GetInterpOrient();
	m_camera->SetOrient(m);
	m_camera->SetPosition(m_body->GetInterpPosition());

	m_camera->Update();
}

void Camera::Draw()
{
	auto *r = GetContext()->GetRenderer();
	m_camera->Draw(r, m_body);
}

}
