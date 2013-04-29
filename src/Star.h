// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _STAR_H
#define _STAR_H

#include "TerrainBody.h"

namespace Graphics { class Renderer; }

class Star: public TerrainBody {
public:
	OBJDEF(Star, TerrainBody, STAR);
	Star(SystemBody *sbody);
	Star();
	virtual ~Star() {};

	virtual Serializer::Object Serialize() const;

	virtual void Render(Graphics::Renderer *r, const Camera *camera, const vector3d &viewCoords, const matrix4x4d &viewTransform);

protected:
/* XXX SERIALIZER
	virtual void Load(Serializer::Reader &rd, Space *space);
*/
	void InitStar();

};

#endif /* _STAR_H */
