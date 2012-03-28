#ifndef _LUA_MODEL_COMPILER_H
#define _LUA_MODEL_COMPILER_H

#include <map>
#include <vector>
#include <sigc++/sigc++.h>
#include "MyLuaMathTypes.h"

// LMR = Lua Model Renderer
class LmrCollMesh;
class GeomTree;

namespace LMR { class GeomBuffer; }

namespace Graphics {
	class Renderer;
	class Material;
}

class EquipSet;

#define LMR_MAX_LOD 4

struct LmrMaterial {
	float diffuse[4];
	float specular[4];
	float emissive[4];
	float shininess;
	// make sure save and load routines in ShipFlavour are matching
};

struct LmrLight {
	float position[4];
	float color[4];
	float quadraticAttenuation;
};

struct LmrObjParams
{
	enum { LMR_ANIMATION_MAX = 10 };

	const char *animationNamespace; // the namespace to look up animation names in, from LuaConstants

	double time;
	int animStages[LMR_ANIMATION_MAX];
	double animValues[LMR_ANIMATION_MAX];
	const char *label;
	const EquipSet *equipment; // for ships
	int flightState;

	float linthrust[3];		// 1.0 to -1.0
	float angthrust[3];		// 1.0 to -1.0

	struct LmrMaterial pMat[3];
};

namespace LMR { struct RenderState; }
class LmrCollMesh;

class LmrModel {
public:
	LmrModel(lua_State *lua, const char *model_name, Graphics::Renderer *renderer);
	virtual ~LmrModel();
	lua_State *GetLua() const { return m_lua; }
	void Render(const matrix4x4f &trans, const LmrObjParams *params);
	void Render(const LMR::RenderState *rstate, const vector3f &cameraPos, const matrix4x4f &trans, const LmrObjParams *params);
	void GetCollMeshGeometry(LmrCollMesh *mesh, const matrix4x4f &transform, const LmrObjParams *params);
	float GetDrawClipRadius() const { return m_drawClipRadius; }
	float GetFloatAttribute(const char *attr_name) const;
	int GetIntAttribute(const char *attr_name) const;
	bool GetBoolAttribute(const char *attr_name) const;
	void PushAttributeToLuaStack(const char *attr_name) const;
	const char *GetName() const { return m_name.c_str(); }
	bool HasTag(const char *tag) const;

	void InitMaterial(const std::string &name);
	RefCountedPtr<Graphics::Material> AllocMaterial(const std::string &name);
	void SetMaterial(const std::string &name, const Graphics::Material &mat);

private:
	void Build(int lod, const LmrObjParams *params);

	lua_State *m_lua;
	Graphics::Renderer *m_renderer;

	typedef std::vector< RefCountedPtr<Graphics::Material> > MaterialListType;
	typedef std::map<std::string,MaterialListType> MaterialMapType;
	MaterialMapType m_materials;

	std::vector<LmrLight> m_lights;
	float m_lodPixelSize[LMR_MAX_LOD];
	int m_numLods;
	LMR::GeomBuffer *m_staticGeometry[LMR_MAX_LOD];
	LMR::GeomBuffer *m_dynamicGeometry[LMR_MAX_LOD];
	std::string m_name;
	bool m_hasDynamicFunc;
	// only used for lod pixel size at the moment
	float m_drawClipRadius;
	float m_scale;
	friend class LMR::GeomBuffer;
};

struct LmrModelNotFoundException {};
void LmrModelRender(LmrModel *m, const matrix4x4f &transform);
void LmrNotifyScreenWidth(float width);
lua_State *LmrGetLuaState();

class LmrCollMesh
{
public:
	LmrCollMesh(LmrModel *m, const LmrObjParams *params);
	~LmrCollMesh();

	const Aabb &GetAabb() const { return m_aabb; }
	float GetBoundingRadius() const { return m_radius; }
	int GetTrisWithGeomflag(unsigned int flags, int num, vector3d *outVtx) const;
	GeomTree *geomTree;
	// num vertices, num indices, num flags
	int nv, ni, nf;
	float *pVertex;
	int *pIndex;
	int m_numTris; // ni/3
	unsigned int *pFlag; // 1 per tri
	friend class LmrModel;
private:
	Aabb m_aabb;
	float m_radius;
	friend class LMR::GeomBuffer;
};


#endif /* _LUA_MODEL_COMPILER_H */
