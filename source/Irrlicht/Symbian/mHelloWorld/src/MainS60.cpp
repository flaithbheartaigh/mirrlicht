//
// Include
//

#include <aknapp.h>
#include <aknappui.h>
#include <akndoc.h>
#include <coecntrl.h>
#include <eikstart.h>
#include <pathinfo.h>

#include <GLES/egl.h>

#define USE_IRRLICHT

class CMainS60Application;
class CMainS60AppUi;
class CMainS60AppView;
class CMainS60Document;

#ifdef USE_IRRLICHT
	#include "irrlicht.h"
	using namespace irr;
	using namespace core;
	using namespace scene;
	using namespace video;
	using namespace io;
	using namespace gui;

	static IrrlichtDevice *device;	
#else
	#include "Game.h"
	static EGLDisplay	eglDisplay;
	static EGLConfig	eglConfig;
	static EGLContext	eglContext;
	static EGLSurface	eglWindowSurface;

	static Game game;

#endif
/**
* CMainS60Application application class.
* Provides factory to create concrete document object.
* An instance of CMainS60Application is the application part of the
* AVKON application framework for the MainS60 example application.
*/
class CMainS60Application : public CAknApplication
{
    public: // Functions from base classes

        /**
        * From CApaApplication, AppDllUid.
        * @return Application's UID (KUidMainS60App).
        */
        TUid AppDllUid() const;

    protected: // Functions from base classes

        /**
        * From CApaApplication, CreateDocumentL.
        * Creates CMainS60Document document object. The returned
        * pointer in not owned by the CMainS60Application object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
};

/**
* CMainS60AppUi application UI class.
* Interacts with the user through the UI and request message processing
* from the handler class
*/
class CMainS60AppUi : public CAknAppUi
{
    public: // Constructors and destructor

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL();

        /**
        * CMainS60AppUi.
        * C++ default constructor. This needs to be public due to
        * the way the framework constructs the AppUi
        */
        CMainS60AppUi();

        /**
        * ~CMainS60AppUi.
        * Virtual Destructor.
        */
        virtual ~CMainS60AppUi();

    private:  // Functions from base classes

        /**
        * From CEikAppUi, HandleCommandL.
        * Takes care of command handling.
        * @param aCommand Command to be handled.
        */
        void HandleCommandL( TInt aCommand );

        /**
        *  HandleStatusPaneSizeChange.
        *  Called by the framework when the application status pane
 		*  size is changed.
        */
		void HandleStatusPaneSizeChange();
        
    private: // Data

        /**
        * The application view
        * Owned by CMainS60AppUi
        */
        CMainS60AppView* iAppView;        
};

class CMainS60AppView : public CCoeControl
{
    public: // New methods

        /**
        * NewL.
        * Two-phased constructor.
        * Create a CMainS60AppView object, which will draw itself to aRect.
        * @param aRect The rectangle this view will be drawn to.
        * @return a pointer to the created instance of CMainS60AppView.
        */
        static CMainS60AppView* NewL( const TRect& aRect );

        /**
        * NewLC.
        * Two-phased constructor.
        * Create a CMainS60AppView object, which will draw itself
        * to aRect.
        * @param aRect Rectangle this view will be drawn to.
        * @return A pointer to the created instance of CMainS60AppView.
        */
        static CMainS60AppView* NewLC( const TRect& aRect );

		static TInt Update( TAny* aInstance );

        /**
        * ~CMainS60AppView
        * Virtual Destructor.
        */
        virtual ~CMainS60AppView();

    public:  // Functions from base classes

        /**
        * From CCoeControl, Draw
        * Draw this CMainS60AppView to the screen.
        * @param aRect the rectangle of this view that needs updating
        */
        void Draw( const TRect& aRect ) const;

        /**
        * From CoeControl, SizeChanged.
        * Called by framework when the view size is changed.
        */
        virtual void SizeChanged();

    private: // Constructors

        /**
        * ConstructL
        * 2nd phase constructor.
        * Perform the second phase construction of a
        * CMainS60AppView object.
        * @param aRect The rectangle this view will be drawn to.
        */
        void ConstructL(const TRect& aRect);

        /**
        * CMainS60AppView.
        * C++ default constructor.
        */
        CMainS60AppView();

		CPeriodic*	update;
};

/**
* CMainS60Document application class.
* An instance of class CMainS60Document is the Document part of the
* AVKON application framework for the MainS60 example application.
*/
class CMainS60Document : public CAknDocument
{
    public: // Constructors and destructor

        /**
        * NewL.
        * Two-phased constructor.
        * Construct a CMainS60Document for the AVKON application aApp
        * using two phase construction, and return a pointer
        * to the created object.
        * @param aApp Application creating this document.
        * @return A pointer to the created instance of CMainS60Document.
        */
        static CMainS60Document* NewL( CEikApplication& aApp );

        /**
        * NewLC.
        * Two-phased constructor.
        * Construct a CMainS60Document for the AVKON application aApp
        * using two phase construction, and return a pointer
        * to the created object.
        * @param aApp Application creating this document.
        * @return A pointer to the created instance of CMainS60Document.
        */
        static CMainS60Document* NewLC( CEikApplication& aApp );

        /**
        * ~CMainS60Document
        * Virtual Destructor.
        */
        virtual ~CMainS60Document();

    public: // Functions from base classes

        /**
        * CreateAppUiL
        * From CEikDocument, CreateAppUiL.
        * Create a CMainS60AppUi object and return a pointer to it.
        * The object returned is owned by the Uikon framework.
        * @return Pointer to created instance of AppUi.
        */
        CEikAppUi* CreateAppUiL();

    private: // Constructors

        /**
        * ConstructL
        * 2nd phase constructor.
        */
        void ConstructL();

        /**
        * CMainS60Document.
        * C++ default constructor.
        * @param aApp Application creating this document.
        */
        CMainS60Document( CEikApplication& aApp );
};

//
// Constants
//

// UID for the application - this should correspond to the uid defined in the mmp file
const TUid KUidMainS60App = { 0xed8ee21e };


//
// Main
//

LOCAL_C CApaApplication* NewApplication()
{
	return new CMainS60Application;
}

GLDEF_C TInt E32Main()
{
	return EikStart::RunApplication( NewApplication );
}

//
// Application
//

// Creates CApaDocument object
CApaDocument* CMainS60Application::CreateDocumentL()
{
    // Create an MainS60 document, and return a pointer to it
    return (static_cast<CApaDocument*>
                    ( CMainS60Document::NewL( *this ) ) );
}

// Returns application UID
TUid CMainS60Application::AppDllUid() const
{
    // Return the UID for the MainS60 application
    return KUidMainS60App;
}

//
// AppUi
//

// Symbian 2nd phase constructor can leave.
void CMainS60AppUi::ConstructL()
{
    // Initialise app UI with standard value.
    BaseConstructL();

	// Create view object
	iAppView = CMainS60AppView::NewL( ClientRect() );
}

// C++ default constructor can NOT contain any code, that might leave.
CMainS60AppUi::CMainS60AppUi()
{
    // No implementation required
}

// Destructor.
CMainS60AppUi::~CMainS60AppUi()
{
    if ( iAppView )
    {
        delete iAppView;
        iAppView = NULL;
    }

}

// Takes care of command handling.
void CMainS60AppUi::HandleCommandL( TInt aCommand )
{
    switch( aCommand )
    {
        case EAknSoftkeyExit:
            Exit();
            break;
    }
}

//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
void CMainS60AppUi::HandleStatusPaneSizeChange()
{
	iAppView->SetRect( ClientRect() );	
} 

//
// AppView
//

// Two-phased constructor.
CMainS60AppView* CMainS60AppView::NewL( const TRect& aRect )
{
    CMainS60AppView* self = CMainS60AppView::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
}

// Two-phased constructor.
CMainS60AppView* CMainS60AppView::NewLC( const TRect& aRect )
{
    CMainS60AppView* self = new ( ELeave ) CMainS60AppView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
}

// Symbian 2nd phase constructor can leave.
void CMainS60AppView::ConstructL( const TRect& aRect )
{
/*
	// Set the default path
	RFs fileServer;
	TFileName path;
	char buffer[128] = {0};
	char* temp = 0;

	fileServer.Connect();
	path = PathInfo::MemoryCardRootPath();
	path.Append(PathInfo::OthersPath());
	fileServer.Close();

	if (path.Length() + 1 <= 128)
	{
		temp = (char*)path.Ptr();	
		for (int i = 0; i < path.Length()*2; i+=2)
		{
			buffer[i/2] = temp[i];
		}
		buffer[path.Length()] = 0;
	}
*/
	// Set the search path to the Others directory on the memory card
	//Loader::setSearchPath(buffer);
#ifndef USE_IRRLICHT
	Loader::setSearchPath("C:\\Temp\\VS2003Temp\\Symbian1\\data\\");
#endif

    // Create a window for this application view
    CreateWindowL();

    // Set the windows size to full screen
	SetExtentToWholeScreen();

    // Activate the window, which makes it ready to be drawn
    ActivateL();
	
#ifdef USE_IRRLICHT
	SIrrlichtCreationParameters parameters;
	parameters.WindowSize = core::dimension2d<s32>(240, 320);
	parameters.DriverType = EDT_OPENGL;
	parameters.WindowId = (s32)(&Window());
	device = createDeviceEx(parameters);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	//IAnimatedMesh* mesh = smgr->getMesh("C:\\Temp\\VS2003Temp\\Symbian1\\data\\sydney.md2");
	IAnimatedMesh* mesh = smgr->getMesh("C:\\irrlicht_media\\sydney.md2");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation ( scene::EMAT_STAND );
		node->setMaterialTexture( 0, driver->getTexture("C:\\irrlicht_media\\sydney.bmp") );
	}
	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
#else
    //
    // EGL creation
    //
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

	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(eglDisplay, &majorVersion, &minorVersion);
	eglGetConfigs(eglDisplay, NULL, 0, &numConfigs);
	eglChooseConfig(eglDisplay, s_configAttribs, &eglConfig, 1, &numConfigs);
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);

	eglWindowSurface = eglCreateWindowSurface(eglDisplay, eglConfig, &Window(), NULL);
	eglMakeCurrent(eglDisplay, eglWindowSurface, eglWindowSurface, eglContext);
	game.init();

	game.reshape(aRect.Width(), aRect.Height());

#endif	
	
    update = CPeriodic::NewL( CActive::EPriorityIdle );
    
    update->Start( 1000, 1000, TCallBack( CMainS60AppView::Update, this ) );
}

// C++ default constructor can NOT contain any code, that might leave.
CMainS60AppView::CMainS60AppView()
{
    // No implementation required
}


// Destructor.
CMainS60AppView::~CMainS60AppView()
{
	delete update;
#if defined(USE_IRRLICHT)	
	device->drop();
#else
	game.destroy();
	
	//
	// EGL destruction
	//
	eglMakeCurrent(eglDisplay, NULL, NULL, NULL);
	eglDestroyContext(eglDisplay, eglContext);
	eglDestroySurface(eglDisplay, eglWindowSurface);
	eglTerminate(eglDisplay);	
#endif
}


// Draws the display.
void CMainS60AppView::Draw( const TRect& /*aRect*/ ) const
{
}

// Called by framework when the view size is changed.
void CMainS60AppView::SizeChanged()
{  
	TSize size;
    size = this->Size();
#ifdef USE_IRRLICHT
	//IVideoDriver* driver = device->getVideoDriver();
	//driver->OnResize(core::dimension2d<s32>(size.iWidth,size.iHeight)); 
#else
    game.reshape(size.iWidth, size.iHeight);
#endif
}

TInt CMainS60AppView::Update( TAny* aInstance )
{
	if (aInstance)
	{
#ifdef USE_IRRLICHT
		IVideoDriver* driver = device->getVideoDriver();
		ISceneManager* smgr = device->getSceneManager();
		IGUIEnvironment* guienv = device->getGUIEnvironment();
		if(device->run()){
			driver->beginScene(true, true, SColor(255,100,101,140));

			smgr->drawAll();
			//guienv->drawAll();

			driver->endScene();
		}
#else
		game.update();
	
	    eglSwapBuffers( eglDisplay, eglWindowSurface );
#endif
	}
        
    return 0; 
}

//
// Document
//

// CMainS60Document::NewL()
// Two-phased constructor.
CMainS60Document* CMainS60Document::NewL( CEikApplication& aApp )
{
    CMainS60Document* self = NewLC( aApp );
    CleanupStack::Pop( self );
    return self;
}

// CMainS60Document::NewLC()
// Two-phased constructor.
CMainS60Document* CMainS60Document::NewLC( CEikApplication& aApp )
{
    CMainS60Document* self =
        new ( ELeave ) CMainS60Document( aApp );

    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
}

// Symbian 2nd phase constructor can leave.
void CMainS60Document::ConstructL()
{
    // No implementation required
}

// C++ default constructor can NOT contain any code, that might leave.
CMainS60Document::CMainS60Document( CEikApplication& aApp )
    : CAknDocument( aApp )
{
    // No implementation required
}

// Destructor.
CMainS60Document::~CMainS60Document()
{
    // No implementation required
}

// Constructs CreateAppUi.
CEikAppUi* CMainS60Document::CreateAppUiL()
{
    // Create the application user interface, and return a pointer to it;
    // the framework takes ownership of this object
    return ( static_cast <CEikAppUi*> ( new ( ELeave )
                                        CMainS60AppUi ) );
}
