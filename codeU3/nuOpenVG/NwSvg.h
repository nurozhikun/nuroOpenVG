#ifndef __NW_SVG_H
#define __NW_SVG_H

#include <math.h>
#include "nuOpenVG.h"

using namespace MYOPENVG;

class NwSvg// : public NwSvgProperty
{
public:
	NwSvg(emSvgType svgType);
	virtual ~NwSvg();
	static emSvgType typeType(emSvgType svgType);

	virtual void appendSvgFigure(const long* pData, int nCount, emSvgType svgType){}
	virtual void appendSvgImage(const SVG_IMG_DATA& imgData){}
	virtual void draw(){}
	bool isSimilar(emSvgType svgType, const SVG_ATTRIBUTE* pAttribute = NULL);
	void setType(emSvgType svgType) { m_svgType = typeType(svgType); }
	emSvgType getType() { return m_svgType;}
protected:
	virtual bool isSameAttribute(const SVG_ATTRIBUTE* pAttribute) { return false; }
private:
	emSvgType		m_svgType;
};

class NwSvgFigure : public NwSvg
{
public:
	NwSvgFigure();
	NwSvgFigure(const SVG_ATTRIBUTE& svgAttr, emSvgType svgType = SVG_FIGURE);
	virtual ~NwSvgFigure();
	void clearSvgFigure();
	int appendSvgFigure(const long* pData, int nCount, emSvgType svgType, const SVG_ATTRIBUTE& svgAttr);
	void drawSvgFigure();

protected:
	virtual bool isSameAttribute(const SVG_ATTRIBUTE* pAttribute);
	virtual void appendSvgFigure(const long* pData, int nCount, emSvgType svgType);
	void setAttribute();
	bool createPath();
	virtual void draw();
private:
	SVG_ATTRIBUTE	m_svgAttribute;
	VGPath			m_path;
	bool			m_bHasData;
};

/*
class NwSvgPath : public NwSvg
{
public:
	NwSvgPath();
	virtual ~NwSvgPath();

private:
	SVG_ATTRIBUTE	m_svgAttribute;
	VGPath			m_path;
};
*/

class NwSvgImage : public NwSvg
{
public:
	NwSvgImage(emSvgType svgType = SVG_IMAGE):NwSvg(svgType){
		
	}
	virtual ~NwSvgImage(){

	}

	
	void drawSvgImage(const SVG_IMG_DATA& imgData) { 
		appendSvgImage(imgData); 
		draw();
	}
protected:
	virtual void appendSvgImage(const SVG_IMG_DATA& imgData){
		m_imgData = imgData;
	}
	virtual void draw();
private:
	SVG_IMG_DATA	m_imgData;
};

class NwSvgList
{
public:
	enum emConstValues{
		LIST_PAGE_SIZE = 32,
	};
	struct SVG_LIST_PAGE{
		NwSvg*			ppSvgPage[LIST_PAGE_SIZE];
		unsigned int	nCount;
		SVG_LIST_PAGE*  pNextPage;
	};
	NwSvgList(){
		m_pListHead = NULL;
		m_pListEnd	= NULL;
		m_nPageCount  = 0;
		m_nSize		  = 0;
	}
	virtual ~NwSvgList(){
		clearSvgList();
	}
	NwSvg* getSvgHolder( emSvgType svgType, const SVG_ATTRIBUTE* pSvgAttr = NULL);
	void clearSvgList();
	void drawSvg();
private:
	SVG_LIST_PAGE*	m_pListHead;
	SVG_LIST_PAGE*	m_pListEnd;
	unsigned int	m_nPageCount;
	unsigned int	m_nSize;

};




#endif
