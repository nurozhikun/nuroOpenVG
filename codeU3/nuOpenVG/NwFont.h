#ifndef __NW_FONT_H
#define __NW_FONT_H

#include "NwFreeType.h"
#include "nuOpenVG.h"

using namespace MYOPENVG;

class NwFont
{
public:
	enum emFontConst{
		FONT_PAGE_SIZE	= 8*1024,
	};
	typedef struct tagFONT_NODE{
		VGuint		nGlyphCode;
		VGuint		nEscapement;
//		VGPath		glyphPath;
	}FONT_NODE; 
	typedef struct tagFONT_PAGE{
		FONT_NODE		pPage[FONT_PAGE_SIZE];
		long			nCount;
		tagFONT_PAGE*	pNext;
	}FONT_PAGE;
	NwFont(void);
	~NwFont(void);

	bool initFontVG(char* sFileTtf);
	void freeFontVG();
	void setLogFont(const NURO_LOGFONT& font);
	void getLogFont(NURO_LOGFONT& font);
	int drawTextVG(WCHAR* pText, int nCount, PNURORECT pRect, int nFormat);
	int drawTextVG(WCHAR* pText, int nCount, long xOrigion, long yOrigion);

private:
	bool addGlyph(int nGlyphCode);
	
	bool insertFontToPage(const FONT_NODE& fontNode);
	void freeFontPage();
	FONT_NODE*	lookForGlyph(VGuint	nGlyphCode);
private:
	VGFont	m_vgFont;
	NwFreeType	m_freeType;
	FONT_PAGE*	m_pFontPageHead;
	FONT_PAGE*	m_pFontPageEnd;
	NURO_LOGFONT	m_logFont;
	VGfloat		m_fScale;
};

#endif
