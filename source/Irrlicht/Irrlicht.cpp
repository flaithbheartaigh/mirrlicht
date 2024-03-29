// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"

#ifdef _IRR_WINDOWS_
	#include <windows.h>
	#if defined(_DEBUG) && !defined(__GNUWIN32__)
		#include <crtdbg.h>
	#endif // _DEBUG
#endif

#ifdef _IRR_XBOX_PLATFORM_
	#include <xtl.h>
#endif

#include "irrlicht.h"

#if defined(__SYMBIAN32__) && defined(__DLL__)
#ifdef IRRLICHT_API
#undef IRRLICHT_API
#endif
#define IRRLICHT_API EXPORT_C
#endif 

namespace irr
{

	//! stub for calling createDeviceEx
	IRRLICHT_API IrrlichtDevice* IRRCALLCONV createDevice(video::E_DRIVER_TYPE driverType,
			const core::dimension2d<s32>& windowSize,
			u32 bits, bool fullscreen, 
			bool stencilbuffer, bool vsync, IEventReceiver* res,
			const char* version)
	{
		irr::SIrrlichtCreationParameters p;
		p.DriverType = driverType;
		p.WindowSize = windowSize;
		p.Bits = bits;
		p.Fullscreen = fullscreen;
		p.Stencilbuffer = stencilbuffer;
		p.Vsync = vsync;
		p.EventReceiver = res;	
		p.SDK_version_do_not_use = version;

		return createDeviceEx(p);
	}

} // end namespace irr


#if defined(_IRR_WINDOWS_API_)

#ifdef _MSC_VER
#pragma comment(exestr, "Irrlicht Engine (c) 2002-2007 Nikolaus Gebhardt")
#endif

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
	// _crtBreakAlloc = 139;

    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			#if defined(_DEBUG) && !defined(__GNUWIN32__)
				_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
			#endif
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

#endif // defined(_IRR_WINDOWS_)

