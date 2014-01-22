#ifndef __NW_OPENVG_H
#define __NW_OPENVG_H

#include <vg/openvg.h>   
#include <vg/vgu.h>   
#include <EGL/egl.h>   
#include "nuDefine.h"
#include "NwSvg.h"
#include "NwFont.h"



class NwOpenVG : public NIOpenVgApi
{
public:
	NwOpenVG(void);
	~NwOpenVG(void);

	bool initOpenVG(nuHDC display, nuHWND wnd, int w = 800, int h = 480, char* sTtsFile = NULL);
	void freeOpenVG();

	virtual int createOpenVG();
	virtual void setPenVG(const NUROPEN& pen);
	virtual void setBrushVG(const NUROBRUSH& brush);
	virtual int appendSvgFigure(const long* pdata, int nCount, emSvgType svgType, const SVG_ATTRIBUTE& svgAttr);
	virtual int appendSvgImage(const SVG_IMG_DATA& imgData);
	virtual int drawSvg();
	virtual int readPixels(NUROBITMAP* pBmp);
	virtual int clearSurface(const SF_CLEAR_PARAM& clearParam);

	virtual void setLogFont(const NURO_LOGFONT& logFont);
	virtual void getLogFont(NURO_LOGFONT& logFont);

	virtual int drawText(WCHAR* pText, int nCount, long xOrigion, long yOrigion);
	virtual int drawText(WCHAR* pText, int nCount, PNURORECT pRect, int nFormat);

	static void setStrokeProperty(VGPaint paint, const SVG_ATTRIBUTE& svgAttr);
	static void setFillProperty(VGPaint paint, const SVG_ATTRIBUTE& svgAttr);

	VGPaint	getPaintStroke() { return m_paintStroke; }
	VGPaint getPaintFill()   { return m_paintFill; }
protected:
	bool creatDefaultPaint();

private:
	EGLDisplay	m_eglDisplay;
	EGLConfig	m_eglConfig;
	EGLSurface	m_eglSurface;
	EGLContext	m_eglContext;

	nuHDC		m_hDisplay;
	nuHWND		m_hWnd;
	VGint		m_nSurfaceW;
	VGint		m_nSurfaceH;
	char		m_sFontPath[NURO_MAX_PATH];

//	VGImage		m_imgBuffer;
	bool		m_bCreate;

	VGPaint		m_paintStroke;
	VGPaint     m_paintFill;

//	VGPath		m_path;

	//lines and polylines
	NwSvgList	m_svgList;

	NwSvgFigure	m_svgFigure;
	NwSvgImage	m_svgImage;
	NwFont		m_font;
};

#endif