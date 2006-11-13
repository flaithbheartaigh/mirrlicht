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
#include "mHelloWorldDocument.h"
#include "mHelloWorldApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidmHelloWorldApp = { 0xe516d258 };

// -----------------------------------------------------------------------------
// CmHelloWorldApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CmHelloWorldApplication::CreateDocumentL()
    {
    // Create an mHelloWorld document, and return a pointer to it
    return (static_cast<CApaDocument*>
                    ( CmHelloWorldDocument::NewL( *this ) ) );
    }

// -----------------------------------------------------------------------------
// CmHelloWorldApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CmHelloWorldApplication::AppDllUid() const
    {
    // Return the UID for the mHelloWorld application
    return KUidmHelloWorldApp;
    }

// End of File
