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
#include <coemain.h>
#include "mHelloWorldAppView.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CmHelloWorldAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CmHelloWorldAppView* CmHelloWorldAppView::NewL( const TRect& aRect )
    {
    CmHelloWorldAppView* self = CmHelloWorldAppView::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CmHelloWorldAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CmHelloWorldAppView* CmHelloWorldAppView::NewLC( const TRect& aRect )
    {
    CmHelloWorldAppView* self = new ( ELeave ) CmHelloWorldAppView;
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CmHelloWorldAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CmHelloWorldAppView::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect( aRect );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// CmHelloWorldAppView::CmHelloWorldAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CmHelloWorldAppView::CmHelloWorldAppView()
    {
    // No implementation required
    }


// -----------------------------------------------------------------------------
// CmHelloWorldAppView::~CmHelloWorldAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CmHelloWorldAppView::~CmHelloWorldAppView()
    {
    // No implementation required
    }


// -----------------------------------------------------------------------------
// CmHelloWorldAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CmHelloWorldAppView::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect drawRect( Rect());

    // Clears the screen
    gc.Clear( drawRect );
    
  	}

// -----------------------------------------------------------------------------
// CmHelloWorldAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CmHelloWorldAppView::SizeChanged()
    {  
    DrawNow();
    }
// End of File
