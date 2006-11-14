/*
============================================================================
 Name        : CSymbIrrlichtView from SymbIrrlichtView.h
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : CSymbIrrlichtView implementation
============================================================================
*/

// INCLUDE FILES
#include  <aknviewappui.h>
#include  <avkon.hrh>
#include  <SymbIrrlicht.rsg>
#include  "SymbIrrlichtView.h"
#include  "SymbIrrlichtContainer.h" 

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSymbIrrlichtView::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
void CSymbIrrlichtView::ConstructL()
    {
    BaseConstructL( R_SYMBIRRLICHT_VIEW1 );
    }

// ---------------------------------------------------------
// CSymbIrrlichtView::~CSymbIrrlichtView()
// destructor
// ---------------------------------------------------------
//
CSymbIrrlichtView::~CSymbIrrlichtView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// ---------------------------------------------------------
// TUid CSymbIrrlichtView::Id()
//
// ---------------------------------------------------------
//
TUid CSymbIrrlichtView::Id() const
    {
    return KViewId;
    }

// ---------------------------------------------------------
// CSymbIrrlichtView::HandleCommandL(TInt aCommand)
// takes care of view command handling
// ---------------------------------------------------------
//
void CSymbIrrlichtView::HandleCommandL(TInt aCommand)
    {   
    switch ( aCommand )
        {
        case EAknSoftkeyOk:
            {
            iEikonEnv->InfoMsg( _L("view1 ok") );
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
// CSymbIrrlichtView::HandleClientRectChange()
// ---------------------------------------------------------
//
void CSymbIrrlichtView::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }

// ---------------------------------------------------------
// CSymbIrrlichtView::DoActivateL(...)
// 
// ---------------------------------------------------------
//
void CSymbIrrlichtView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/)
    {
    if (!iContainer)
        {
        iContainer = new (ELeave) CSymbIrrlichtContainer;
        iContainer->SetMopParent(this);
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );
        } 
   }

// ---------------------------------------------------------
// CSymbIrrlichtView::DoDeactivate()
// 
// ---------------------------------------------------------
//
void CSymbIrrlichtView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }



