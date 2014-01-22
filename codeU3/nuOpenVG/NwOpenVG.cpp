#include "NwOpenVG.h"

NwOpenVG::NwOpenVG(void)
{
	m_eglDisplay = EGL_NO_DISPLAY;
	m_eglSurface = EGL_NO_SURFACE;
	m_eglContext = EGL_NO_CONTEXT;
	m_eglConfig	 = NULL;

	m_paintFill		= VG_INVALID_HANDLE;
	m_paintStroke	= VG_INVALID_HANDLE;
//	m_imgBuffer		= VG_INVALID_HANDLE;
	m_hDisplay	= NULL;
	m_hWnd		= NULL;
	nuMemset(m_sFontPath, 0, sizeof(m_sFontPath));
	m_bCreate = false;
}

NwOpenVG::~NwOpenVG(void)
{
	freeOpenVG();
}

bool NwOpenVG::initOpenVG(nuHDC display, nuHWND wnd, int w /* = 800 */, int h /* = 480 */, char* sTtsFile /* = NULL */)
{
	m_hDisplay	= display;
	m_hWnd		= wnd;
	m_nSurfaceW	= w;
	m_nSurfaceH = h;
	if( NULL != sTtsFile )
	{
		nuStrncpy(m_sFontPath, sTtsFile, NURO_MAX_PATH-1);
	}

	/*
	m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);//reinterpret_cast<NativeDisplayType>(display)
	if( EGL_NO_DISPLAY == m_eglDisplay )
	{
		return false;
	}
	EGLint nMajor, nMinor;
	if( eglInitialize(m_eglDisplay, &nMajor, &nMinor) == EGL_FALSE ||
		eglBindAPI(EGL_OPENVG_API) == EGL_FALSE )
	{
		eglTerminate(m_eglDisplay);
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}
	EGLint anCfgAttribList[] = { EGL_BUFFER_SIZE, 16,  EGL_DEPTH_SIZE, 16,  EGL_RENDERABLE_TYPE, EGL_OPENVG_BIT, EGL_NONE };
	EGLint nCfgCnt;
	if( eglChooseConfig(m_eglDisplay, anCfgAttribList, &m_eglConfig, 1, &nCfgCnt) == EGL_FALSE || 1 != nCfgCnt )
	{
		eglTerminate(m_eglDisplay);
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}

	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, reinterpret_cast<NativeWindowType>(wnd), NULL);
	if( EGL_NO_SURFACE == m_eglSurface )
	{
		
		eglTerminate(m_eglDisplay);
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}
	m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, NULL, NULL);
	if( EGL_NO_CONTEXT == m_eglContext )
	{
		
		eglDestroySurface(m_eglDisplay, m_eglSurface);
		eglTerminate(m_eglDisplay);
		m_eglSurface = EGL_NO_SURFACE;
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}
	if( eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE )
	{
		eglDestroyContext(m_eglDisplay, m_eglContext);
		eglDestroySurface(m_eglDisplay, m_eglSurface);
		eglTerminate(m_eglDisplay);
		m_eglContext = EGL_NO_CONTEXT;
		m_eglSurface = EGL_NO_SURFACE;
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}
	*/
//	createOpenVG();
// 	m_imgBuffer = vgCreateImage(VG_sRGB_565, 800, 480, VG_IMAGE_QUALITY_NONANTIALIASED);
// 	if( VG_INVALID_HANDLE == m_imgBuffer )
// 	{
// 		eglTerminate(m_eglDisplay);
// 		m_eglDisplay = EGL_NO_DISPLAY;
// 		VGErrorCode code = vgGetError();
// 		return false;
// 	}
// 	EGLSurface eglSurface = eglCreatePbufferFromClientBuffer(m_eglDisplay, EGL_OPENVG_IMAGE, (EGLClientBuffer)m_imgBuffer, m_eglConfig, anCfgAttribList);
// 	if( EGL_NO_SURFACE == eglSurface )
// 	{
// 		VGErrorCode code = vgGetError();
// 		return false;
// 	}
	return true;
}

int NwOpenVG::createOpenVG()
{
	if( m_bCreate )
	{
		return 0;
	}
	m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY/*reinterpret_cast<NativeDisplayType>(display)*/);
	if( EGL_NO_DISPLAY == m_eglDisplay )
	{
		return false;
	}
	EGLint nMajor, nMinor;
	if( eglInitialize(m_eglDisplay, &nMajor, &nMinor) == EGL_FALSE ||
		eglBindAPI(EGL_OPENVG_API) == EGL_FALSE )
	{
		eglTerminate(m_eglDisplay);
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}
	EGLint anCfgAttribList[] = { EGL_BUFFER_SIZE, 16,  EGL_DEPTH_SIZE, 16,  EGL_RENDERABLE_TYPE, EGL_OPENVG_BIT, EGL_NONE };
	EGLint nCfgCnt;
	if( eglChooseConfig(m_eglDisplay, anCfgAttribList, &m_eglConfig, 1, &nCfgCnt) == EGL_FALSE || 1 != nCfgCnt )
	{
		eglTerminate(m_eglDisplay);
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}

	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, reinterpret_cast<NativeWindowType>(m_hWnd), NULL);
	if( EGL_NO_SURFACE == m_eglSurface )
	{

		eglTerminate(m_eglDisplay);
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}
	m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, NULL, NULL);
	if( EGL_NO_CONTEXT == m_eglContext )
	{

		eglDestroySurface(m_eglDisplay, m_eglSurface);
		eglTerminate(m_eglDisplay);
		m_eglSurface = EGL_NO_SURFACE;
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}
	if( eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE )
	{
		eglDestroyContext(m_eglDisplay, m_eglContext);
		eglDestroySurface(m_eglDisplay, m_eglSurface);
		eglTerminate(m_eglDisplay);
		m_eglContext = EGL_NO_CONTEXT;
		m_eglSurface = EGL_NO_SURFACE;
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}
	if( !creatDefaultPaint() )
	{
		return false;
	}
	if( !m_font.initFontVG(m_sFontPath) )
	{
		return false;
	}
	m_bCreate = true;
	//	VGfloat clearColor[4] = {0.0f, 1.0f, 0.0f, 1.0f};
	//	vgSetfv(VG_CLEAR_COLOR, 4, clearColor);

 	VGfloat pfMatrix[9] = {1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, m_nSurfaceH, 1.0f};
 	vgLoadMatrix(pfMatrix);


	//	vgSetfv(VG_MATRIX_MODE,  VG_MATRIX_PATH_USER_TO_SURFACE, 9, pfMatrix);

	VGint clColor[4] = {0, 255, 0, 255};
	vgSetiv(VG_CLEAR_COLOR, 4, clColor);
	vgClear(0, 0, 800, 480);
	return 0;
}

void NwOpenVG::freeOpenVG()
{
	m_font.freeFontVG();
	if( EGL_NO_DISPLAY != m_eglDisplay )
	{
		eglMakeCurrent( m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
		if( EGL_NO_CONTEXT != m_eglContext )
		{
			eglDestroyContext(m_eglDisplay, m_eglContext);
			m_eglContext = EGL_NO_CONTEXT;
		}
		if( EGL_NO_SURFACE != m_eglSurface )
		{
			eglDestroySurface(m_eglDisplay, m_eglSurface);
			m_eglSurface = EGL_NO_SURFACE;
		}
		eglTerminate(m_eglDisplay);
		m_eglDisplay = EGL_NO_DISPLAY;
	}
	if( VG_INVALID_HANDLE != m_paintStroke )
	{
		vgDestroyPaint(m_paintStroke);
		m_paintStroke = VG_INVALID_HANDLE;
	}
	if( VG_INVALID_HANDLE != m_paintFill )
	{
		vgDestroyPaint(m_paintFill);
		m_paintFill = VG_INVALID_HANDLE;
	}
// 	if( VG_INVALID_HANDLE != m_imgBuffer )
// 	{
// 		vgDestroyImage(m_imgBuffer);
// 		m_imgBuffer = VG_INVALID_HANDLE;
// 	}
}

void NwOpenVG::setPenVG(const NUROPEN& pen)
{
	;
}

void NwOpenVG::setBrushVG(const NUROBRUSH& brush)
{
	;
}

int NwOpenVG::appendSvgFigure(const long* pdata, int nCount, emSvgType svgType, const SVG_ATTRIBUTE& svgAttr)
{
	if( NULL == pdata )
	{
		return -1;
	}
	return m_svgFigure.appendSvgFigure(pdata, nCount, svgType, svgAttr);
	/*
	NwSvg* pSvg = m_svgList.getSvgHolder(svgType, &svgAttr);
	if( NULL == pSvg )
	{
		return 1;
	}
	pSvg->appendSvgFigure(pdata, nCount, svgType);
	return 0;
	*/
}

int NwOpenVG::appendSvgImage(const SVG_IMG_DATA& imgData)
{
	m_svgFigure.drawSvgFigure();
	m_svgImage.drawSvgImage(imgData);
	return 0;
	/*
	NwSvg* pSvg = m_svgList.getSvgHolder(SVG_IMAGE);
	if( NULL == pSvg )
	{
		return 1;
	}
	pSvg->appendSvgImage(imgData);
	drawSvg();
	m_svgList.clearSvgList();
	return 0;
	*/
}

int NwOpenVG::drawSvg()
{
	m_svgList.drawSvg();
// 	short *pData = new short[800*480];
// 	vgReadPixels(pData, 0, VG_sRGB_565, 0, 0, 800, 480);
//	eglSwapBuffers(m_eglDisplay, m_eglSurface);
/*	delete pData;*/
	m_svgList.clearSvgList();
	return 0;
}

int NwOpenVG::readPixels(NUROBITMAP* pBmp)
{
//	m_svgList.drawSvg();
	m_svgFigure.drawSvgFigure();
	vgFinish();
	vgReadPixels(pBmp->pBmpBuff, pBmp->bmpWidth*2, VG_sRGB_565, 0, 0, pBmp->bmpWidth, pBmp->bmpHeight);
//	m_svgList.clearSvgList();
//	vgClear(0, 0, 800, 480);
	return 0;
}

int NwOpenVG::clearSurface(const SF_CLEAR_PARAM& clearParam)
{
	if( NULL == clearParam.pImg )
	{
		VGint clColor[4] = {clearParam.nRed, clearParam.nGreen, clearParam.nBlue, clearParam.nAlpha};
		vgSetiv(VG_CLEAR_COLOR, 4, clColor);
		vgClear(0, 0, m_nSurfaceW, m_nSurfaceH);
	}
	else
	{
		//...;
	}
	return 0;
}

bool NwOpenVG::creatDefaultPaint()
{
	if( VG_INVALID_HANDLE == m_paintStroke )
	{
		m_paintStroke = vgCreatePaint();
	}
	if( VG_INVALID_HANDLE == m_paintFill )
	{
		m_paintFill = vgCreatePaint();
	}
	if( VG_INVALID_HANDLE == m_paintStroke || VG_INVALID_HANDLE == m_paintFill )
	{
		return false;
	}
	vgSetPaint(m_paintStroke, VG_STROKE_PATH);
	vgSetPaint(m_paintFill, VG_FILL_PATH);
	return true;
}

void NwOpenVG::setStrokeProperty(VGPaint paint, const SVG_ATTRIBUTE& svgAttr)
{
	vgSetParameterf(paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
//	vgSeti(VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
//	vgSetParameterf(paint, VG_STROKE_LINE_WIDTH, svgAttr.nLineWidth);
// 	if( svgAttr.nLineWidth / 2)
// 	{
// 		vgSetf(VG_STROKE_LINE_WIDTH, svgAttr.nLineWidth+0.5f);
// 	}
// 	else
	{
		vgSetf(VG_STROKE_LINE_WIDTH, svgAttr.nLineWidth);
	}
//	vgSetParameterf(paint, VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_FASTER);
	vgSetf(VG_RENDERING_QUALITY, VG_RENDERING_QUALITY_FASTER);
//	vgSetParameterf(paint, VG_STROKE_CAP_STYLE, VG_CAP_ROUND);
	vgSetf(VG_STROKE_CAP_STYLE, VG_CAP_ROUND);
//	vgSetParameterf(paint, VG_STROKE_JOIN_STYLE, VG_JOIN_ROUND);
	vgSetf(VG_STROKE_JOIN_STYLE, VG_JOIN_ROUND);
	vgSetColor(paint, svgAttr.nLineColor);
	/*
	if( STROKE_DASH == svgAttr.nStrokeType )
	{
		VGfloat dash[DASH_STEP_COUNT];
		VGint	nDashCount = 0;
		while( nDashCount < DASH_STEP_COUNT )
		{
			if( 0 == svgAttr.nDashSteps[nDashCount] )
			{
				break;
			}
			dash[nDashCount] = svgAttr.nDashSteps[nDashCount];
			++nDashCount;
		}
// 		vgSetParameterfv(paint, VG_STROKE_DASH_PATTERN,nDashCount, dash);
// 		vgSetParameteri(paint, VG_STROKE_DASH_PHASE_RESET, VG_TRUE);
// 		vgSetParameteri(paint, VG_STROKE_DASH_PHASE, svgAttr.nDashPhase);
		vgSetfv(VG_STROKE_DASH_PATTERN, nDashCount, dash);
		vgSetf(VG_STROKE_DASH_PHASE_RESET, VG_TRUE);
		vgSetf(VG_STROKE_DASH_PHASE, svgAttr.nDashPhase);
	}
	else
	{
		vgSetfv(VG_STROKE_DASH_PATTERN, 0, NULL);
	}
	*/
}

void NwOpenVG::setFillProperty(VGPaint paint, const SVG_ATTRIBUTE& svgAttr)
{
//	vgSetParameteri(paint, VG_FILL_RULE, VG_NON_ZERO);
	vgSetf(VG_FILL_RULE, VG_NON_ZERO);
	vgSetColor(paint, svgAttr.nFillColor);
}

void NwOpenVG::setLogFont(const NURO_LOGFONT& logFont)
{
	m_font.setLogFont(logFont);
}

void NwOpenVG::getLogFont(NURO_LOGFONT& logFont)
{
	m_font.getLogFont(logFont);
}

int NwOpenVG::drawText(WCHAR* pText, int nCount, long xOrigion, long yOrigion)
{
	return m_font.drawTextVG(pText, nCount, xOrigion, yOrigion);
}

int NwOpenVG::drawText(WCHAR* pText, int nCount, PNURORECT pRect, int nFormat)
{
	return m_font.drawTextVG(pText, nCount, pRect, nFormat);
}