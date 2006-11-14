/*
============================================================================
 Name        : CSymbIrrlichtAppUi from SymbIrrlichtAppui.h
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : Declares UI class for application.
============================================================================
*/

#ifndef SYMBIRRLICHTAPPUI_H
#define SYMBIRRLICHTAPPUI_H

// INCLUDES
#include <aknviewappui.h>
#include <akntabgrp.h>
#include <aknnavide.h>

#include <akntabobserver.h>

// FORWARD DECLARATIONS
class CSymbIrrlichtContainer;


// CLASS DECLARATION

/**
* Application UI class.
* Provides support for the following features:
* - EIKON control architecture
* - view architecture
* - status pane
* 
*/
class CSymbIrrlichtAppUi : public CAknViewAppUi, MAknTabObserver
    {
    public: // // Constructors and destructor

        /**
        * EPOC default constructor.
        */      
        void ConstructL();

        /**
        * Destructor.
        */      
        ~CSymbIrrlichtAppUi();
        
    public: // New functions

    public: // Functions from base classes
		/**
        * From MAknTabObserver.
        * @param aIndex tab index
        */
		void TabChangedL(TInt aIndex);

    private:
        // From MEikMenuObserver
        void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);

    private:
        /**
        * From CEikAppUi, takes care of command handling.
        * @param aCommand command to be handled
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CEikAppUi, handles key events.
        * @param aKeyEvent Event to handled.
        * @param aType Type of the key event. 
        * @return Response code (EKeyWasConsumed, EKeyWasNotConsumed). 
        */
        virtual TKeyResponse HandleKeyEventL(
            const TKeyEvent& aKeyEvent,TEventCode aType);

    private: //Data
        CAknNavigationControlContainer* iNaviPane;
        CAknTabGroup*                   iTabGroup;
        CAknNavigationDecorator*        iDecoratedTabGroup;
    };

#endif


