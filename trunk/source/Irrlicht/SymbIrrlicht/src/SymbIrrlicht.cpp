/*
============================================================================
 Name        : SymbIrrlicht.cpp
 Author      : 
 Version     :
 Copyright   : GPL v2
 Description : Application entry point
============================================================================
*/

// INCLUDE FILES
#include <eikstart.h>
#include "SymbIrrlichtApplication.h"


LOCAL_C CApaApplication* NewApplication()
	{
	return new CSymbIrrlichtApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );
	}
