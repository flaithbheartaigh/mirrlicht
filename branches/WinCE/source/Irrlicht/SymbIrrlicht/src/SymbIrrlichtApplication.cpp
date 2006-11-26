/*
============================================================================
 Name        : SymbIrrlichtApplication.cpp
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : Main application class
============================================================================
*/

// INCLUDE FILES
#include "SymbIrrlichtDocument.h"
#include "SymbIrrlichtApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidSymbIrrlichtApp = { 0xebf1104c };

// -----------------------------------------------------------------------------
// CSymbIrrlichtApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CSymbIrrlichtApplication::CreateDocumentL()
    {
    // Create an SymbIrrlicht document, and return a pointer to it
    return (static_cast<CApaDocument*>
                    ( CSymbIrrlichtDocument::NewL( *this ) ) );
    }

// -----------------------------------------------------------------------------
// CSymbIrrlichtApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CSymbIrrlichtApplication::AppDllUid() const
    {
    // Return the UID for the SymbIrrlicht application
    return KUidSymbIrrlichtApp;
    }

// End of File
