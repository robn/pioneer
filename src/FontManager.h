#ifndef _FONTMANAGER_H
#define _FONTMANAGER_H

#include <map>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "vector2.h"

class TextureFont;
class VectorFont;

class FontManager {
public:
	// XXX adding scale here is a bad interface because VectorFonts don't need
	// it. it will do for now as the need for font scaling will go away once
	// we have a variable-size gui
	FontManager(const vector2f &scale);
	~FontManager();

	FT_Library GetFreeTypeLibrary() { return m_library; }

	TextureFont *GetTextureFont(const std::string &name);
	VectorFont  *GetVectorFont(const std::string &name);

	vector2f ScaleDimensions(const vector2f &dim) { return vector2f(dim.x/m_scale.x,dim.y/m_scale.y); }

private:
	FontManager(const FontManager &);
	FontManager &operator=(const FontManager &);

	FT_Library m_library;
	vector2f m_scale;

	std::map<std::string, TextureFont*> m_textureFonts;
	std::map<std::string, VectorFont*> m_vectorFonts;
};

#endif

