#include "FontConfig.h"

FontConfig::FontConfig(const std::string &filename) : IniConfig(filename)
{
	// set defaults
	(*this)["AdvanceXAdjustment"] = "0";

	Load();
}
