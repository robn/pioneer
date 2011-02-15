#include "PiLuaClasses.h"

EXPORT_OOLUA_FUNCTIONS_1_CONST(pi_fixed, tonumber)
EXPORT_OOLUA_FUNCTIONS_0_NON_CONST(pi_fixed)

EXPORT_OOLUA_FUNCTIONS_8_CONST(pi_vector, x, y, z, norm, dot, cross, len, print)
EXPORT_OOLUA_FUNCTIONS_0_NON_CONST(pi_vector)
