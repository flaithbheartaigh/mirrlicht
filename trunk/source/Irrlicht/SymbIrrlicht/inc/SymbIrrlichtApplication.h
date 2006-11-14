/*
============================================================================
 Name        : SymbIrrlichtApplication.h
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : Main application class
============================================================================
*/

#ifndef __SYMBIRRLICHTAPPLICATION_H__
#define __SYMBIRRLICHTAPPLICATION_H__

// INCLUDES
#include <aknapp.h>

// CLASS DECLARATION

/**
* CSymbIrrlichtApplication application class.
* Provides factory to create concrete document object.
* An instance of CSymbIrrlichtApplication is the application part of the
* AVKON application framework for the SymbIrrlicht example application.
*/
class CSymbIrrlichtApplication : public CAknApplication
    {
    public: // Functions from base classes

        /**
        * From CApaApplication, AppDllUid.
        * @return Application's UID (KUidSymbIrrlichtApp).
        */
        TUid AppDllUid() const;

    protected: // Functions from base classes

        /**
        * From CApaApplication, CreateDocumentL.
        * Creates CSymbIrrlichtDocument document object. The returned
        * pointer in not owned by the CSymbIrrlichtApplication object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
    };

#endif // __SYMBIRRLICHTAPPLICATION_H__

// End of File
