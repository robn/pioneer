#include "TerrainBody.h"
#include "GeoSphere.h"
#include "Pi.h"
#include "WorldView.h"
#include "Frame.h"
#include "Game.h"
#include "graphics/Graphics.h"
#include "graphics/Renderer.h"

TerrainBody::TerrainBody(SystemBody *sbody) :
	Body(),
	m_sbody(0),
	m_pos(vector3d(0,0,0)),
	m_mass(0),
	m_geosphere(0)
{
	InitTerrainBody(sbody);
}

TerrainBody::TerrainBody() :
	Body(),
	m_sbody(0),
	m_pos(vector3d(0,0,0)),
	m_mass(0),
	m_geosphere(0)
{
}

TerrainBody::~TerrainBody()
{
	if (m_geosphere)
		delete m_geosphere;
}

void TerrainBody::InitTerrainBody(SystemBody *sbody)
{
	assert(!m_sbody);
	m_sbody = sbody;
	m_mass = m_sbody->GetMass();
	if (!m_geosphere)
		m_geosphere = new GeoSphere(sbody);
}

void TerrainBody::Save(Serializer::Writer &wr, Space *space)
{
	Body::Save(wr, space);
	wr.Vector3d(m_pos);
	wr.Int32(space->GetIndexForSystemBody(m_sbody));
}

void TerrainBody::Load(Serializer::Reader &rd, Space *space)
{
	Body::Load(rd, space);
	m_pos = rd.Vector3d();
	SystemBody *sbody = space->GetSystemBodyByIndex(rd.Int32());
	InitTerrainBody(sbody);
}

double TerrainBody::GetBoundingRadius() const
{
	// needs to include all terrain so culling works {in the future},
	// and size of rotating frame is correct
	return m_sbody->GetRadius() * (1.1+m_geosphere->GetMaxFeatureHeight());
}

void TerrainBody::Render(Graphics::Renderer *renderer, const Camera *camera, const vector3d &viewCoords, const matrix4x4d &viewTransform)
{
	matrix4x4d ftran = viewTransform;
	vector3d fpos = viewCoords;
	double rad = m_sbody->GetRadius();

	float znear, zfar;
	renderer->GetNearFarRange(znear, zfar);

	glPushMatrix();
	glColor3f(1,1,1);

	{
		vector3d campos = fpos;
		ftran.ClearToRotOnly();
		campos = ftran.InverseOf() * campos;
		glMultMatrixd(&ftran[0]);

		glScaled(rad, rad, rad);			// rad = real_rad / scale
		campos = campos * (1.0/rad);		// position of camera relative to planet "model"

		// translation not applied until patch render to fix jitter
		m_geosphere->Render(renderer, -campos, m_sbody->GetRadius());
		glTranslated(campos.x, campos.y, campos.z);

		SubRender(renderer, camera, campos);
	}
	glPopMatrix();
}

void TerrainBody::SetFrame(Frame *f)
{
	if (GetFrame()) {
		GetFrame()->SetPlanetGeom(0, 0);
	}
	Body::SetFrame(f);
	if (f) {
		GetFrame()->SetPlanetGeom(0, 0);
	}
}

double TerrainBody::GetTerrainHeight(const vector3d &pos_) const
{
	double radius = m_sbody->GetRadius();
	if (m_geosphere) {
		return radius * (1.0 + m_geosphere->GetHeight(pos_));
	} else {
		assert(0);
		return radius;
	}
}

bool TerrainBody::IsSuperType(SystemBody::BodySuperType t) const
{
	if (!m_sbody) return false;
	else return m_sbody->GetSuperType() == t;
}
