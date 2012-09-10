#include "Terrain.h"

template <>
const char *TerrainHeightFractal<TerrainHeightFlat>::GetHeightFractalName() const { return "Flat"; }

template <>
TerrainHeightFractal<TerrainHeightFlat>::TerrainHeightFractal()
{
}

template <>
double TerrainHeightFractal<TerrainHeightFlat>::GetHeight(const vector3d &p)
{
	return 0.0;
}
