/*
============================================================================
 Name        : CSymbIrrlichtDocument from SymbIrrlichtDocument.h
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : CSymbIrrlichtDocument implementation
============================================================================
*/

// INCLUDE FILES
#include "SymbIrrlichtDocument.h"
#include "SymbIrrlichtAppui.h"

// ================= MEMBER FUNCTIONS =======================

// constructor
CSymbIrrlichtDocument::CSymbIrrlichtDocument(CEikApplication& aApp)
: CAknDocument(aApp)    
    {
    }

// destructor
CSymbIrrlichtDocument::~CSymbIrrlichtDocument()
    {
    }

// EPOC default constructor can leave.
void CSymbIrrlichtDocument::ConstructL()
    {
    }

// Two-phased constructor.
CSymbIrrlichtDocument* CSymbIrrlichtDocument::NewL(
        CEikApplication& aApp)     // CSymbIrrlichtApp reference
    {
    CSymbIrrlichtDocument* self = new (ELeave) CSymbIrrlichtDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }
    
// ----------------------------------------------------
// CSymbIrrlichtDocument::CreateAppUiL()
// constructs CSymbIrrlichtAppUi
// ----------------------------------------------------
//
CEikAppUi* CSymbIrrlichtDocument::CreateAppUiL()
    {
    return new (ELeave) CSymbIrrlichtAppUi;
    }

  
