#include "LuaStar.h"
#include "LuaUtils.h"

/*
 * Class: Star
 *
 * Class representing a star. Inherits from <Body>.
 */

static bool promotion_test(DeleteEmitter *o)
{
	return dynamic_cast<Star*>(o);
}

template <> const char *LuaObject<Star>::s_type = "Star";

template <> void LuaObject<Star>::RegisterClass() {
	const char *l_parent = "Body";

	LuaObjectBase::CreateClass(s_type, l_parent, 0, 0, 0);
	LuaObjectBase::RegisterPromotion(l_parent, s_type, promotion_test);
}
