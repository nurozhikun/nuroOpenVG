#include "NLoadVgModule.h"

NLoadVgModule::NLoadVgModule(void)
{
	m_hInst = NULL;
}

NLoadVgModule::~NLoadVgModule(void)
{
	freeVgModule();
}

NIOpenVgApi* NLoadVgModule::initVgModule(VG_MODULE_INIT* pParam)
{
#ifdef _USE_VGMODULE_DLL
	TCHAR sFile[NURO_MAX_PATH];
	nuGetModuleFileName(NULL, sFile, NURO_MAX_PATH);
	for(int i = nuTcslen(sFile) - 1; i >= 0; i--)
	{
		if( sFile[i] == '\\' )
		{
			sFile[i+1] = '\0';
			break;
		}
	}
	nuTcscat(sFile, nuTEXT("Setting\\vgModule.dll"));
	m_hInst = nuLoadLibrary(sFile);
	if( m_hInst == NULL )
	{
		return NULL;
	}
	vgmodule_initProc vgMdl_init = (vgmodule_initProc)nuGetProcAddress(m_hInst, nuTEXT("vgMdl_init"));
	if( vgMdl_init == NULL )
	{
		return NULL;
	}
	return (NIOpenVgApi*)vgMdl_init(pParam);
#else
	m_hInst = (nuHINSTANCE)(-1);
	return vgMdl_init(pParam);
#endif
}

void NLoadVgModule::freeVgModule()
{
#ifdef _USE_VGMODULE_DLL
	if( m_hInst != NULL )
	{
		vgmodule_freeProc vgMdl_free = (vgmodule_freeProc)nuGetProcAddress(m_hInst, nuTEXT("vgMdl_free"));
		if( vgMdl_free != NULL )
		{
			vgMdl_free();
		}
		nuFreeLibrary(m_hInst);
	}
#else
	if ( m_hInst != NULL )
	{
		vgMdl_free();
	}
#endif
	m_hInst = NULL;
}