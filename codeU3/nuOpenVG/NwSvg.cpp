#include "NwSvg.h"
#include "NwOpenVG.h"

NwSvg::NwSvg(emSvgType svgType)
//	:NwSvgProperty(type)
{
	m_svgType = typeType(svgType);
}

emSvgType NwSvg::typeType(emSvgType svgType)
{
	if( svgType < SVG_IMAGE )
	{
		return SVG_FIGURE;
	}
	else if( svgType < SVG_FONT )
	{
		return SVG_IMAGE;
	}
	else
	{
		return SVG_FONT;
	}
}

NwSvg::~NwSvg()
{
	
}

bool NwSvg::isSimilar(emSvgType svgType, const SVG_ATTRIBUTE* pAttribute)
{
	return (typeType(svgType) == m_svgType && isSameAttribute(pAttribute));
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//------------------------for class NwSvgFigure-------------------------------------
NwSvgFigure::NwSvgFigure()
:NwSvg(SVG_FIGURE)
{
	m_svgAttribute.nStrokeType  = STROKE_NONE;
	m_svgAttribute.nFillType	= FILL_NONE;
	m_path = VG_INVALID_HANDLE;
	m_bHasData	= false;
}
NwSvgFigure::NwSvgFigure(const SVG_ATTRIBUTE& svgAttr, emSvgType svgType /* = SVG_FIGURE */)
:NwSvg(svgType)
{
	m_svgAttribute = svgAttr;
	m_path = VG_INVALID_HANDLE;
	m_bHasData	= false;
}
NwSvgFigure::~NwSvgFigure()
{
	if( VG_INVALID_HANDLE != m_path )
	{
		vgClearPath(m_path, VG_PATH_CAPABILITY_ALL);
		vgDestroyPath(m_path);
	}
}
void NwSvgFigure::clearSvgFigure()
{
	if( VG_INVALID_HANDLE != m_path )
	{
		vgClearPath(m_path, VG_PATH_CAPABILITY_ALL);
		vgDestroyPath(m_path);
		m_path = VG_INVALID_HANDLE;
	}
	m_bHasData	= false;
}
int NwSvgFigure::appendSvgFigure(const long* pData, int nCount, emSvgType svgType, const SVG_ATTRIBUTE& svgAttr)
{
	if( m_bHasData && !isSameAttribute(&svgAttr) )
	{
		drawSvgFigure();
	}
	if( VG_INVALID_HANDLE == m_path )
	{
		if( !createPath() )
		{
			return false;
		}
	}
	m_svgAttribute = svgAttr;
	appendSvgFigure(pData, nCount, svgType);
	return 0;
}

void NwSvgFigure::drawSvgFigure()
{
	if( VG_INVALID_HANDLE != m_path && m_bHasData )
	{
		draw();
		clearSvgFigure();
	}
}

bool NwSvgFigure::isSameAttribute(const SVG_ATTRIBUTE* pAttribute)
{
	if( NULL != pAttribute &&
		pAttribute->nStrokeType == m_svgAttribute.nStrokeType && 
		pAttribute->nFillType   == m_svgAttribute.nFillType   && 
		( STROKE_NONE == m_svgAttribute.nStrokeType || (pAttribute->nLineWidth == m_svgAttribute.nLineWidth && pAttribute->nLineColor == m_svgAttribute.nLineColor) ) && 
		( FILL_NONE == m_svgAttribute.nFillType || pAttribute->nFillColor == m_svgAttribute.nFillColor ))
	{
		return true;
	}
	return false;
}

void NwSvgFigure::setAttribute()
{
	VGPaint paint = vgGetPaint(VG_STROKE_PATH);
	if( STROKE_NONE != m_svgAttribute.nStrokeType &&
		VG_INVALID_HANDLE != paint )
	{
		NwOpenVG::setStrokeProperty(paint, m_svgAttribute);
	}
	paint = vgGetPaint(VG_FILL_PATH);
	if( FILL_NONE != m_svgAttribute.nFillType &&
		VG_INVALID_HANDLE != paint )
	{
		NwOpenVG::setFillProperty(paint, m_svgAttribute);
	}
}

bool NwSvgFigure::createPath()
{
	if( VG_INVALID_HANDLE == m_path )
	{
		m_path = vgCreatePath(VG_PATH_FORMAT_STANDARD,VG_PATH_DATATYPE_S_32, 1.0f, 0.0f, 0, 0,VG_PATH_CAPABILITY_ALL);
	}
	if( VG_INVALID_HANDLE == m_path )
	{
		return false;
	}
	else
	{
		return true;
	}
}

void NwSvgFigure::appendSvgFigure(const long* pData, int nCount, emSvgType svgType)
{
	if( NULL == pData || !createPath() )
	{
		return;
	}
	if( SVG_POLYLINE == svgType )
	{
		VGint nPtCount = nCount / 2;
		VGubyte* pCmd = new VGubyte[nPtCount];
		if( NULL == pCmd )
		{
			return;
		}
		pCmd[0] = VG_MOVE_TO_ABS;
		for(int i = 1; i < nPtCount; ++i)
		{
			pCmd[i] = VG_LINE_TO_ABS;
		}
		vgAppendPathData(m_path, nPtCount, pCmd, pData);
		delete pCmd;
	}
	else if( SVG_POLYGON == svgType )
	{
		VGint nPtCount = nCount / 2 + 1;
		VGubyte* pCmd = new VGubyte[nPtCount];
		if( NULL == pCmd )
		{
			return;
		}
		pCmd[0] = VG_MOVE_TO_ABS;
		for(int i = 1; i < nPtCount - 1; ++i)
		{
			pCmd[i] = VG_LINE_TO_ABS;
		}
		pCmd[nPtCount-1] = VG_CLOSE_PATH;
		vgAppendPathData(m_path, nPtCount, pCmd, pData);
		delete pCmd;
	}
	else if( SVG_ELLIPSE_EDGE == svgType )
	{
		if( nCount < 5 )
		{
			return;
		}
		//		vguEllipse(m_path, pData[0], pData[1], pData[2], pData[3]);
		VGint pLCCWdata[7];
		pLCCWdata[0] = pData[0] - pData[2];
		pLCCWdata[1] = pData[1];
		pLCCWdata[2] = pData[2];
		pLCCWdata[3] = pData[3];
		pLCCWdata[4] = pData[4];
		pLCCWdata[5] = pData[0] + pData[2];
		pLCCWdata[6] = pData[1];
		VGubyte pCmd[2] = {VG_MOVE_TO_ABS, VG_LCCWARC_TO_ABS};
		vgAppendPathData(m_path, 2, pCmd, pLCCWdata);
		pLCCWdata[0] = pData[0] + pData[2];
		pLCCWdata[5] = pData[0] - pData[2];
		vgAppendPathData(m_path, 2, pCmd, pLCCWdata);
	}
	m_bHasData = true;
}

void NwSvgFigure::draw()
{
	setAttribute();
	VGbitfield paintModes = 0;
	if( STROKE_NONE != m_svgAttribute.nStrokeType )
	{
		paintModes |= VG_STROKE_PATH;
	}
	if( FILL_NONE != m_svgAttribute.nFillType )
	{
		paintModes |= VG_FILL_PATH;
	}
	vgDrawPath(m_path, paintModes);
	//vgFinish();
	vgClearPath(m_path, VG_PATH_CAPABILITY_ALL);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/*
NwSvgPath::NwSvgPath()
:NwSvg(SVG_FIGURE)
{
	;
}
NwSvgPath::~NwSvgPath()
{

}
*/

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//------------------------for class NwSvgImage--------------------------------------

void NwSvgImage::draw()
{
	if( NULL == m_imgData.pImageSrc )
	{
		return ;
	}
	if( IMG_SRC_NURO_BMP == m_imgData.nImageSrcType )
	{
		PNUROBITMAP pBmp =(PNUROBITMAP)m_imgData.pImageSrc;
		if( IMG_PAINT_REPEAT == m_imgData.nImgPaintMode )
		{
			int nX = m_imgData.nDx; 
			int nY = m_imgData.nDy;
			int nXMax = m_imgData.nDx + m_imgData.nW;
			int nYMax = m_imgData.nDy + m_imgData.nH;
			while( nY < nYMax )
			{
				int nH = NURO_MIN(pBmp->bmpHeight, nYMax - nY);
				while( nX < nXMax )
				{
					int nW = NURO_MIN(pBmp->bmpWidth, nXMax - nX);
					vgWritePixels(pBmp->pBmpBuff, pBmp->bmpWidth*2, VG_sRGB_565
						, nX, nY, nW, nH);
					nX += nW;
				} 
				nY += nH;
			}
		}
		else
		{
			vgWritePixels(pBmp->pBmpBuff, pBmp->bmpWidth*2, VG_sRGB_565, m_imgData.nDx, m_imgData.nDy, m_imgData.nW, m_imgData.nH);
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//--------------------------for class NwSvgList--------------------------------

NwSvg* NwSvgList::getSvgHolder(emSvgType svgType, const SVG_ATTRIBUTE* pSvgAttr /* = NULL */)
{
	if( NULL == m_pListEnd || 
		0 == m_pListEnd->nCount ||
		!m_pListEnd->ppSvgPage[m_pListEnd->nCount - 1]->isSimilar(svgType, pSvgAttr) )
	{
		if( NULL == m_pListEnd ||
			LIST_PAGE_SIZE == m_pListEnd->nCount )
		{
			SVG_LIST_PAGE* pPage = new SVG_LIST_PAGE;
			if( NULL == pPage )
			{
				return NULL;
			}
			memset(pPage, 0, sizeof(SVG_LIST_PAGE));
			if( NULL == m_pListHead )
			{
				m_pListHead = m_pListEnd = pPage;
			}
			else
			{
				m_pListEnd = m_pListEnd->pNextPage = pPage;
			}
		}
		switch( NwSvg::typeType(svgType) )
		{
		case SVG_FIGURE:
			m_pListEnd->ppSvgPage[m_pListEnd->nCount] = new NwSvgFigure(*pSvgAttr);
			break;
		case SVG_IMAGE:
			m_pListEnd->ppSvgPage[m_pListEnd->nCount] = new NwSvgImage();
			break;
		default:
			m_pListEnd->ppSvgPage[m_pListEnd->nCount] = NULL;
			break;
		}
		if( NULL != m_pListEnd->ppSvgPage[m_pListEnd->nCount] )
		{
			++m_pListEnd->nCount;
		}
		else
		{
			return NULL;
		}
	}
	return m_pListEnd->ppSvgPage[m_pListEnd->nCount - 1];
}

void NwSvgList::clearSvgList()
{
	while( NULL != m_pListHead )
	{
		m_pListEnd = m_pListHead;
		m_pListHead = m_pListHead->pNextPage;
		for( int i = 0; i < LIST_PAGE_SIZE; ++i )
		{
			if( NULL != m_pListEnd->ppSvgPage[i] )
			{
				delete m_pListEnd->ppSvgPage[i];
			}
		}
		delete m_pListEnd;
	}
	m_pListEnd = NULL;
}

void NwSvgList::drawSvg()
{
	SVG_LIST_PAGE *pPage = m_pListHead;
	while(NULL != pPage)
	{
		for(int i = 0; i < pPage->nCount; ++i)
		{
			pPage->ppSvgPage[i]->draw();
		}
		pPage = pPage->pNextPage;
	}
}