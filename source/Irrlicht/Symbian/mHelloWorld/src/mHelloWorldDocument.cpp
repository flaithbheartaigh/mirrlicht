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
#include "mHelloWorldAppUi.h"
#include "mHelloWorldDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CmHelloWorldDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CmHelloWorldDocument* CmHelloWorldDocument::NewL( CEikApplication&
                                                          aApp )
    {
    CmHelloWorldDocument* self = NewLC( aApp );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CmHelloWorldDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CmHelloWorldDocument* CmHelloWorldDocument::NewLC( CEikApplication&
                                                           aApp )
    {
    CmHelloWorldDocument* self =
        new ( ELeave ) CmHelloWorldDocument( aApp );

    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CmHelloWorldDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CmHelloWorldDocument::ConstructL()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CmHelloWorldDocument::CmHelloWorldDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CmHelloWorldDocument::CmHelloWorldDocument( CEikApplication& aApp )
    : CAknDocument( aApp )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CmHelloWorldDocument::~CmHelloWorldDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CmHelloWorldDocument::~CmHelloWorldDocument()
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// CmHelloWorldDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CmHelloWorldDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it;
    // the framework takes ownership of this object
    return ( static_cast <CEikAppUi*> ( new ( ELeave )
                                        CmHelloWorldAppUi ) );
    }

// End of File
