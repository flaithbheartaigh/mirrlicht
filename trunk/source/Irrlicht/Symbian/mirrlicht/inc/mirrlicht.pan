/*
============================================================================
 Name        : mirrlicht.pan
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Panic codes
============================================================================
*/

#ifndef __MIRRLICHT_PAN__
#define __MIRRLICHT_PAN__

/** mirrlicht panic codes */
enum TmirrlichtPanic
    {
    EmirrlichtNullPointer
    };

inline void Panic(TmirrlichtPanic aReason)
    {
	_LIT(applicationName,"m");
    User::Panic(applicationName, aReason);
    }

#endif  // __MIRRLICHT_PAN__


