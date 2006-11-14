/*
============================================================================
 Name        : CSymbIrrlichtDocument from SymbIrrlichtDocument.h
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : Declares document for application.
============================================================================
*/

#ifndef SYMBIRRLICHTDOCUMENT_H
#define SYMBIRRLICHTDOCUMENT_H

// INCLUDES
#include <akndoc.h>
   
// CONSTANTS

// FORWARD DECLARATIONS
class  CEikAppUi;

// CLASS DECLARATION

/**
*  CSymbIrrlichtDocument application class.
*/
class CSymbIrrlichtDocument : public CAknDocument
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CSymbIrrlichtDocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        */
        virtual ~CSymbIrrlichtDocument();

    public: // New functions

    public: // Functions from base classes
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * EPOC default constructor.
        */
        CSymbIrrlichtDocument(CEikApplication& aApp);
        void ConstructL();

    private:

        /**
        * From CEikDocument, create CSymbIrrlichtAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
    };

#endif



