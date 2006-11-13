/*
============================================================================
 Name        : mHelloWorld.rss
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Main application class
============================================================================
*/

#ifndef __MHELLOWORLDAPPLICATION_H__
#define __MHELLOWORLDAPPLICATION_H__

// INCLUDES
#include <aknapp.h>

// CLASS DECLARATION

/**
* CmHelloWorldApplication application class.
* Provides factory to create concrete document object.
* An instance of CmHelloWorldApplication is the application part of the
* AVKON application framework for the mHelloWorld example application.
*/
class CmHelloWorldApplication : public CAknApplication
    {
    public: // Functions from base classes

        /**
        * From CApaApplication, AppDllUid.
        * @return Application's UID (KUidmHelloWorldApp).
        */
        TUid AppDllUid() const;

    protected: // Functions from base classes

        /**
        * From CApaApplication, CreateDocumentL.
        * Creates CmHelloWorldDocument document object. The returned
        * pointer in not owned by the CmHelloWorldApplication object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
    };

#endif // __MHELLOWORLDAPPLICATION_H__

// End of File
