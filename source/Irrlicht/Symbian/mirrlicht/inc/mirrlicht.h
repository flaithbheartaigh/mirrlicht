/*
============================================================================
 Name        : mirrlicht.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : mirrlicht.h - Cmirrlicht class header
               Defines the API for mirrlicht.dll
============================================================================
*/

#ifndef __MIRRLICHT_H__
#define __MIRRLICHT_H__

// INCLUDES
#include <e32base.h>    // CBase
#include <e32std.h>     // TBuf

// CONSTANTS
const TInt KmirrlichtBufferLength = 15;
typedef TBuf<KmirrlichtBufferLength> TmirrlichtExampleString;

//  CLASS DEFINITIONS

class Cmirrlicht : public CBase
    {
    public:     // new functions
        IMPORT_C static Cmirrlicht* NewL();
        IMPORT_C static Cmirrlicht* NewLC();
        IMPORT_C ~Cmirrlicht();

    public:     // new functions, example API
        IMPORT_C TVersion Version() const;
        IMPORT_C void ExampleFuncAddCharL(const TChar& aChar);
        IMPORT_C void ExampleFuncRemoveLast();
	  IMPORT_C void ExampleFuncClearBuffer();
        IMPORT_C const TPtrC ExampleFuncString() const;

    private:    // new functions
        Cmirrlicht();
        void ConstructL();

    private:    // data
        TmirrlichtExampleString* iString;
    };


#endif  // __MIRRLICHT_H__


