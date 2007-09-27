/* This file is copied from the ES 3D project(Norbert Nopper) on sourceforge.net*/
//
// Include
//

#include <aknapp.h>
#include <aknappui.h>
#include <akndoc.h>
#include <coecntrl.h>
#include <eikstart.h>

#include <unistd.h> //for chdir
#include <sys/reent.h> //for CloseSTDLIB
#include <GLES/egl.h>

class CMainS60Application;
class CMainS60AppUi;
class CMainS60AppView;
class CMainS60Document;


#include "irrlicht.h"

//if SHOW_HELLO_WORLD is not defined, we'll show the Quake3Map example
#define SHOW_HELLO_WORLD
//#define SHOW_QUAKE3_MAP
//#define SHOW_SPECIAL_FX

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

static IrrlichtDevice *device;	

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
        
		/** 
		* Handle key inputs
		*/
		virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);

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
const TUid KUidMainS60App = { 0xebf1104c };


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

CMainS60AppUi::CMainS60AppUi()
{
}

// Destructor.
CMainS60AppUi::~CMainS60AppUi()
{
    if ( iAppView )
    {
    	RemoveFromStack( iAppView );
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
	
	//NOTE! on symbian platform, the emulator interprets "E:\" as "%SDK_ROOT%\epoc32\winscw\E"
	//I placed the media files under "E:\\irrlicht\\media". The code in irrlicht\examples uses
	//relative directory ../../media. Therefore, we create the follow dummy directory beforehand
	//and change the working directory at run time.
	chdir("E:\\irrlicht\\dummy\\dummy");	
	
	SIrrlichtCreationParameters parameters;
	parameters.WindowSize = core::dimension2d<s32>(240, 320);
	parameters.DriverType = EDT_OPENGL;
	parameters.WindowId = (s32)(&Window());
#ifdef SHOW_SPECIAL_FX
	parameters.Stencilbuffer = true;
#endif
	device = createDeviceEx(parameters);
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

#if defined(SHOW_HELLO_WORLD)	

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

	smgr->addSkyBoxSceneNode(
		driver->getTexture("../../media/irrlicht2_up_small.jpg"),
		driver->getTexture("../../media/irrlicht2_dn_small.jpg"),
		driver->getTexture("../../media/irrlicht2_lf_small.jpg"),
		driver->getTexture("../../media/irrlicht2_rt_small.jpg"),
		driver->getTexture("../../media/irrlicht2_ft_small.jpg"),
		driver->getTexture("../../media/irrlicht2_bk_small.jpg"));

#elif defined(SHOW_QUAKE3_MAP)

	device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2-small.pk3");
	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* q3node = 0;
	scene::ITriangleSelector* selector = 0;

	if (mesh)
		q3node = smgr->addOctTreeSceneNode(mesh->getMesh(0));
	if (q3node){
		q3node->setPosition(core::vector3df(-1370,-130,-1400));
		selector = smgr->createOctTreeTriangleSelector(mesh->getMesh(0), q3node, 128);
		q3node->setTriangleSelector(selector);
		selector->drop();
	}
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(core::vector3df(0,10,-150));

	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
											selector, camera, core::vector3df(30,50,30),
											core::vector3df(0,-3,0), 
											core::vector3df(0,50,0));
	camera->addAnimator(anim);
	anim->drop();

	//don't use the faerie model because of the memory constraints
	video::SMaterial material;
	//material.Texture1 = driver->getTexture("../../media/faerie2.bmp");
	material.Texture1 = driver->getTexture("../../media/laalaa8.bmp");
	material.Lighting = true;

	scene::IAnimatedMeshSceneNode* node = 0;
	//scene::IAnimatedMesh* faerie = smgr->getMesh("../../media/faerie.md2");
	scene::IAnimatedMesh* faerie = smgr->getMesh("../../media/laalaa.md2");

	if (faerie)
	{
		node = smgr->addAnimatedMeshSceneNode(faerie);
		node->setRotation(core::vector3df(0,90,0));
		node->setPosition(core::vector3df(10,-20,-30));
		node->setMD2Animation(scene::EMAT_RUN);
		node->getMaterial(0) = material;

		node = smgr->addAnimatedMeshSceneNode(faerie);
		node->setPosition(core::vector3df(40,-20,-30));
		node->setMD2Animation(scene::EMAT_SALUTE);
		node->getMaterial(0) = material;
	}

	material.Texture1 = 0;
	material.Lighting = false;

	// Add a light
	smgr->addLightSceneNode(0, core::vector3df(-20,30,-60),
							video::SColorf(1.0f,1.0f,1.0f,1.0f),
							200.0f);

	smgr->addLightSceneNode(0, core::vector3df(-60,100,400),
							video::SColorf(1.0f,1.0f,1.0f,1.0f),
							600.0f);
#elif defined(SHOW_SPECIAL_FX)
	//create room
	scene::IAnimatedMesh* mesh = smgr->getMesh(
		"../../media/room.3ds");

	smgr->getMeshManipulator()->makePlanarTextureMapping(
		mesh->getMesh(0), 0.004f);

	scene::ISceneNode* node = 0;

	node = smgr->addAnimatedMeshSceneNode(mesh);
	node->setMaterialTexture(0,	driver->getTexture("../../media/wall.jpg"));
	node->getMaterial(0).SpecularColor.set(0,0,0,0); 

#if 0 //animated water needs texgen, which isn't supported by OpenGL ES
	// add animated water

	mesh = smgr->addHillPlaneMesh("myHill",
		core::dimension2d<f32>(20,20),
		core::dimension2d<s32>(40,40), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(10,10));

	node = smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 3.0f, 300.0f, 30.0f);
	node->setPosition(core::vector3df(0,7,0));

	node->setMaterialTexture(0,	driver->getTexture("../../media/stones.jpg"));
	node->setMaterialTexture(1,	driver->getTexture("../../media/water.jpg"));

	node->setMaterialType(video::EMT_REFLECTION_2_LAYER);
#endif
	// create light

	node = smgr->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 600.0f);
	scene::ISceneNodeAnimator* anim = 0;
	anim = smgr->createFlyCircleAnimator (core::vector3df(0,150,0),250.0f);
	node->addAnimator(anim);
	anim->drop();

	// attach billboard to light

	node = smgr->addBillboardSceneNode(node, core::dimension2d<f32>(50, 50));
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	node->setMaterialTexture(0,	driver->getTexture("../../media/particlewhite.bmp"));

	// create a particle system

	scene::IParticleSystemSceneNode* ps = 0;
	ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition(core::vector3df(-70,60,40));
	ps->setScale(core::vector3df(2,2,2));

	ps->setParticleSize(core::dimension2d<f32>(20.0f, 20.0f));

	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-7,0,-7,7,1,7),
		core::vector3df(0.0f,0.06f,0.0f),
		80,100,
		video::SColor(0,255,255,255), video::SColor(0,255,255,255),
		800,2000);

	ps->setEmitter(em);
	em->drop();

	scene::IParticleAffector* paf =
		ps->createFadeOutParticleAffector();

	ps->addAffector(paf);
	paf->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, driver->getTexture("../../media/fire.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

	// add animated character 
#define USE_DWARF
#ifdef USE_DWARF
	mesh = smgr->getMesh("../../media/dwarf.x");
#else
	mesh = smgr->getMesh("../../media/cube.ms3d");
#endif
	scene::IAnimatedMeshSceneNode* anode = 0;

	anode = smgr->addAnimatedMeshSceneNode(mesh);
#ifdef USE_DWARF
	anode->setPosition(core::vector3df(-50,20,-60));
	anode->setAnimationSpeed(15);
#else
	anode->setPosition(core::vector3df(-50,50,-50));
#endif

	// add shadow
	anode->addShadowVolumeSceneNode();
	smgr->setShadowColor(video::SColor(150,0,0,0));

	// make the model a little bit bigger and normalize its normals
	// because of this for correct lighting
#ifdef USE_DWARF
	anode->setScale(core::vector3df(2,2,2));
#else
	anode->setScale(core::vector3df(0.5,0.5,0.5));
#endif
	anode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	camera->setPosition(core::vector3df(-50,50,-200)); 
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


TInt CMainS60AppView::Update( TAny* aInstance )
{
	if (aInstance)
	{
		IVideoDriver* driver = device->getVideoDriver();
		ISceneManager* smgr = device->getSceneManager();
		IGUIEnvironment* guienv = device->getGUIEnvironment();
		if(device->run()){
			driver->beginScene(true, true, SColor(255,100,101,140));

			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();
		}
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
