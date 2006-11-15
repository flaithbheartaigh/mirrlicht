// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CIrrDeviceSymbian.h"

#ifdef __SYMBIAN32__

#include <stdio.h>
#include <stdlib.h>
#include "IEventReceiver.h"
#include "irrList.h"
#include "os.h"
#include "CTimer.h"
#include "irrString.h"
#include "Keycodes.h"
#include "COSOperator.h"
#include "CColorConverter.h"
#include "irrlicht.h"

namespace irr
{
	namespace video
	{
		IVideoDriver* createOpenGLDriver(const core::dimension2d<s32>& screenSize,
			EGLSurface window, EGLDisplay display, bool fullscreen,
			bool stencilBuffer, io::IFileSystem* io, bool vsync, bool antiAlias);
	}


} // end namespace irr



namespace irr
{

//! constructor
CIrrDeviceSymbian::CIrrDeviceSymbian(video::E_DRIVER_TYPE driverType,
	const core::dimension2d<s32>& windowSize,
	u32 bits, bool fullscreen,
	bool sbuffer, bool vsync, bool antiAlias,
	IEventReceiver* receiver,
	RWindow* externalWindow,
	const char* version)
 : CIrrDeviceStub(version, receiver), close(false), DriverType(driverType),
	Fullscreen(fullscreen), StencilBuffer(sbuffer), pWindow(externalWindow)
{
	#ifdef _DEBUG
	setDebugName("CIrrDeviceSymbian");
	#endif

	// print version, distribution etc.
	core::stringc symbianversion("Symbian ");
	//TODO get operating system information of Symbian
/**
	// thx to LynxLuna for pointing me to the uname function
	utsname LinuxInfo; 
	uname(&LinuxInfo);

	linuxversion += LinuxInfo.sysname;
	linuxversion += " ";
	linuxversion += LinuxInfo.release;
	linuxversion += " ";
	linuxversion += LinuxInfo.version;
*/
	Operator = new COSOperator(symbianversion.c_str());
	os::Printer::log(symbianversion.c_str(), ELL_INFORMATION);

	// create keymap
	createKeyMap();

	// create window

	if (driverType != video::EDT_NULL)
	{
		// create the window, only if we do not use the null device
		if (!createWindow(windowSize, bits))
			return;
	}

	// create cursor control

	//CursorControl = new CCursorControl(this, driverType == video::EDT_NULL);
	CursorControl = NULL;
	// create driver

	createDriver(driverType, windowSize, bits, vsync, antiAlias);

	if (!VideoDriver)
		return;

	createGUIAndScene();
}



//! destructor
CIrrDeviceSymbian::~CIrrDeviceSymbian()
{
	if (display)
	{
		//os::Printer::log("Deleting window...", ELL_INFORMATION);

		#ifdef _IRR_COMPILE_WITH_OPENGL_
		if (Context)
		{			
			if (!eglMakeCurrent(display, NULL, NULL, NULL))
				os::Printer::log("Could not release opengl es context.", ELL_WARNING);
			eglDestroyContext(display, Context);
			eglDestroySurface(display, eglWindowSurface);
			eglTerminate(display);	
			Context = 0;
		}
		#endif // #ifdef _IRR_COMPILE_WITH_OPENGL_		
		
	}
}


bool CIrrDeviceSymbian::createWindow(const core::dimension2d<s32>& windowSize,
						  u32 bits)
{
	Width = windowSize.Width;
	Height = windowSize.Height;

#ifdef _DEBUG
	os::Printer::log("Creating EGL window...", ELL_INFORMATION);	
#endif

	static const EGLint s_configAttribs[] =
	{
		EGL_RED_SIZE,		8,
			EGL_GREEN_SIZE, 	8,
			EGL_BLUE_SIZE,		8,
			EGL_ALPHA_SIZE, 	8,
			EGL_DEPTH_SIZE, 	16,
			EGL_NONE
	};	

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLConfig	eglConfig;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, &majorVersion, &minorVersion);
	eglGetConfigs(display, NULL, 0, &numConfigs);
	eglChooseConfig(display, s_configAttribs, &eglConfig, 1, &numConfigs);
	Context = eglCreateContext(display, eglConfig, NULL, NULL);

	eglWindowSurface = eglCreateWindowSurface(display, eglConfig, pWindow, NULL);
	eglMakeCurrent(display, eglWindowSurface, eglWindowSurface, Context);
	
	if (!display)
	{
		os::Printer::log("Error: Need running XServer to start Irrlicht Engine.", ELL_ERROR);
		return false;
	}
	

	// query extension

	if (Fullscreen)
	{
		//SetExtentToWholeScreen();
	}

	return true;
}


//! create the driver
void CIrrDeviceSymbian::createDriver(video::E_DRIVER_TYPE driverType,
				   const core::dimension2d<s32>& windowSize,
				   u32 bits, bool vsync, bool antiAlias)
{
	switch(driverType)
	{

	case video::EDT_OPENGL:
		if (Context)
			VideoDriver = video::createOpenGLDriver(windowSize, eglWindowSurface, display, Fullscreen, StencilBuffer, FileSystem, vsync, antiAlias);
		break;

	case video::EDT_NULL:
		VideoDriver = video::createNullDriver(FileSystem, windowSize);
		break;

	default:
		os::Printer::log("This driver is not available in Linux. Try OpenGL or Software renderer.",
			ELL_WARNING);
	}
}



//! runs the device. Returns false if device wants to be deleted
bool CIrrDeviceSymbian::run()
{
	os::Timer::tick();
	/*
	if (DriverType != video::EDT_NULL)
	{
		irr::SEvent irrevent;

		while (XPending(display) > 0 && !close)
		{
			XNextEvent(display, &event);

			switch (event.type)
			{
			case ConfigureNotify:
				// check for changed window size
				if ((event.xconfigure.width != Width) ||
					(event.xconfigure.height != Height))
				{
					Width = event.xconfigure.width;
					Height = event.xconfigure.height;
					//os::Printer::log("TODO: resize should be disabled.", ELL_INFORMATION);
					if (VideoDriver)
						VideoDriver->OnResize(core::dimension2d<s32>(Width, Height));
				}
				break;

			case MotionNotify:
				irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
				irrevent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
				irrevent.MouseInput.X = event.xbutton.x;
				irrevent.MouseInput.Y = event.xbutton.y;

				postEventFromUser(irrevent);
				break;

			case ButtonPress:
			case ButtonRelease:

				irrevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
				irrevent.MouseInput.X = event.xbutton.x;
				irrevent.MouseInput.Y = event.xbutton.y;

				irrevent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;

				switch(event.xbutton.button)
				{
				case  Button1:
					irrevent.MouseInput.Event =
						(event.type == ButtonPress) ? irr::EMIE_LMOUSE_PRESSED_DOWN : irr::EMIE_LMOUSE_LEFT_UP;
					break;

				case  Button3:
					irrevent.MouseInput.Event =
						(event.type == ButtonPress) ? irr::EMIE_RMOUSE_PRESSED_DOWN : irr::EMIE_RMOUSE_LEFT_UP;
					break;

				case  Button2:
					irrevent.MouseInput.Event =
						(event.type == ButtonPress) ? irr::EMIE_MMOUSE_PRESSED_DOWN : irr::EMIE_MMOUSE_LEFT_UP;
					break;

				case  Button4:
					irrevent.MouseInput.Event = EMIE_MOUSE_WHEEL;
					irrevent.MouseInput.Wheel = 1.0f;
					break;

				case  Button5:
					irrevent.MouseInput.Event = EMIE_MOUSE_WHEEL;
					irrevent.MouseInput.Wheel = -1.0f;
					break;
				}

				if (irrevent.MouseInput.Event != irr::EMIE_MOUSE_MOVED)
					postEventFromUser(irrevent);
				break;

			case MappingNotify:
				XRefreshKeyboardMapping (&event.xmapping) ;
				break;

			case KeyRelease:
			case KeyPress:
				{
					SKeyMap mp;
					//mp.X11Key = XLookupKeysym(&event.xkey, 0);
					char buf[5]="\0\0\0\0";
					XLookupString (&event.xkey, buf, 4, &mp.X11Key, NULL) ;

					s32 idx = KeyMap.binary_search(mp);

					if (idx != -1)
						irrevent.KeyInput.Key = (EKEY_CODE)KeyMap[idx].Win32Key;
					else
						os::Printer::log("Could not find win32 key for x11 key.", ELL_WARNING);
					irrevent.EventType = irr::EET_KEY_INPUT_EVENT;
					irrevent.KeyInput.PressedDown = (event.type == KeyPress);
					mbtowc(&irrevent.KeyInput.Char, buf, 4);
					irrevent.KeyInput.Control = (event.xkey.state & ControlMask) != 0;
					irrevent.KeyInput.Shift = (event.xkey.state & ShiftMask) != 0;
					postEventFromUser(irrevent);
				}
				break;

			case ClientMessage:
				if (*XGetAtomName(display, event.xclient.message_type) ==
					*wmDeleteWindow)
				{
					os::Printer::log("Quit message received.", ELL_INFORMATION);
					close = true;
				}
				break;

			default:
				break;
			} // end switch

		} // end while
	}

	return !close;
	*/
	return true;
}



//! sets the caption of the window
void CIrrDeviceSymbian::setWindowCaption(const wchar_t* text)
{
	if (DriverType == video::EDT_NULL)
		return;

	core::stringc textc = text;
	//XSetStandardProperties(display, window, textc.c_str(), textc.c_str(),
	//		       None, NULL, 0, NULL);
}



//! presents a surface in the client area
void CIrrDeviceSymbian::present(video::IImage* image, s32 windowId, core::rect<s32>* src )
{
	// thx to Nadav, who send me some clues of how to display the image 
	// to the X Server.
/*
	if (image->getColorFormat() != video::ECF_A1R5G5B5 &&
	    image->getColorFormat() != video::ECF_A8R8G8B8)
	{
		os::Printer::log("Internal error, can only present A1R5G5B5 and A8R8G8B8 pictures.");
		return;
	}
	
	int destwidth = SoftwareImage->width;
	int destheight = SoftwareImage->height;
	int srcwidth = image->getDimension().Width;
	int srcheight = image->getDimension().Height;
	
	if ( image->getColorFormat() == video::ECF_A8R8G8B8 )
	{
		// display 32 bit image
		
		s32* srcdata = (s32*)image->lock();
		
		if (Depth == 32)
		{	
			int destPitch = SoftwareImage->bytes_per_line / 4;
			s32* destData = reinterpret_cast<s32*>(SoftwareImage->data);
			
			for (int y=0; y<srcheight; ++y)
				for (int x=0; x<srcwidth; ++x)
					if (x < destwidth && y < destheight)
						destData[y*destPitch + x] = srcdata[y*srcwidth+x];
		}
		else
		if (Depth == 16)
		{
			// convert from A1R5G5B5 to R5G6B6
			
			int destPitch = SoftwareImage->bytes_per_line / 2;
			s16* destData = reinterpret_cast<s16*>(SoftwareImage->data);
			
			for (int x=0; x<srcwidth; ++x)
				for (int y=0; y<srcheight; ++y)
					if (x < destwidth && y < destheight)
					{
						s16 c = video::X8R8G8B8toA1R5G5B5(srcdata[y*srcwidth+x]);
						destData[y*destPitch + x] = 
							((c >> 10)&0x1F)<<11 |
							((c >> 5)&0x1F) << 6 |
							((c)&0x1F);
					}
		}
		else
		if (Depth == 24)
		{
			// convert from A1R5G5B5 to X8R8G8B8
	
			int destPitch = SoftwareImage->bytes_per_line / 4;
			s32* destData = reinterpret_cast<s32*>(SoftwareImage->data);
			
			for (int y=0; y<srcheight; ++y)
				for (int x=0; x<srcwidth; ++x)
					if (x < destwidth && y < destheight)
						destData[y*destPitch + x] = srcdata[y*srcwidth+x];
		}
		else
		{
			os::Printer::log("Unsupported screen depth.");
		}
		
		image->unlock();
	}
	else
	{
	
		// display 16 bit image:
	
		s16* srcdata = (s16*)image->lock();
	
		if (Depth == 16)
		{
			// convert from A1R5G5B5 to R5G6B6
			
			int destPitch = SoftwareImage->bytes_per_line / 2;
			s16* destData = reinterpret_cast<s16*>(SoftwareImage->data);
			
			for (int x=0; x<srcwidth; ++x)
				for (int y=0; y<srcheight; ++y)
					if (x < destwidth && y < destheight)
					{
						s16 c = srcdata[y*srcwidth+x];
						destData[y*destPitch + x] = 
							((c >> 10)&0x1F)<<11 |
							((c >> 5)&0x1F) << 6 |
							((c)&0x1F);
					}
		}
		else
		if (Depth == 32)
		{
			// convert from A1R5G5B5 to X8R8G8B8
				
			int destPitch = SoftwareImage->bytes_per_line / 4;
			s32* destData = reinterpret_cast<s32*>(SoftwareImage->data);
			
			for (int y=0; y<srcheight; ++y)
				for (int x=0; x<srcwidth; ++x)
					if (x < destwidth && y < destheight)
						destData[y*destPitch + x] = 
						video::A1R5G5B5toA8R8G8B8(srcdata[y*srcwidth+x]);	
		}
		else
		if (Depth == 24)
		{
			// convert from A1R5G5B5 to X8R8G8B8
	
			int destPitch = SoftwareImage->bytes_per_line / 4;
			s32* destData = reinterpret_cast<s32*>(SoftwareImage->data);
			
			for (int y=0; y<srcheight; ++y)
				for (int x=0; x<srcwidth; ++x)
					if (x < destwidth && y < destheight)
						destData[y*destPitch + x] = 
						video::A1R5G5B5toA8R8G8B8(srcdata[y*srcwidth+x]);
		}
		else
		{
			os::Printer::log("Unsupported screen depth.");
		}
	
		image->unlock();
	}

	GC gc = DefaultGC(display, DefaultScreen(display));
	XPutImage(display, eglWindowSurface, gc, SoftwareImage, 0, 0, 0, 0, destwidth, destheight);
*/
}



//! notifies the device that it should close itself
void CIrrDeviceSymbian::closeDevice()
{
	close = true;
}



//! returns if window is active. if not, nothing need to be drawn
bool CIrrDeviceSymbian::isWindowActive()
{
	return true; // TODO
}


void CIrrDeviceSymbian::createKeyMap()
{
	// I don't know if this is the best method  to create
	// the lookuptable, but I'll leave it like that until
	// I find a better version.
/*
	KeyMap.push_back(SKeyMap(XK_BackSpace, KEY_BACK));
	KeyMap.push_back(SKeyMap(XK_Tab, KEY_TAB));
	KeyMap.push_back(SKeyMap(XK_Linefeed, 0)); // ???
	KeyMap.push_back(SKeyMap(XK_Clear, KEY_CLEAR));
	KeyMap.push_back(SKeyMap(XK_Return, KEY_RETURN));
	KeyMap.push_back(SKeyMap(XK_Pause, KEY_PAUSE));
	KeyMap.push_back(SKeyMap(XK_Scroll_Lock, KEY_SCROLL));
	KeyMap.push_back(SKeyMap(XK_Sys_Req, 0)); // ???
	KeyMap.push_back(SKeyMap(XK_Escape, KEY_ESCAPE));
	KeyMap.push_back(SKeyMap(XK_Delete, KEY_DELETE));
	KeyMap.push_back(SKeyMap(XK_Home, KEY_HOME));
	KeyMap.push_back(SKeyMap(XK_Left, KEY_LEFT));
	KeyMap.push_back(SKeyMap(XK_Up, KEY_UP));
	KeyMap.push_back(SKeyMap(XK_Right, KEY_RIGHT));
	KeyMap.push_back(SKeyMap(XK_Down, KEY_DOWN));
	KeyMap.push_back(SKeyMap(XK_Prior, KEY_PRIOR));
	KeyMap.push_back(SKeyMap(XK_Page_Up, KEY_PRIOR));
	KeyMap.push_back(SKeyMap(XK_Next, KEY_NEXT));
	KeyMap.push_back(SKeyMap(XK_Page_Down, KEY_NEXT));
	KeyMap.push_back(SKeyMap(XK_End, KEY_END));
	KeyMap.push_back(SKeyMap(XK_Begin, KEY_HOME));
	KeyMap.push_back(SKeyMap(XK_KP_Space, KEY_SPACE));
	KeyMap.push_back(SKeyMap(XK_KP_Tab, KEY_TAB));
	KeyMap.push_back(SKeyMap(XK_KP_Enter, KEY_RETURN));
	KeyMap.push_back(SKeyMap(XK_KP_F1, KEY_F1));
	KeyMap.push_back(SKeyMap(XK_KP_F2, KEY_F2));
	KeyMap.push_back(SKeyMap(XK_KP_F3, KEY_F3));
	KeyMap.push_back(SKeyMap(XK_KP_F4, KEY_F4));
	KeyMap.push_back(SKeyMap(XK_KP_Left, KEY_LEFT));
	KeyMap.push_back(SKeyMap(XK_KP_Up, KEY_UP));
	KeyMap.push_back(SKeyMap(XK_KP_Right, KEY_RIGHT));
	KeyMap.push_back(SKeyMap(XK_KP_Down, KEY_DOWN));
	KeyMap.push_back(SKeyMap(XK_KP_Prior, KEY_PRIOR));
	KeyMap.push_back(SKeyMap(XK_KP_Page_Up, KEY_PRIOR));
	KeyMap.push_back(SKeyMap(XK_KP_Next, KEY_NEXT));
	KeyMap.push_back(SKeyMap(XK_KP_Page_Down, KEY_NEXT));
	KeyMap.push_back(SKeyMap(XK_KP_End, KEY_END));
	KeyMap.push_back(SKeyMap(XK_KP_Begin, KEY_HOME));
	KeyMap.push_back(SKeyMap(XK_KP_Insert, KEY_INSERT));
	KeyMap.push_back(SKeyMap(XK_KP_Delete, KEY_DELETE));
	KeyMap.push_back(SKeyMap(XK_KP_Equal, 0)); // ???
	KeyMap.push_back(SKeyMap(XK_KP_Multiply, KEY_MULTIPLY));
	KeyMap.push_back(SKeyMap(XK_KP_Add, KEY_ADD));
	KeyMap.push_back(SKeyMap(XK_KP_Separator, KEY_SEPARATOR));
	KeyMap.push_back(SKeyMap(XK_KP_Subtract, KEY_SUBTRACT));
	KeyMap.push_back(SKeyMap(XK_KP_Decimal, KEY_DECIMAL));
	KeyMap.push_back(SKeyMap(XK_KP_Divide, KEY_DIVIDE));
	KeyMap.push_back(SKeyMap(XK_KP_0, KEY_KEY_0));
	KeyMap.push_back(SKeyMap(XK_KP_1, KEY_KEY_1));
	KeyMap.push_back(SKeyMap(XK_KP_2, KEY_KEY_2));
	KeyMap.push_back(SKeyMap(XK_KP_3, KEY_KEY_3));
	KeyMap.push_back(SKeyMap(XK_KP_4, KEY_KEY_4));
	KeyMap.push_back(SKeyMap(XK_KP_5, KEY_KEY_5));
	KeyMap.push_back(SKeyMap(XK_KP_6, KEY_KEY_6));
	KeyMap.push_back(SKeyMap(XK_KP_7, KEY_KEY_7));
	KeyMap.push_back(SKeyMap(XK_KP_8, KEY_KEY_8));
	KeyMap.push_back(SKeyMap(XK_KP_9, KEY_KEY_9));
	KeyMap.push_back(SKeyMap(XK_F1, KEY_F1));
	KeyMap.push_back(SKeyMap(XK_F2, KEY_F2));
	KeyMap.push_back(SKeyMap(XK_F3, KEY_F3));
	KeyMap.push_back(SKeyMap(XK_F4, KEY_F4));
	KeyMap.push_back(SKeyMap(XK_F5, KEY_F5));
	KeyMap.push_back(SKeyMap(XK_F6, KEY_F6));
	KeyMap.push_back(SKeyMap(XK_F7, KEY_F7));
	KeyMap.push_back(SKeyMap(XK_F8, KEY_F8));
	KeyMap.push_back(SKeyMap(XK_F9, KEY_F9));
	KeyMap.push_back(SKeyMap(XK_F10, KEY_F10));
	KeyMap.push_back(SKeyMap(XK_F11, KEY_F11));
	KeyMap.push_back(SKeyMap(XK_F12, KEY_F12));
	KeyMap.push_back(SKeyMap(XK_Shift_L, KEY_LSHIFT));
	KeyMap.push_back(SKeyMap(XK_Shift_R, KEY_RSHIFT));
	KeyMap.push_back(SKeyMap(XK_Control_L, KEY_LCONTROL));
	KeyMap.push_back(SKeyMap(XK_Control_R, KEY_RCONTROL));
	KeyMap.push_back(SKeyMap(XK_Caps_Lock, KEY_CAPITAL));
	KeyMap.push_back(SKeyMap(XK_Shift_Lock, KEY_CAPITAL));
	KeyMap.push_back(SKeyMap(XK_Meta_L, KEY_LWIN));
	KeyMap.push_back(SKeyMap(XK_Meta_R, KEY_RWIN));
	KeyMap.push_back(SKeyMap(XK_Alt_L, KEY_LMENU));
	KeyMap.push_back(SKeyMap(XK_Alt_R, KEY_RMENU));
	KeyMap.push_back(SKeyMap(XK_ISO_Level3_Shift, KEY_RMENU));
	KeyMap.push_back(SKeyMap(XK_Menu, KEY_MENU));
	KeyMap.push_back(SKeyMap(XK_space, KEY_SPACE));
	KeyMap.push_back(SKeyMap(XK_exclam, 0)); //?
	KeyMap.push_back(SKeyMap(XK_quotedbl, 0)); //?
	KeyMap.push_back(SKeyMap(XK_section, 0)); //?
	KeyMap.push_back(SKeyMap(XK_numbersign, 0)); //?
	KeyMap.push_back(SKeyMap(XK_dollar, 0)); //?
	KeyMap.push_back(SKeyMap(XK_percent, 0)); //?
	KeyMap.push_back(SKeyMap(XK_ampersand, 0)); //?
	KeyMap.push_back(SKeyMap(XK_apostrophe, 0)); //?
	KeyMap.push_back(SKeyMap(XK_parenleft, 0)); //?
	KeyMap.push_back(SKeyMap(XK_parenright, 0)); //?
	KeyMap.push_back(SKeyMap(XK_asterisk, 0)); //?
	KeyMap.push_back(SKeyMap(XK_plus, KEY_PLUS)); //?
	KeyMap.push_back(SKeyMap(XK_comma, KEY_COMMA)); //?
	KeyMap.push_back(SKeyMap(XK_minus, KEY_MINUS)); //?
	KeyMap.push_back(SKeyMap(XK_period, KEY_PERIOD)); //? 
	KeyMap.push_back(SKeyMap(XK_slash, 0)); //?
	KeyMap.push_back(SKeyMap(XK_0, KEY_KEY_0));
	KeyMap.push_back(SKeyMap(XK_1, KEY_KEY_1));
	KeyMap.push_back(SKeyMap(XK_2, KEY_KEY_2));
	KeyMap.push_back(SKeyMap(XK_3, KEY_KEY_3));
	KeyMap.push_back(SKeyMap(XK_4, KEY_KEY_4));
	KeyMap.push_back(SKeyMap(XK_5, KEY_KEY_5));
	KeyMap.push_back(SKeyMap(XK_6, KEY_KEY_6));
	KeyMap.push_back(SKeyMap(XK_7, KEY_KEY_7));
	KeyMap.push_back(SKeyMap(XK_8, KEY_KEY_8));
	KeyMap.push_back(SKeyMap(XK_9, KEY_KEY_9));
	KeyMap.push_back(SKeyMap(XK_colon, 0)); //?
	KeyMap.push_back(SKeyMap(XK_semicolon, 0)); //?
	KeyMap.push_back(SKeyMap(XK_less, 0)); //?
	KeyMap.push_back(SKeyMap(XK_equal, 0)); //?
	KeyMap.push_back(SKeyMap(XK_greater, 0)); //?
	KeyMap.push_back(SKeyMap(XK_question, 0)); //?
	KeyMap.push_back(SKeyMap(XK_at, 0)); //?
	KeyMap.push_back(SKeyMap(XK_mu, 0)); //?
	KeyMap.push_back(SKeyMap(XK_EuroSign, 0)); //?
	KeyMap.push_back(SKeyMap(XK_A, KEY_KEY_A));
	KeyMap.push_back(SKeyMap(XK_B, KEY_KEY_B));
	KeyMap.push_back(SKeyMap(XK_C, KEY_KEY_C));
	KeyMap.push_back(SKeyMap(XK_D, KEY_KEY_D));
	KeyMap.push_back(SKeyMap(XK_E, KEY_KEY_E));
	KeyMap.push_back(SKeyMap(XK_F, KEY_KEY_F));
	KeyMap.push_back(SKeyMap(XK_G, KEY_KEY_G));
	KeyMap.push_back(SKeyMap(XK_H, KEY_KEY_H));
	KeyMap.push_back(SKeyMap(XK_I, KEY_KEY_I));
	KeyMap.push_back(SKeyMap(XK_J, KEY_KEY_J));
	KeyMap.push_back(SKeyMap(XK_K, KEY_KEY_K));
	KeyMap.push_back(SKeyMap(XK_L, KEY_KEY_L));
	KeyMap.push_back(SKeyMap(XK_M, KEY_KEY_M));
	KeyMap.push_back(SKeyMap(XK_N, KEY_KEY_N));
	KeyMap.push_back(SKeyMap(XK_O, KEY_KEY_O));
	KeyMap.push_back(SKeyMap(XK_P, KEY_KEY_P));
	KeyMap.push_back(SKeyMap(XK_Q, KEY_KEY_Q));
	KeyMap.push_back(SKeyMap(XK_R, KEY_KEY_R));
	KeyMap.push_back(SKeyMap(XK_S, KEY_KEY_S));
	KeyMap.push_back(SKeyMap(XK_T, KEY_KEY_T));
	KeyMap.push_back(SKeyMap(XK_U, KEY_KEY_U));
	KeyMap.push_back(SKeyMap(XK_V, KEY_KEY_V));
	KeyMap.push_back(SKeyMap(XK_W, KEY_KEY_W));
	KeyMap.push_back(SKeyMap(XK_X, KEY_KEY_X));
	KeyMap.push_back(SKeyMap(XK_Y, KEY_KEY_Y));
	KeyMap.push_back(SKeyMap(XK_Z, KEY_KEY_Z));
	KeyMap.push_back(SKeyMap(XK_Adiaeresis, 0)); //?
	KeyMap.push_back(SKeyMap(XK_Odiaeresis, 0)); //?
	KeyMap.push_back(SKeyMap(XK_Udiaeresis, 0)); //?
	KeyMap.push_back(SKeyMap(XK_bracketleft, 0)); //?
	KeyMap.push_back(SKeyMap(XK_backslash, 0)); //?
	KeyMap.push_back(SKeyMap(XK_bracketright, 0)); //?
	KeyMap.push_back(SKeyMap(XK_asciicircum, 0)); //?
	KeyMap.push_back(SKeyMap(XK_degree, 0)); //?
	KeyMap.push_back(SKeyMap(XK_underscore, 0)); //?
	KeyMap.push_back(SKeyMap(XK_grave, 0)); //?
	KeyMap.push_back(SKeyMap(XK_acute, 0)); //?
	KeyMap.push_back(SKeyMap(XK_quoteleft, 0)); //?
	KeyMap.push_back(SKeyMap(XK_a, KEY_KEY_A));
	KeyMap.push_back(SKeyMap(XK_b, KEY_KEY_B));
	KeyMap.push_back(SKeyMap(XK_c, KEY_KEY_C));
	KeyMap.push_back(SKeyMap(XK_d, KEY_KEY_D));
	KeyMap.push_back(SKeyMap(XK_e, KEY_KEY_E));
	KeyMap.push_back(SKeyMap(XK_f, KEY_KEY_F));
	KeyMap.push_back(SKeyMap(XK_g, KEY_KEY_G));
	KeyMap.push_back(SKeyMap(XK_h, KEY_KEY_H));
	KeyMap.push_back(SKeyMap(XK_i, KEY_KEY_I));
	KeyMap.push_back(SKeyMap(XK_j, KEY_KEY_J));
	KeyMap.push_back(SKeyMap(XK_k, KEY_KEY_K));
	KeyMap.push_back(SKeyMap(XK_l, KEY_KEY_L));
	KeyMap.push_back(SKeyMap(XK_m, KEY_KEY_M));
	KeyMap.push_back(SKeyMap(XK_n, KEY_KEY_N));
	KeyMap.push_back(SKeyMap(XK_o, KEY_KEY_O));
	KeyMap.push_back(SKeyMap(XK_p, KEY_KEY_P));
	KeyMap.push_back(SKeyMap(XK_q, KEY_KEY_Q));
	KeyMap.push_back(SKeyMap(XK_r, KEY_KEY_R));
	KeyMap.push_back(SKeyMap(XK_s, KEY_KEY_S));
	KeyMap.push_back(SKeyMap(XK_t, KEY_KEY_T));
	KeyMap.push_back(SKeyMap(XK_u, KEY_KEY_U));
	KeyMap.push_back(SKeyMap(XK_v, KEY_KEY_V));
	KeyMap.push_back(SKeyMap(XK_w, KEY_KEY_W));
	KeyMap.push_back(SKeyMap(XK_x, KEY_KEY_X));
	KeyMap.push_back(SKeyMap(XK_y, KEY_KEY_Y));
	KeyMap.push_back(SKeyMap(XK_z, KEY_KEY_Z));
	KeyMap.push_back(SKeyMap(XK_ssharp, 0)); //?
	KeyMap.push_back(SKeyMap(XK_adiaeresis, 0)); //?
	KeyMap.push_back(SKeyMap(XK_odiaeresis, 0)); //?
	KeyMap.push_back(SKeyMap(XK_udiaeresis, 0)); //?

	KeyMap.sort();
*/
}



#if defined(__SYMBIAN32__) && defined(__DLL__)
#ifdef IRRLICHT_API
#undef IRRLICHT_API
#endif
#define IRRLICHT_API EXPORT_C
#endif 

IRRLICHT_API IrrlichtDevice* IRRCALLCONV createDeviceEx(const SIrrlichtCreationParameters& param)
{
	CIrrDeviceSymbian* dev = new CIrrDeviceSymbian(
		param.DriverType,
		param.WindowSize,
		param.Bits,
		param.Fullscreen,
		param.Stencilbuffer,
		param.Vsync,
		param.AntiAlias,
		param.EventReceiver,
		reinterpret_cast<RWindow*>(param.WindowId),
		param.SDK_version_do_not_use);

	if (dev && !dev->getVideoDriver() && param.DriverType != video::EDT_NULL)
	{
		dev->drop();
		dev = 0;
	}

	return dev;
}

} // end namespace

#endif // LINUX

