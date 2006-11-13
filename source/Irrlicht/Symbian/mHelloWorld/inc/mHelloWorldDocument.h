/*
============================================================================
 Name        : mHelloWorld.rss
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Main application class
============================================================================
*/

#ifndef __MHELLOWORLDDOCUMENT_H__
#define __MHELLOWORLDDOCUMENT_H__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CmHelloWorldAppUi;
class CEikApplication;


// CLASS DECLARATION

/**
* CmHelloWorldDocument application class.
* An instance of class CmHelloWorldDocument is the Document part of the
* AVKON application framework for the mHelloWorld example application.
*/
class CmHelloWorldDocument : public CAknDocument
    {
    public: // Constructors and destructor

        /**
        * NewL.
        * Two-phased constructor.
        * Construct a CmHelloWorldDocument for the AVKON application aApp
        * using two phase construction, and return a pointer
        * to the created object.
        * @param aApp Application creating this document.
        * @return A pointer to the created instance of CmHelloWorldDocument.
        */
        static CmHelloWorldDocument* NewL( CEikApplication& aApp );

        /**
        * NewLC.
        * Two-phased constructor.
        * Construct a CmHelloWorldDocument for the AVKON application aApp
        * using two phase construction, and return a pointer
        * to the created object.
        * @param aApp Application creating this document.
        * @return A pointer to the created instance of CmHelloWorldDocument.
        */
        static CmHelloWorldDocument* NewLC( CEikApplication& aApp );

        /**
        * ~CmHelloWorldDocument
        * Virtual Destructor.
        */
        virtual ~CmHelloWorldDocument();

    public: // Functions from base classes

        /**
        * CreateAppUiL
        * From CEikDocument, CreateAppUiL.
        * Create a CmHelloWorldAppUi object and return a pointer to it.
        * The object returned is owned by the Uikon framework.
        * @return Pointer to created instance of AppUi.
        */
        CEikAppUi* CreateAppUiL();

    private: // Constructors

        /**
        * ConstructL
        * 2nd phase constructor.
        */
        void ConstructL();

        /**
        * CmHelloWorldDocument.
        * C++ default constructor.
        * @param aApp Application creating this document.
        */
        CmHelloWorldDocument( CEikApplication& aApp );

    };

#endif // __MHELLOWORLDDOCUMENT_H__

// End of File
