/*
============================================================================
 Name        : CSymbIrrlichtView2 from SymbIrrlichtView2.h
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : CSymbIrrlichtView2 implementation
============================================================================
*/

// INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <SymbIrrlicht.rsg>
#include  "SymbIrrlichtView2.h"
#include  "SymbIrrlichtContainer2.h" 

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSymbIrrlichtView2::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CSymbIrrlichtView2::ConstructL()
    {
    BaseConstructL( R_SYMBIRRLICHT_VIEW2 );
    }

// ---------------------------------------------------------
// CSymbIrrlichtView2::~CSymbIrrlichtView2()
// destructor
// ---------------------------------------------------------
//
CSymbIrrlichtView2::~CSymbIrrlichtView2()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CSymbIrrlichtView2::Id()
// 
// ---------------------------------------------------------
//
TUid CSymbIrrlichtView2::Id() const
    {
    return KView2Id;
    }

// ---------------------------------------------------------
// CSymbIrrlichtView2::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CSymbIrrlichtView2::HandleCommandL(TInt aCommand)
    {   
    switch ( aCommand )
        {
        case EAknSoftkeyOk:
            {
            iEikonEnv->InfoMsg( _L("view2 ok") );
            break;
            }
        case EAknSoftkeyBack:
            {
            AppUi()->HandleCommandL(EEikCmdExit);
            break;
            }
        default:
            {
            AppUi()->HandleCommandL( aCommand );
            break;
            }
        }
    }

// ---------------------------------------------------------
// CSymbIrrlichtView2::HandleClientRectChange()
// ---------------------------------------------------------
//
void CSymbIrrlichtView2::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CSymbIrrlichtView2::DoActivateL(...)
// 
// ---------------------------------------------------------
//
void CSymbIrrlichtView2::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CSymbIrrlichtContainer2;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );
        }
   }

// ---------------------------------------------------------
// CSymbIrrlichtView2::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CSymbIrrlichtView2::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }



