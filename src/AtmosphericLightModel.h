// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "graphics/Renderer.h"
#include "graphics/Light.h"
#include "Camera.h"
#include "Color.h"
#include <vector>

class AtmosphericLightModel {
public:
	void SetLighting(Graphics::Renderer *r, const Camera *camera, std::vector<Graphics::Light> &oldLights, Color &oldAmbient);
	void ResetLighting(Graphics::Renderer *r, const std::vector<Graphics::Light> &oldLights, const Color &oldAmbient);

private:
	void CalcLighting(double &ambient, double &direct, const Camera *camera);
};
