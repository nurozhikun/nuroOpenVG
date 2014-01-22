#include "NwBitmap.h"

using namespace NURO;

NwBitmap::NwBitmap(void)
{
	m_pBmpBuffer = NULL;
}

NwBitmap::~NwBitmap(void)
{
	if( NULL != m_pBmpBuffer )
	{
		nuFree(m_pBmpBuffer);
	}
}


bool NwBitmap::loadBmpFile(const TCHAR* ptsBmpName)
{
	nuFILE* fp = nuTfopen(ptsBmpName, NURO_FS_RB);
	if(fp == NULL)
	{
		return nuFALSE;
	}
	NURO_BITMAPFILEHEADER bmpFileHead;
	nuMemset(&bmpFileHead, 0, sizeof(NURO_BITMAPFILEHEADER));
	if(nuFread(&bmpFileHead, sizeof(NURO_BITMAPFILEHEADER), 1, fp) != 1)
	{
		nuFclose(fp);
		return nuFALSE;
	}
	NURO_BITMAPINFOHEADER bmpInfoHead;
	nuMemset(&bmpInfoHead, 0, sizeof(NURO_BITMAPINFOHEADER));
	if( nuFread(&bmpInfoHead, sizeof(NURO_BITMAPINFOHEADER), 1, fp) != 1)
	{
		nuFclose(fp);
		return nuFALSE;
	}
	nuUINT DataSizePerLine= (bmpInfoHead.biWidth * 16+31)/32*4;
	nuBYTE* pLine = (nuBYTE*)nuMalloc(DataSizePerLine);
	if(pLine == NULL)
	{
		nuFclose(fp);
		return nuFALSE;
	}
	nuMemset(pLine, 0, DataSizePerLine);
	int arrTag = (bmpInfoHead.biHeight>0)?0:1;
	bmpInfoHead.biHeight = NURO_ABS(bmpInfoHead.biHeight);

	nuUINT DataSizePerLineNew = DataSizePerLine;//(((nuWORD)(bmpInfoHead.biWidth))* 16+7)/8;

	m_nLineBytes = DataSizePerLineNew;//m_bitmap->bytesPerLine = DataSizePerLineNew;
	m_nBufferLen = m_nLineBytes*((nuWORD)bmpInfoHead.biHeight);//m_bitmap->bmpBuffLen   = m_bitmap->bytesPerLine*((nuWORD)bmpInfoHead.biHeight);
	nuBYTE* pBuff = (nuBYTE*)nuMalloc(m_nBufferLen);
	if(pBuff == NULL)
	{
		nuFree(pLine);
		nuFclose(fp);
		return false;
	}
	nuFseek(fp, bmpFileHead.bfOffBits, NURO_SEEK_SET);    
	if(nuFread(pBuff, m_nBufferLen, 1, fp) != 1)
	{
		nuFree(pBuff);
		nuFclose(fp);
		return false;
	}
	if(!arrTag)
	{
		nuINT i=0;
		nuINT iEnd = bmpInfoHead.biHeight >> 1;
		nuBYTE *pBuffTag = pBuff;
		nuBYTE *pBuffTag2 = pBuffTag + (bmpInfoHead.biHeight-1)*DataSizePerLineNew;
		for(i=0; i<iEnd; ++i)
		{
			nuMemcpy(pLine, pBuffTag, DataSizePerLineNew);
			nuMemcpy(pBuffTag, pBuffTag2, DataSizePerLineNew);
			nuMemcpy(pBuffTag2, pLine, DataSizePerLineNew);
			pBuffTag += DataSizePerLineNew;
			pBuffTag2 -= DataSizePerLineNew;
		}
	}
	nuFree(pLine);
	nuFclose(fp);
	m_pBmpBuffer = pBuff;//m_bitmap->pBmpBuff	= pBuff;
	m_nWidth = (nuWORD)bmpInfoHead.biWidth;//m_bitmap->bmpWidth	= (nuWORD)bmpInfoHead.biWidth;
	m_nHeight = (nuWORD)bmpInfoHead.biHeight;//m_bitmap->bmpHeight	= (nuWORD)bmpInfoHead.biHeight;
//	m_bitmap->bmpBufferType = NURO_BMP_BUFFERTYPE_MALLOC;
	m_nColorType = RGB_565;
	m_nBitCount	 = 16;
	return true;
}

void NwBitmap::bitmapFree()
{
	if( NULL != m_pBmpBuffer )
	{
		nuFree(m_pBmpBuffer);
		m_pBmpBuffer = NULL;
	}
}