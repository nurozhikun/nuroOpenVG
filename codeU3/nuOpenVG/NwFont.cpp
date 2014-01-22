#include "NwFont.h"

NwFont::NwFont(void)
{
	m_vgFont = VG_INVALID_HANDLE;
	m_pFontPageHead	= NULL;
	m_pFontPageEnd  = NULL;
	m_fScale = 1.0f;
}

NwFont::~NwFont(void)
{
	freeFontPage();
}

bool NwFont::initFontVG(char* sFileTtf)
{
	if( m_freeType.initFreeType(sFileTtf) )
	{
		return false;
	}
	m_vgFont = vgCreateFont(0);
	if( VG_INVALID_HANDLE == m_vgFont )
	{
		return false;
	}
	return true;
}

void NwFont::freeFontVG()
{
	freeFontPage();
	if( VG_INVALID_HANDLE != m_vgFont )
	{
		vgDestroyFont(m_vgFont);
		m_vgFont = VG_INVALID_HANDLE;
	}
}

bool NwFont::addGlyph(int nGlyphCode)
{
	if( VG_INVALID_HANDLE == m_vgFont )
	{
		return false;
	}
	FT_GlyphSlot glyph = m_freeType.loadGlyph(nGlyphCode);
	if( NULL == glyph )
	{
		return false;
	}
	VGPath fontPath = vgCreatePath(VG_PATH_FORMAT_STANDARD,VG_PATH_DATATYPE_S_32, 1.0f, 0.0f, 0, 0,VG_PATH_CAPABILITY_ALL);
	if( VG_INVALID_HANDLE == fontPath )
	{
		return false;
	}
	VGubyte *pCmd = new VGubyte[glyph->outline.n_points];
	if( NULL == pCmd )
	{
		return false;
	}
	VGint nCmdCount = 0;
	VGint nIndex = 0;
	for(short i = 0; i < glyph->outline.n_contours; ++i)
	{
		nCmdCount = glyph->outline.contours[i] - nIndex + 1;
		setPolygonPathSegments(pCmd, nCmdCount);
		vgAppendPathData(fontPath, nCmdCount, pCmd, &glyph->outline.points[nIndex]);
		nIndex = glyph->outline.contours[i]+1;
	}
	VGfloat glyphOrigin[2] = {0.0f, 0.0f};
	VGfloat glyphEscapement[2] = {glyph->metrics.horiAdvance, 0.0f};
	vgSetGlyphToPath(m_vgFont, nGlyphCode, fontPath, VG_FALSE, glyphOrigin, glyphEscapement);
	FONT_NODE fontNode;
	fontNode.nGlyphCode  = nGlyphCode;
	fontNode.nEscapement = glyph->metrics.horiAdvance;
	insertFontToPage(fontNode);
	vgDestroyPath(fontPath);
	delete[] pCmd;
	return true;
}


bool NwFont::insertFontToPage(const FONT_NODE& fontNode)
{
	if( NULL == m_pFontPageEnd || FONT_PAGE_SIZE == m_pFontPageEnd->nCount )
	{
		FONT_PAGE* page = new FONT_PAGE;
		if( NULL == page )
		{
			return false;
		}
		nuMemset(page, 0, sizeof(FONT_PAGE));
		if( NULL == m_pFontPageEnd )
		{
			m_pFontPageHead = m_pFontPageEnd = page;
		}
		else
		{
			m_pFontPageEnd = m_pFontPageEnd->pNext = page;
		}
	}
	m_pFontPageEnd->pPage[m_pFontPageEnd->nCount++] = fontNode;
	return true;
}

void NwFont::freeFontPage()
{
	while(NULL != m_pFontPageHead)
	{
		for(long i = 0; i < m_pFontPageHead->nCount; ++i)
		{
			vgClearGlyph(m_vgFont, m_pFontPageHead->pPage[i].nGlyphCode);
		}
		m_pFontPageEnd = m_pFontPageHead->pNext;
		delete m_pFontPageHead;
		m_pFontPageHead = m_pFontPageEnd;
	}
}

int NwFont::drawTextVG(WCHAR* pText, int nCount, PNURORECT pRect, int nFormat)
{
	if( NULL == pText || NULL == pRect )
	{
		return 0;
	}
	if( nFormat&DT_VG_F_CALRECT )
	{
		long nTotalWidth = nCount * m_logFont.lfWidth;
		pRect->right	= pRect->left + nTotalWidth;
		pRect->bottom	= pRect->top + m_logFont.lfHeight;
		return nCount;
	}
	else
	{
		return drawTextVG(pText, nCount, pRect->left, pRect->top);
	}
}

int NwFont::drawTextVG(WCHAR* pText, int nCount, long xOrigion, long yOrigion)
{
	if( NULL == pText || NULL == m_vgFont )
	{
		return 0;
	}
	VGfloat		pfMatrix[9];
	vgGetMatrix(pfMatrix);
	VGfloat fOrigion[2] = { xOrigion, yOrigion };
	vgSetfv(VG_GLYPH_ORIGIN, 2, fOrigion);
	vgScale(m_fScale, m_fScale);
	for(int i = 0; i < nCount; ++i)
	{
		FONT_NODE* pFontNode = lookForGlyph(pText[i]);
		if( NULL == pFontNode )
		{
			addGlyph(pText[i]);
		}
		vgDrawGlyph(m_vgFont, pText[i], VG_FILL_PATH|VG_STROKE_PATH, VG_FALSE);
	}
	vgLoadMatrix(pfMatrix);
	return nCount;
}

void NwFont::setLogFont(const NURO_LOGFONT& font)
{
	m_logFont = font;
	if( 0 == font.lfHeight )
	{
		m_logFont.lfHeight = font.lfWidth;
	}
	if( 0 == font.lfWidth )
	{
		m_logFont.lfWidth = font.lfHeight;
	}
	m_fScale = m_logFont.lfHeight * 1.0f / NwFreeType::FT_OUTLINE_SIZE;
}

void NwFont::getLogFont(NURO_LOGFONT& font)
{
	font = m_logFont;
}

NwFont::FONT_NODE* NwFont::lookForGlyph(VGuint nGlyphCode)
{
	FONT_PAGE* pFontPage = m_pFontPageHead;
	while( NULL != pFontPage )
	{
		for(long i = 0; i < pFontPage->nCount; ++i)
		{
			if( nGlyphCode == pFontPage->pPage[i].nGlyphCode )
			{
				return &pFontPage->pPage[i];
			}
		}
		pFontPage = pFontPage->pNext;
	}
	return NULL;
}

