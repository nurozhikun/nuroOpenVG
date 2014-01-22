#include "NwFreeType.h"

NwFreeType::NwFreeType(void)
{
	m_library = NULL;
	m_face	  = NULL;
}

NwFreeType::~NwFreeType(void)
{
}

int NwFreeType::initFreeType(char *ftFile)
{
	int nError = FT_Init_FreeType(&m_library);
	if( nError )
	{
		return nError;
	}
	nError = FT_New_Face(m_library, ftFile, 0, &m_face);
	if( nError )
	{
		return nError;
	}
	nError = FT_Set_Pixel_Sizes(m_face, 0, FT_OUTLINE_SIZE/64);
	if( nError )
	{
		return nError;
	}
	return 0;
}

FT_GlyphSlot NwFreeType::loadGlyph(long nCharCode)
{ 
	FT_UInt nGlyphIdx = FT_Get_Char_Index( m_face, nCharCode );
	int nError = FT_Load_Glyph(m_face, nGlyphIdx, FT_LOAD_DEFAULT);
	if( nError )
	{
		return NULL;
	}
	return m_face->glyph;
}