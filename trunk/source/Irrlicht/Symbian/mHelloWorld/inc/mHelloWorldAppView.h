/*
============================================================================
 Name        : mHelloWorld.rss
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Main application class
============================================================================
*/

#ifndef __MHELLOWORLDAPPVIEW_H__
#define __MHELLOWORLDAPPVIEW_H__

// INCLUDES
#include <coecntrl.h>

// CLASS DECLARATION
class CmHelloWorldAppView : public CCoeControl
    {
    public: // New methods

        /**
        * NewL.
        * Two-phased constructor.
        * Create a CmHelloWorldAppView object, which will draw itself to aRect.
        * @param aRect The rectangle this view will be drawn to.
        * @return a pointer to the created instance of CmHelloWorldAppView.
        */
        static CmHelloWorldAppView* NewL( const TRect& aRect );

        /**
        * NewLC.
        * Two-phased constructor.
        * Create a CmHelloWorldAppView object, which will draw itself
        * to aRect.
        * @param aRect Rectangle this view will be drawn to.
        * @return A pointer to the created instance of CmHelloWorldAppView.
        */
        static CmHelloWorldAppView* NewLC( const TRect& aRect );

        /**
        * ~CmHelloWorldAppView
        * Virtual Destructor.
        */
        virtual ~CmHelloWorldAppView();

    public:  // Functions from base classes

        /**
        * From CCoeControl, Draw
        * Draw this CmHelloWorldAppView to the screen.
        * @param aRect the rectangle of this view that needs updating
        */
        void Draw( const TRect& aRect ) const;

        /**
        * From CoeControl, SizeChanged.
        * Called by framework when the view size is changed.
        */
        virtual void SizeChanged();

    private: // Constructors

        /**
        * ConstructL
        * 2nd phase constructor.
        * Perform the second phase construction of a
        * CmHelloWorldAppView object.
        * @param aRect The rectangle this view will be drawn to.
        */
        void ConstructL(const TRect& aRect);

        /**
        * CmHelloWorldAppView.
        * C++ default constructor.
        */
        CmHelloWorldAppView();

    };

#endif // __MHELLOWORLDAPPVIEW_H__

// End of File
