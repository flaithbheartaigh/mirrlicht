/*
============================================================================
 Name        : CSymbIrrlichtContainer from SymbIrrlichtContainer.h
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : Container control implementation
============================================================================
*/

// INCLUDE FILES
#include "SymbIrrlichtContainer.h"

#include <eiklabel.h>  // for example label control


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSymbIrrlichtContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CSymbIrrlichtContainer::ConstructL(const TRect& aRect)
    {
    CreateWindowL();

    iLabel = new (ELeave) CEikLabel;
    iLabel->SetContainerWindowL( *this );
    iLabel->SetTextL( _L("Example View") );

    iToDoLabel = new (ELeave) CEikLabel;
    iToDoLabel->SetContainerWindowL( *this );
    iToDoLabel->SetTextL( _L("Add Your controls\n here") );

    SetRect(aRect);
    ActivateL();
    }

// Destructor
CSymbIrrlichtContainer::~CSymbIrrlichtContainer()
    {
    delete iLabel;
    delete iToDoLabel;
    }

// ---------------------------------------------------------
// CSymbIrrlichtContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CSymbIrrlichtContainer::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    iLabel->SetExtent( TPoint(10,10), iLabel->MinimumSize() );
    iToDoLabel->SetExtent( TPoint(10,100), iToDoLabel->MinimumSize() );
    }

// ---------------------------------------------------------
// CSymbIrrlichtContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CSymbIrrlichtContainer::CountComponentControls() const
    {
    return 2; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CSymbIrrlichtContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CSymbIrrlichtContainer::ComponentControl(TInt aIndex) const
    {
    switch ( aIndex )
        {
        case 0:
            return iLabel;
        case 1:
            return iToDoLabel;
        default:
            return NULL;
        }
    }

// ---------------------------------------------------------
// CSymbIrrlichtContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CSymbIrrlichtContainer::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
    // TODO: Add your drawing code here
    // example code...
    gc.SetPenStyle( CGraphicsContext::ENullPen );
    gc.SetBrushColor( KRgbGray );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.DrawRect( aRect );
    }

// ---------------------------------------------------------
// CSymbIrrlichtContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CSymbIrrlichtContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }


  
