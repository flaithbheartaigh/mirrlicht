/*
============================================================================
 Name        : CSymbIrrlichtApp from SymbIrrlichtApp.h
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : Declares main application class.
============================================================================
*/

#ifndef SYMBIRRLICHTAPP_H
#define SYMBIRRLICHTAPP_H

// INCLUDES
#include <aknapp.h>

// CONSTANTS
// UID of the application
const TUid KUidSymbIrrlicht = { 0xebf1104c };

// CLASS DECLARATION

/**
* CSymbIrrlichtApp application class.
* Provides factory to create concrete document object.
* 
*/
class CSymbIrrlichtApp : public CAknApplication
    {
    
    public: // Functions from base classes
    private:

        /**
        * From CApaApplication, creates CSymbIrrlichtDocument document object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
        
        /**
        * From CApaApplication, returns application's UID (KUidSymbIrrlicht).
        * @return The value of KUidSymbIrrlicht.
        */
        TUid AppDllUid() const;
    };

#endif



