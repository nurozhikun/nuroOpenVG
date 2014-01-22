#ifndef __N_LOAD_VG_MODULE_H
#define __N_LOAD_VG_MODULE_H

#include "nuOpenVG.h"

#define _USE_VGMODULE_DLL

#ifdef _USE_VGMODULE_DLL
typedef void* (*vgmodule_initProc)(void*);
typedef void (*vgmodule_freeProc)();
#else
extern "C" void* vgMdl_init(void* pParam);
extern "C" vgMdl_free();
#endif

using namespace MYOPENVG;

class NLoadVgModule
{
public:
	NLoadVgModule(void);
	~NLoadVgModule(void);

	NIOpenVgApi* initVgModule(VG_MODULE_INIT* pParam);
	void		 freeVgModule();

protected:
	nuHINSTANCE	m_hInst;
};

#endif
