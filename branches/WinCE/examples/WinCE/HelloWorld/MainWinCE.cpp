#include <windows.h>
//#define USE_GLES

#ifdef USE_GLES
 #include "GLES/gl.h"
 #include "GLES/egl.h"
 #pragma comment(lib,"libEGL.lib")
 #pragma comment(lib,"libGLES_CM_NoE.lib")
#else
 #include "irrlicht.h"
 #pragma comment(lib,"irrlichtCE.lib")

 using namespace irr;
 using namespace core;
 using namespace scene;
 using namespace video;
 using namespace io;
 using namespace gui;
#endif

static LONG WINAPI	MainWndProc	(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	MSG 			msg;
	WNDCLASS		wndclass;
	HWND			hWnd;
	HDC				hDC = 0;


	TCHAR name[] = L"Hello World";
#ifdef USE_GLES
	const EGLint s_configAttribs[] =
	{
		EGL_RED_SIZE,		8,
		EGL_GREEN_SIZE, 	8,
		EGL_BLUE_SIZE,		8,
		EGL_ALPHA_SIZE, 	8,
		EGL_DEPTH_SIZE, 	16,
		EGL_NONE
	};

	EGLDisplay	eglDisplay;
	EGLConfig	eglConfig;
	EGLContext	eglContext;
	EGLSurface	eglWindowSurface;
	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
#else
	IrrlichtDevice *device;
#endif
	BOOL			run;

	wndclass.style		   = 0;
	wndclass.lpfnWndProc   = (WNDPROC)MainWndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance	   = hInstance;
	wndclass.hIcon		   = 0;
	wndclass.hCursor	   = LoadCursor (NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndclass.lpszMenuName  = 0;
	wndclass.lpszClassName = name;

	if (!RegisterClass(&wndclass))
	{
		return FALSE;
	}

	hWnd = CreateWindow( name,
						  name,
						  WS_VISIBLE,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
#ifdef USE_GLES
#ifdef POWERVR
	hDC = GetDC(hWnd);

	eglDisplay = eglGetDisplay(hDC);
#else
	hDC = 0;

	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#endif
	eglInitialize(eglDisplay, &majorVersion, &minorVersion);
	eglGetConfigs(eglDisplay, NULL, 0, &numConfigs);
	eglChooseConfig(eglDisplay, s_configAttribs, &eglConfig, 1, &numConfigs);
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);

	eglWindowSurface = eglCreateWindowSurface(eglDisplay, eglConfig, hWnd, NULL);
	eglMakeCurrent(eglDisplay, eglWindowSurface, eglWindowSurface, eglContext);
#else
	SIrrlichtCreationParameters parameters;
	parameters.WindowSize = core::dimension2d<s32>(240, 320);
	parameters.DriverType = EDT_OPENGL;
	parameters.WindowId = (s32)(hWnd);
	device = createDeviceEx(parameters);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");	
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation ( scene::EMAT_STAND );				
		node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp"));
	}
	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

	guienv->addStaticText(L"Hello World! This is the Irrlicht OpenGL renderer!",
		rect<int>(10,10,240,22), true);
#endif

	run = TRUE;

	while (run)
	{
		static RECT lastrc = {-1, -1, -1, -1};	 
		static RECT rc;

		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//run = FALSE;
			}
		}

		GetClientRect(hWnd, &rc);

		if (rc.right != lastrc.right || rc.bottom != lastrc.bottom)
		{
		
			lastrc = rc;
		}
#ifdef USE_GLES
		glClearColor(1.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		eglSwapBuffers(eglDisplay, eglWindowSurface);
#else
		if(device->run()){
			driver->beginScene(true, true, SColor(255,100,101,140));

			//smgr->drawAll();
			guienv->drawAll();

			driver->endScene();
		}
#endif
	}

	
#ifdef USE_GLES
	eglMakeCurrent(eglDisplay, NULL, NULL, NULL);
	eglDestroyContext(eglDisplay, eglContext);
	eglDestroySurface(eglDisplay, eglWindowSurface);
	eglTerminate(eglDisplay);
#else
	device->drop();
#endif
	if (hDC)
	{
		ReleaseDC(hWnd, hDC);
	}

	DestroyWindow (hWnd);

	return 0;
}

static LONG WINAPI MainWndProc(HWND	hWnd, UINT uMsg, WPARAM	wParam, LPARAM lParam)
{
	LONG			lRet = 1;

	switch (uMsg)
	{
		case WM_CLOSE:
			{
				PostQuitMessage(0);
			}
			break;

		case WM_KEYDOWN:
			{
				if (wParam == VK_ESCAPE)
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);
				}
			}

		default:
			{
				lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
			break;
	}

	return lRet;
}
