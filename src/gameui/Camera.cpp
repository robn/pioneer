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
	auto *r = GetContext()->GetRenderer();
	float zNear, zFar;
	r->GetNearFarRange(zNear, zFar);

	const float fovY = Pi::config->Float("FOVVertical");

	m_camera.reset(new ::Camera(Graphics::GetScreenWidth(), Graphics::GetScreenHeight(), fovY, zNear, zFar));

	m_camera->SetFrame(Pi::player->GetFrame());
}

void Camera::Update()
{
	m_camera->Update();
}

void Camera::Draw()
{
	auto *r = GetContext()->GetRenderer();
	m_camera->Draw(r);
}

}
