#ifndef __NW_BITMAP_H
#define __NW_BITMAP_H

#include "nuSystem.h"

class NwBitmap
{
public:
	enum emColorType{
		RGB_565		= 1,
		RGBA_8888	= 2,
	};
	NwBitmap(void);
	virtual ~NwBitmap(void);
	bool	loadBmpFile(const TCHAR* ptsBmpName);
	void	bitmapFree();
private:
	nuINT		m_nColorType;
	nuSHORT		m_nBitCount;
	nuINT		m_nWidth;
	nuINT		m_nHeight;
	nuINT		m_nLineBytes;
	nuINT		m_nBufferLen;
	nuBYTE*		m_pBmpBuffer;

};


#endif
