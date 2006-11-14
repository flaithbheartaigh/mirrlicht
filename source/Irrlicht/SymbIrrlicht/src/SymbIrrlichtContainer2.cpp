/*
============================================================================
 Name        : CSymbIrrlichtContainer2 from SymbIrrlichtContainer2.cpp
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : Container control implementation
============================================================================
*/

// INCLUDE FILES
#include "SymbIrrlichtContainer2.h"

#include <eiklabel.h>  // for example label control


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CSymbIrrlichtContainer2::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CSymbIrrlichtContainer2::ConstructL(const TRect& aRect)
    {
    CreateWindowL();

    iLabel = new (ELeave) CEikLabel;
    iLabel->SetContainerWindowL( *this );
    iLabel->SetTextL( _L("Example View 2") );

    iToDoLabel = new (ELeave) CEikLabel;
    iToDoLabel->SetContainerWindowL( *this );
    iToDoLabel->SetTextL( _L("Add Your controls\n here") );

    SetRect(aRect);
    ActivateL();
    }

// Destructor
CSymbIrrlichtContainer2::~CSymbIrrlichtContainer2()
    {
    delete iLabel;
    delete iToDoLabel;
    }

// ---------------------------------------------------------
// CSymbIrrlichtContainer2::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CSymbIrrlichtContainer2::SizeChanged()
    {
    // TODO: Add here control resize code etc.
    iLabel->SetExtent( TPoint(10,10), iLabel->MinimumSize() );
    iToDoLabel->SetExtent( TPoint(10,100), iToDoLabel->MinimumSize() );
    }

// ---------------------------------------------------------
// CSymbIrrlichtContainer2::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CSymbIrrlichtContainer2::CountComponentControls() const
    {
    return 2; // return nbr of controls inside this container
    }

// ---------------------------------------------------------
// CSymbIrrlichtContainer2::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CSymbIrrlichtContainer2::ComponentControl(TInt aIndex) const
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
// CSymbIrrlichtContainer2::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CSymbIrrlichtContainer2::Draw(const TRect& aRect) const
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
// CSymbIrrlichtContainer2::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CSymbIrrlichtContainer2::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }


  
