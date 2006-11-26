/*
============================================================================
 Name        : mHelloWorld.rss
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Main application class
============================================================================
*/

#ifndef __MHELLOWORLDAPPUI_H__
#define __MHELLOWORLDAPPUI_H__

// INCLUDES
#include <aknappui.h>

#include "mirrlicht.h"

// FORWARD DECLARATIONS
class CmHelloWorldAppView;


// CLASS DECLARATION
/**
* CmHelloWorldAppUi application UI class.
* Interacts with the user through the UI and request message processing
* from the handler class
*/
class CmHelloWorldAppUi : public CAknAppUi
    {
    public: // Constructors and destructor

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL();

        /**
        * CmHelloWorldAppUi.
        * C++ default constructor. This needs to be public due to
        * the way the framework constructs the AppUi
        */
        CmHelloWorldAppUi();

        /**
        * ~CmHelloWorldAppUi.
        * Virtual Destructor.
        */
        virtual ~CmHelloWorldAppUi();

    private:  // Functions from base classes

        /**
        * From CEikAppUi, HandleCommandL.
        * Takes care of command handling.
        * @param aCommand Command to be handled.
        */
        void HandleCommandL( TInt aCommand );

        /**
        *  HandleStatusPaneSizeChange.
        *  Called by the framework when the application status pane
 		*  size is changed.
        */

		void HandleStatusPaneSizeChange();
        
    private: // Data

        /**
        * The application view
        * Owned by CmHelloWorldAppUi
        */
        CmHelloWorldAppView* iAppView;

        /**
        * The application engine
        * Owned by CmHelloWorldAppUi
        */        
        Cmirrlicht*    iEngine;
    };

#endif // __MHELLOWORLDAPPUI_H__

// End of File
