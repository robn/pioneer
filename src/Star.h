// Copyright © 2008-2012 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _STAR_H
#define _STAR_H

#include "TerrainBody.h"

class Graphic;
namespace Graphics { class Renderer; }

class Star: public TerrainBody {
public:
	OBJDEF(Star, TerrainBody, STAR);
	Star(SystemBody *sbody);
	Star();
	virtual ~Star() {};

	virtual double GetClipRadius() const;
	virtual void Render(Graphics::Renderer *r, Camera *camera, GraphicCollector &collector, const vector3d &viewCoords, const matrix4x4d &viewTransform);
};

#endif /* _STAR_H */
