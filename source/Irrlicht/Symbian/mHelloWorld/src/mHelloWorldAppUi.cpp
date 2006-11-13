/*
============================================================================
 Name        : mHelloWorld.rss
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Main application class
============================================================================
*/

// INCLUDE FILES
#include <avkon.hrh>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <mHelloWorld.rsg>
#include <f32file.h>
#include <s32file.h>

#include "mHelloWorld.pan"
#include "mHelloWorldAppUi.h"
#include "mHelloWorldAppView.h"
#include "mHelloWorld.hrh"

/*_LIT( KHelloFileName,	"z:\\system\\apps\\mHelloWorld\\Hello.txt" );
_LIT( KHelloText, "HELLO WORLD!");*/

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CmHelloWorldAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CmHelloWorldAppUi::ConstructL()
    {
    // Initialise app UI with standard value.
    BaseConstructL();
    
    iEngine = Cmirrlicht::NewL();

    // Create view object
    iAppView = CmHelloWorldAppView::NewL( ClientRect() );

    
    }
// -----------------------------------------------------------------------------
// CmHelloWorldAppUi::CmHelloWorldAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CmHelloWorldAppUi::CmHelloWorldAppUi()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CmHelloWorldAppUi::~CmHelloWorldAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CmHelloWorldAppUi::~CmHelloWorldAppUi()
    {
    if ( iAppView )
        {
        delete iAppView;
        iAppView = NULL;
        }
    delete iEngine;
    }

// -----------------------------------------------------------------------------
// CmHelloWorldAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CmHelloWorldAppUi::HandleCommandL( TInt aCommand )
    {
    switch( aCommand )
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            Exit();
            break;
        case EmHelloWorldCommand1:
            {            
            // Load a string from the resource file and display it
            HBufC* textResource = StringLoader::LoadLC( R_HEWB_COMMAND1_TEXT );
            CAknInformationNote* informationNote = new ( ELeave ) CAknInformationNote;
            // Show the information Note with textResource loaded with StringLoader.
            informationNote->ExecuteLD( *textResource);
            // Pop HBuf from CleanUpStack and Destroy it.
            CleanupStack::PopAndDestroy( textResource );
            }
            break;
		case EmHelloWorldCommand2:
			{
            // Use the engine DLL
            TInt ch;
            for(ch = 'A'; ch <= 'O'; ch++)
                {
                iEngine->ExampleFuncAddCharL(ch);
                }
            // Show the information Note with text from the engine
            CAknInformationNote* informationNote = new (ELeave) CAknInformationNote;
            informationNote->ExecuteLD(iEngine->ExampleFuncString());
		iEngine->ExampleFuncClearBuffer();
            break;            
        default:
            Panic( EmHelloWorldUi );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CmHelloWorldAppUi::HandleStatusPaneSizeChange()
{
	iAppView->SetRect( ClientRect() );
	
} 

// End of File
