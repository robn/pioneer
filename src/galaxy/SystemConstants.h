#ifndef _SYSTEMCONSTANTS_H
#define _SYSTEMCONSTANTS_H

#include "fixed.h"
#include "SystemBody.h"

namespace SystemConstants {
	extern const float starColors[SystemBody::TYPE_LAST][3];
	extern const float starRealColors[SystemBody::TYPE_LAST][3];
	extern const double starLuminosities[SystemBody::TYPE_LAST];
	extern const float starScale[SystemBody::TYPE_LAST];
	extern const fixed starMetallicities[SystemBody::TYPE_LAST];

	struct StarTypeInfo {
		SystemBody::BodySuperType supertype;
		int mass[2]; // min,max % sol for stars, unused for planets
		int radius[2]; // min,max % sol radii for stars, % earth radii for planets
		int tempMin, tempMax;
	};
	extern const StarTypeInfo starTypeInfo[SystemBody::TYPE_LAST];
}

#endif
