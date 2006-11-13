/*
============================================================================
 Name        : mHelloWorld.rss
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Main application class
============================================================================
*/

// INCLUDE FILES
#include <eikstart.h>
#include "mHelloWorldApplication.h"


LOCAL_C CApaApplication* NewApplication()
	{
	return new CmHelloWorldApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );
	}
