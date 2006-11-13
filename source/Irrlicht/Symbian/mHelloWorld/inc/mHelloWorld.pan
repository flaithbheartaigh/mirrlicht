/*
============================================================================
 Name        : mHelloWorld.rss
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Main application class
============================================================================
*/

#ifndef __MHELLOWORLD_PAN__
#define __MHELLOWORLD_PAN__

/** mHelloWorld application panic codes */
enum TmHelloWorldPanics
    {
    EmHelloWorldUi = 1
    // add further panics here
    };

inline void Panic(TmHelloWorldPanics aReason)
    {
    _LIT(applicationName,"mHelloWorld");
    User::Panic(applicationName, aReason);
    }

#endif // __MHELLOWORLD_PAN__
