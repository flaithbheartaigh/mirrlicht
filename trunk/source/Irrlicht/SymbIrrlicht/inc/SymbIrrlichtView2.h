/*
============================================================================
 Name        : CSymbIrrlichtView2 from SymbIrrlichtView2.h
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : Declares view for application.
============================================================================
*/

#ifndef SYMBIRRLICHTVIEW2_H
#define SYMBIRRLICHTVIEW2_H

// INCLUDES
#include <aknview.h>


// CONSTANTS
// UID of view
const TUid KView2Id = {2};

// FORWARD DECLARATIONS
class CSymbIrrlichtContainer2;

// CLASS DECLARATION

/**
*  CSymbIrrlichtView2 view class.
* 
*/
class CSymbIrrlichtView2 : public CAknView
    {
    public: // Constructors and destructor

        /**
        * EPOC default constructor.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CSymbIrrlichtView2();

    public: // Functions from base classes
                        
		/**
        * From CAknView returns Uid of View
        * @return TUid uid of the view
        */
        TUid Id() const;

        /**
        * From MEikMenuObserver delegate commands from the menu
        * @param aCommand a command emitted by the menu 
        * @return void
        */
        void HandleCommandL(TInt aCommand);

        /**
        * From CAknView reaction if size change
        * @return void
        */
        void HandleClientRectChange();

    private:

        /**
        * From CAknView activate the view
        * @param aPrevViewId 
        * @param aCustomMessageId 
        * @param aCustomMessage 
        * @return void
        */
        void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,
            const TDesC8& aCustomMessage);

        /**
        * From CAknView deactivate the view (free resources)
        * @return void
        */
        void DoDeactivate();

    private: // Data
        CSymbIrrlichtContainer2* iContainer;
    };

#endif


