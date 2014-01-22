#include "NwOpenVG.h"

NwOpenVG* g_pOpenVg = NULL;

#ifdef _USRDLL
#ifdef VGMODULE_EXPORTS
#define VGMODULE_API extern "C" __declspec(dllexport)
#else
#define VGMODULE_API extern "C" __declspec(dllimport)
#endif
#else
#define VGMODULE_API 
#endif

VGMODULE_API void* vgMdl_init(void* pParam)
{
	if( NULL == pParam )
	{
		return NULL;
	}
	if( NULL == g_pOpenVg )
	{
		VG_MODULE_INIT* pInit = (VG_MODULE_INIT*)pParam;
		g_pOpenVg = new NwOpenVG();
		if( NULL == g_pOpenVg || !g_pOpenVg->initOpenVG(pInit->display, pInit->wnd, pInit->width, pInit->height, pInit->sTtfFile) )
		{
			return NULL;
		}
	}
	return g_pOpenVg;
}

VGMODULE_API void vgMdl_free()
{
	if( NULL != g_pOpenVg )
	{
		g_pOpenVg->freeOpenVG();
		delete g_pOpenVg;
		g_pOpenVg = NULL;
	}
}