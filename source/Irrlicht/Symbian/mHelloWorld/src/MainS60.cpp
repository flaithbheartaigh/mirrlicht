//
// Include
//

#include <aknapp.h>
#include <aknappui.h>
#include <akndoc.h>
#include <coecntrl.h>
#include <eikstart.h>

#include "mHelloWorld.hrh" //menu command

#include <unistd.h> //for chdir
#include <sys/reent.h> //for CloseSTDLIB


class CMainS60Application;
class CMainS60AppUi;
class CMainS60AppView;
class CMainS60Document;

#include "irrlicht.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
	
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
		* Get the irrlicht render device
		*/
		IrrlichtDevice* GetIrrDevice() const { return device; }

		/**
		* Get the text GUI element. For setting FPS in the update routine
		*/
		IGUIStaticText *GetGUIFPSText() const { return textFPS; }

		/** 
		* Handle key inputs
		*/
		virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

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

		IrrlichtDevice *device;	

		//text to show FPS (frames per second)
		IGUIStaticText *textFPS;
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
const TUid KUidMainS60App = { 0xe516d258 };


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
	// Disable key blocking
	SetKeyBlockMode(ENoKeyBlock);

	// Create view object
	iAppView = CMainS60AppView::NewL( ClientRect() );

	AddToStackL( iAppView );
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
        RemoveFromStack(iAppView);
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
			CloseSTDLIB();
            Exit();
            break;
		case EmShowTerrain:
			{
				IrrlichtDevice* irrDevice = iAppView->GetIrrDevice();
				ISceneManager* smgr = irrDevice->getSceneManager();
				IVideoDriver* driver = irrDevice->getVideoDriver();

				smgr->clear();
				driver->removeAllTextures();

				scene::ICameraSceneNode* camera = 
					smgr->addCameraSceneNodeFPS(0,100.0f,1200.0f);

				camera->setPosition(core::vector3df(1900*2,255*2,3700*2));
				camera->setTarget(core::vector3df(2397*2,343*2,2700*2));
				camera->setFarValue(12000.0f);

				// add terrain scene node
				//smaller heighmap. 75% of the original 256x256 size. The emulator's memory should set to above 48M.
				scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode( 
					"../../media/terrain-heightmap-small.bmp"); 

				terrain->setScale(core::vector3df(40, 4.4f, 40));
				terrain->setMaterialFlag(video::EMF_LIGHTING, false);

				terrain->setMaterialTexture(0, driver->getTexture("../../media/terrain-texture-small.jpg"));
				terrain->setMaterialTexture(1, driver->getTexture("../../media/detailmap3.jpg"));

				terrain->setMaterialType(video::EMT_DETAIL_MAP);

				terrain->scaleTexture(1.0f, 20.0f);

				// create triangle selector for the terrain	
				scene::ITriangleSelector* selector
					= smgr->createTerrainTriangleSelector(terrain, 0);
				terrain->setTriangleSelector(selector);
				selector->drop();

				// create collision response animator and attach it to the camera
				scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
					selector, camera, core::vector3df(60,100,60),
					core::vector3df(0,0,0), 
					core::vector3df(0,50,0));
				camera->addAnimator(anim);
				anim->drop();
			}
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
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size to full screen
	SetExtentToWholeScreen();

    // Activate the window, which makes it ready to be drawn
    ActivateL();
	
	chdir("C:\\irrlicht\\dummy\\dummy");	

	SIrrlichtCreationParameters parameters;
	parameters.WindowSize = core::dimension2d<s32>(240, 320);
	parameters.DriverType = EDT_OPENGL;
	parameters.WindowId = &Window();
	device = createDeviceEx(parameters);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	textFPS = guienv->addStaticText(L"FPS",	rect<int>(10,10,40,22), true);
	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation ( scene::EMAT_STAND );
		node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
	}
	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
	
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
	device->drop();
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
	//IVideoDriver* driver = device->getVideoDriver();
	//driver->OnResize(core::dimension2d<s32>(size.iWidth,size.iHeight)); 
}

TInt CMainS60AppView::Update( TAny* aInstance )
{
	static int lastFPS = -1;

	if (aInstance)
	{		
		IrrlichtDevice* irrDevice = ((CMainS60AppView*)aInstance)->GetIrrDevice();
		IVideoDriver* driver = irrDevice->getVideoDriver();
		ISceneManager* smgr = irrDevice->getSceneManager();
		IGUIEnvironment* guienv = irrDevice->getGUIEnvironment();
		if(irrDevice->run()){
			driver->beginScene(true, true, SColor(255,100,101,140));

			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();

			// display frames per second in window title
			int fps = driver->getFPS();
			if (lastFPS != fps)
			{
				core::stringw str = L"FPS:";				
				str += fps;
				
				IGUIStaticText* fpsText=((CMainS60AppView*)aInstance)->GetGUIFPSText();
				fpsText->setText(str.c_str());
				lastFPS = fps;
			}

		}
	}
        
    return 0; 
}

TKeyResponse CMainS60AppView::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
{
	irr::SEvent event;
	if(aType == EEventKeyDown || aType == EEventKeyUp ){
		event.EventType = irr::EET_KEY_INPUT_EVENT;
		if(aType == EEventKeyDown){
			event.KeyInput.PressedDown = true;
			//device->getLogger()->log("KeyDown");
		}else{
			event.KeyInput.PressedDown = false;
			//device->getLogger()->log("KeyDown");
		}
		switch(aKeyEvent.iScanCode)
		{
		case EStdKeyNkp2:
		case EStdKeyUpArrow:			
			event.KeyInput.Key = irr::KEY_UP;
			device->postEventFromUser(event);
			return EKeyWasConsumed;
		case EStdKeyNkp8:
		case EStdKeyDownArrow:
			event.KeyInput.Key = irr::KEY_DOWN;			
			device->postEventFromUser(event);
			return EKeyWasConsumed;	
		case EStdKeyNkp4:
		case EStdKeyLeftArrow:
			event.KeyInput.Key = irr::KEY_LEFT;			
			device->postEventFromUser(event);
			return EKeyWasConsumed;
		case EStdKeyNkp6:
		case EStdKeyRightArrow:
			event.KeyInput.Key = irr::KEY_RIGHT;			
			device->postEventFromUser(event);
			return EKeyWasConsumed;
		}
	}
	return EKeyWasNotConsumed;
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
