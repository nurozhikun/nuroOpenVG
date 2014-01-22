#ifndef __NW_FREETYPE_H
#define __NW_FREETYPE_H

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>
#include <freetype/ftstroke.h>

class NwFreeType
{
public:
	enum emConst{
		FT_OUTLINE_SIZE	= 8 * 64,
	};
	NwFreeType(void);
	~NwFreeType(void);

	int initFreeType(char *ftFile);
	FT_GlyphSlot loadGlyph(long nCharCode);
	//freetype member
	FT_Library	m_library;
	FT_Face		m_face;
};

#endif