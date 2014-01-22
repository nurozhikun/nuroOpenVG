#ifndef __NU_OPENVG_H
#define __NU_OPENVG_H

#include <vg/openvg.h>   
#include <vg/vgu.h>  
#include "nuSystem.h"
#include <windows.h>
#include <Dbgapi.h>

namespace MYOPENVG
{
	enum emConstant
	{
		DASH_STEP_COUNT = 5,
	};
	enum emSvgType
	{
		SVG_FIGURE	 = 0,
		SVG_POLYLINE = 1,
		SVG_POLYGON	 = 10,
		SVG_ELLIPSE_EDGE,
		SCG_ELLIPSE_FILL,
		SVG_RECT_LTWH,//left,top,width,height
		SVG_RECT_LBWH,//left,bottom, width, height
		SVG_RECT_LTRB,//left,top,right,bottom
		SVG_RECT_LBRT,//left,bottom,right,top
		SVG_IMAGE = 100,
		SVG_FONT  = 200,
	};
	enum emStrokeType
	{
		STROKE_NONE	= 0,
		STROKE_SOLID,
		STROKE_DASH,
	};
	enum emFillType
	{
		FILL_NONE	= 0,
		FILL_PAINT,
	};
	typedef struct tagSVG_ATTRIBUTE
	{
//		nuWORD		nSvgType;
		VGshort		nStrokeType;
		VGshort		nFillType;
		VGshort		nLineWidth;
		VGint		nLineColor;
		VGshort		nDashSteps[DASH_STEP_COUNT];
		VGshort		nDashPhase;
		VGint		nFillColor;
	}SVG_ATTRIBUTE;
	enum emImagePaintMode
	{
		IMG_PAINT_COPY	= 0,
		IMG_PAINT_REPEAT,
	};
	enum emImageType
	{
		IMG_SRC_NURO_BMP,
		IMG_SRC_BMP_NAME,
	};
	typedef struct tagSVG_IMG_DATA
	{
		VGshort nImgPaintMode;
		VGshort	nImageSrcType;
		void*	pImageSrc;
		VGint	nDx;
		VGint	nDy;
		VGint	nSx;
		VGint	nSy;
		VGint	nW;
		VGint	nH;
		VGint	nRotateAngle;
	}SVG_IMG_DATA;

	enum emImagePixelType
	{
		IMG_PIXEL_sRGB_565,
		IMG_PIXEL_SRGB_8888,
	};
	typedef struct tagNURO_IMAGE
	{
		int		imgPixelType;
		int		imgWidth;
		int		imgHeight;
		int		imgBitCount;
		int		imgLineLength;
		char*	imgBuffer;
	}NURO_IMAGE;
	typedef struct tagVG_MODULE_INIT
	{
		nuHDC	display;
		nuHWND	wnd;
		int		width;
		int		height;
		char*	sTtfFile;
	}VG_MODULE_INIT;

	enum emDrawTextVGFormat
	{
		DT_VG_F_TOP		= 0x00,
		DT_VG_F_LEFT	= 0x00,
		DT_VG_F_CENTER	= 0x01,
		DT_VG_F_RIGHT	= 0x02,
		DT_VG_F_VCENTER	= 0x04,
		DT_VG_F_BOTTOM	= 0x08,
		DT_VG_F_CALRECT = 0x0400,
	};

	typedef struct tagNURO_LOGFONT
	{
		long		lfCenterColor;
		long		lfRimColor;
		long		lfHeight;
		long		lfWidth;
		long		lfEscapement;
		long		lfWeight;
	}NURO_LOGFONT;
	class NIOpenVgApi
	{
	public:
		NIOpenVgApi(){}
		virtual ~NIOpenVgApi(){}
// 		virtual void setPenVG(const NUROPEN& pen) = 0;
// 		virtual void setBrushVG(const NUROBRUSH& brush) = 0;
		virtual int createOpenVG() = 0;
		virtual int appendSvgFigure(const long* pdata, int nCount, emSvgType svgType, const SVG_ATTRIBUTE& svgAttr) = 0;
		virtual int appendSvgImage(const SVG_IMG_DATA& imgData) = 0;
		virtual int drawSvg() = 0;
		virtual int readPixels(NUROBITMAP* pBmp) = 0;
		typedef struct tagSF_CLEAR_PARAM{
			NURO_IMAGE*		pImg;
			VGbyte			nRed;
			VGbyte			nGreen;
			VGbyte			nBlue;
			VGbyte			nAlpha;
		}SF_CLEAR_PARAM;
		virtual int clearSurface(const SF_CLEAR_PARAM& clearParam) = 0;
		virtual void setLogFont(const NURO_LOGFONT& logFont) = 0;
		virtual void getLogFont(NURO_LOGFONT& logFont) = 0;
		virtual int drawText(WCHAR* pText, int nCount, long xOrigion, long yOrigion) = 0;
		virtual int drawText(WCHAR* pText, int nCount, PNURORECT pRect, int nFormat) = 0;
	};


	inline int getMainPath(char *pszPath, int nBuffLen)
	{
		if( NULL == pszPath )
		{
			return 0;
		}
		TCHAR sFile[NURO_MAX_PATH];
		nuGetModuleFileName(NULL, sFile, NURO_MAX_PATH);
		int nLen = 0;
		for(int i = nuTcslen(sFile) - 1; i >= 0; i--)
		{
			if( sFile[i] == '\\' )
			{
				sFile[i+1] = '\0';
				nLen = i+1;
				break;
			}
		}
		if( nLen >= nBuffLen )
		{
			return 0;
		}
		for(int j = 0; j < nLen; ++j)
		{
			pszPath[j] = (char)sFile[j];
		}
		pszPath[nLen] = '\0';
	}
	inline VGint sRGBA(VGbyte r, VGbyte g, VGbyte b, VGbyte a){
//		return ((((VGint)r)<<24)|(((VGint)g)<<16)|(((VGint)b)<<8)|a);
		return (r*0xFFFFFF + g*0xFFFF + b*0xFF + a);
	}
	inline void setPolygonPathSegments(VGubyte* pCmd, VGint nCount){
		pCmd[0] = VG_MOVE_TO_ABS;
		for(int i = 1; i < nCount - 1; ++i)
		{
			pCmd[i] = VG_LINE_TO_ABS;
		}
		pCmd[nCount-1] = VG_CLOSE_PATH;
	}

}





#endif