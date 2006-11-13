/*
============================================================================
 Name        : Cmirrlicht from mirrlicht.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Cmirrlicht DLL source
============================================================================
*/

//  Include Files  

#include "mirrlicht.h"	// Cmirrlicht
#include "mirrlicht.pan"      	// panic codes

//  Member Functions

EXPORT_C Cmirrlicht* Cmirrlicht::NewLC()
    {
    Cmirrlicht* self = new (ELeave) Cmirrlicht;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }


EXPORT_C Cmirrlicht* Cmirrlicht::NewL()
    {
    Cmirrlicht* self = Cmirrlicht::NewLC();
    CleanupStack::Pop(self);
    return self;
    }


Cmirrlicht::Cmirrlicht()
    {
    // note, CBase initialises all member variables to zero
    }


void Cmirrlicht::ConstructL()
    {
    // second phase constructor, anything that may leave must be constructed here
    iString = new (ELeave) TmirrlichtExampleString;
    }


EXPORT_C Cmirrlicht::~Cmirrlicht()
    {
    delete iString;
    }


EXPORT_C TVersion Cmirrlicht::Version() const
    {
    // Version number of example API
    const TInt KMajor = 1;
    const TInt KMinor = 0;
    const TInt KBuild = 1;
    return TVersion(KMajor, KMinor, KBuild);
    }


EXPORT_C void Cmirrlicht::ExampleFuncAddCharL(const TChar& aChar)
    {
    __ASSERT_ALWAYS(iString != NULL, Panic(EmirrlichtNullPointer));

    if (iString->Length() >= KmirrlichtBufferLength)
        {
        User::Leave(KErrTooBig);
        }

    iString->Append(aChar);
    }


EXPORT_C void Cmirrlicht::ExampleFuncRemoveLast()
    {
    __ASSERT_ALWAYS(iString != NULL, Panic(EmirrlichtNullPointer));

    if (iString->Length() > 0)
        {
        iString->SetLength(iString->Length() - 1);
        }
    }
EXPORT_C void Cmirrlicht::ExampleFuncClearBuffer()
	{
	iString->Zero();
    }

EXPORT_C const TPtrC Cmirrlicht::ExampleFuncString() const
    {
    __ASSERT_ALWAYS(iString != NULL, Panic(EmirrlichtNullPointer));
    return *iString;
    }



