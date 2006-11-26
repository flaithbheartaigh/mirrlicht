/*
============================================================================
 Name        : CSymbIrrlichtAppUi from SymbIrrlichtAppui.cpp
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : CSymbIrrlichtAppUi implementation
============================================================================
*/

// INCLUDE FILES
#include "SymbIrrlichtAppui.h"
#include "SymbIrrlichtView.h"
#include "SymbIrrlichtView2.h"
#include <SymbIrrlicht.rsg>
#include "SymbIrrlicht.hrh"

#include <avkon.hrh>

// ================= MEMBER FUNCTIONS =======================
//
// ----------------------------------------------------------
// CSymbIrrlichtAppUi::ConstructL()
// 
// ----------------------------------------------------------
//
void CSymbIrrlichtAppUi::ConstructL()
    {
    BaseConstructL();


    // Show tabs for main views from resources
    CEikStatusPane* sp = StatusPane();

    // Fetch pointer to the default navi pane control
    iNaviPane = (CAknNavigationControlContainer*)sp->ControlL( 
        TUid::Uid(EEikStatusPaneUidNavi));

    // Tabgroup has been read from resource and it were pushed to the navi pane. 
    // Get pointer to the navigation decorator with the ResourceDecorator() function. 
    // Application owns the decorator and it has responsibility to delete the object.
    iDecoratedTabGroup = iNaviPane->ResourceDecorator();
    if (iDecoratedTabGroup)
        {
        iTabGroup = (CAknTabGroup*) iDecoratedTabGroup->DecoratedControl();
		iTabGroup->SetObserver( this );
		}

    CSymbIrrlichtView* view1 = new (ELeave) CSymbIrrlichtView;

    CleanupStack::PushL( view1 );
    view1->ConstructL();
    AddViewL( view1 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view1

    CSymbIrrlichtView2* view2 = new (ELeave) CSymbIrrlichtView2;

    CleanupStack::PushL( view2 );
    view2->ConstructL();
    AddViewL( view2 );      // transfer ownership to CAknViewAppUi
    CleanupStack::Pop();    // view2

    SetDefaultViewL(*view1);

    }

// ----------------------------------------------------
// CSymbIrrlichtAppUi::~CSymbIrrlichtAppUi()
// Destructor
// Frees reserved resources
// ----------------------------------------------------
//
CSymbIrrlichtAppUi::~CSymbIrrlichtAppUi()
    {
    delete iDecoratedTabGroup;
   }

// ------------------------------------------------------------------------------
// CSymbIrrlichtAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
//  This function is called by the EIKON framework just before it displays
//  a menu pane. Its default implementation is empty, and by overriding it,
//  the application can set the state of menu items dynamically according
//  to the state of application data.
// ------------------------------------------------------------------------------
//
void CSymbIrrlichtAppUi::DynInitMenuPaneL(
    TInt /*aResourceId*/,CEikMenuPane* /*aMenuPane*/)
    {
    }

// ----------------------------------------------------
// CSymbIrrlichtAppUi::HandleKeyEventL(
//     const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
// takes care of key event handling
// ----------------------------------------------------
//
TKeyResponse CSymbIrrlichtAppUi::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType)
    {
	if ( iTabGroup == NULL )
        {
        return EKeyWasNotConsumed;
        }

    if ( aKeyEvent.iCode == EKeyLeftArrow || aKeyEvent.iCode == EKeyRightArrow )
        {
        return iTabGroup->OfferKeyEventL( aKeyEvent, aType );
        }
    else
        {
        return EKeyWasNotConsumed;
        }
    }

// ----------------------------------------------------
// CSymbIrrlichtAppUi::HandleCommandL(TInt aCommand)
// takes care of command handling
// ----------------------------------------------------
//
void CSymbIrrlichtAppUi::HandleCommandL(TInt aCommand)
    {
    switch ( aCommand )
        {
        case EEikCmdExit:
            {
            Exit();
            break;
            }
        case ESymbIrrlichtCmdAppTest:
            {
            iEikonEnv->InfoMsg(_L("test"));
            break;
            }
        // TODO: Add Your command handling code here

        default:
            break;      
        }
    }

 // ----------------------------------------------------
 // CSymbIrrlichtAppUi::TabChangedL(TInt aIndex)
 // This method gets called when CAknTabGroup active 
 // tab has changed.
 // ----------------------------------------------------
 //
 void CSymbIrrlichtAppUi::TabChangedL(TInt aIndex)
     {
     ActivateLocalViewL(TUid::Uid(iTabGroup->TabIdFromIndex(aIndex)));
     }
