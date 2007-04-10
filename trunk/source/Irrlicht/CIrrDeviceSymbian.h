// Copyright (C) 2002-2006 Nikolaus Gebhardt. 2006 Ming Li.
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_IRR_DEVICE_SYMBIAN_H_INCLUDED__
#define __C_IRR_DEVICE_SYMBIAN_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef __SYMBIAN32__

#include "CIrrDeviceStub.h"
#include "IrrlichtDevice.h"
#include "IImagePresenter.h"
#include "IGUIEnvironment.h"
#include "ICursorControl.h"

#include <w32std.h> //RWindow: Windows on Symbian system
#include <GLES/egl.h>
#include "glext.h"

namespace irr
{

	class CIrrDeviceSymbian : public CIrrDeviceStub, public video::IImagePresenter
	{
	public:

		//! constructor
		CIrrDeviceSymbian(video::E_DRIVER_TYPE deviceType, 
			const core::dimension2d<s32>& windowSize, u32 bits,
			bool fullscreen, bool stencilbuffer, bool vsync, bool antiAlias, IEventReceiver* receiver, RWindow* externalWindow,
			const char* version);

		//! destructor
		virtual ~CIrrDeviceSymbian();

		//! runs the device. Returns false if device wants to be deleted
		virtual bool run();

		//! Cause the device to temporarily pause execution and let other processes to run
		// This should bring down processor usage without major performance loss for Irrlicht
		virtual void yield();

		//! Pause execution and let other processes to run for a specified amount of time.
		virtual void sleep(u32 timeMs, bool pauseTimer);

		//! sets the caption of the window
		virtual void setWindowCaption(const wchar_t* text);

		//! returns if window is active. if not, nothing need to be drawn
		virtual bool isWindowActive();

		//! presents a surface in the client area
		virtual void present(video::IImage* surface, s32 windowId = 0, core::rect<s32>* src=0 );

		//! notifies the device that it should close itself
		virtual void closeDevice();

	private:

		//! create the driver
		void createDriver(video::E_DRIVER_TYPE driverType,
			const core::dimension2d<s32>& windowSize, u32 bits,
			bool vsync, bool antiAlias);

		bool createWindow(const core::dimension2d<s32>& windowSize, u32 bits);

		void createKeyMap();
		

		EGLDisplay display;
		int screennr;
		EGLSurface eglWindowSurface;
		EGLContext Context;
		RWindow *pWindow;
		bool Fullscreen;
		bool StencilBuffer;
//		XEvent event;
		video::E_DRIVER_TYPE DriverType;

		int x,y;
		unsigned int Width, Height, Depth;
		bool close;
/*
		struct SKeyMap
		{
			SKeyMap() {}
			SKeyMap(s32 x11, s32 win32)
				: X11Key(x11), Win32Key(win32)
			{
			}

			KeySym X11Key;
			s32 Win32Key;

			bool operator<(const SKeyMap& o) const
			{
				return X11Key<o.X11Key;
			}
		};


		core::array<SKeyMap> KeyMap;
*/
	};


} // end namespace irr

#endif // __SYMBIAN32__
#endif // __C_IRR_DEVICE_SYMBIAN_H_INCLUDED__

