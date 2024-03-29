// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_IMAGE_LOADER_TGA_H_INCLUDED__
#define __C_IMAGE_LOADER_TGA_H_INCLUDED__

#include "IImageLoader.h"


namespace irr
{
namespace video
{


// byte-align structures
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__) 
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#elif defined(__SYMBIAN32__)    
#   if defined(__WINS__)
#     define PACK_STRUCT 
#     pragma pack(1)
#   elif defined(__ARMCC__)
#     define PACK_STRUCT 
#   else 
#	  define PACK_STRUCT	__attribute__((packed,aligned(1)))
#   endif
#else
#	define PACK_STRUCT
#endif

	// these structs are also used in the TGA writer
#if defined(__SYMBIAN32__) && defined(__ARMCC__)
__packed 
#endif  
	 struct STGAHeader{
	    u8 IdLength;
	    u8 ColorMapType;
	    u8 ImageType;
	    u8 FirstEntryIndex[2];
	    u16 ColorMapLength;
	    u8 ColorMapEntrySize;
	    u8 XOrigin[2];
	    u8 YOrigin[2];
	    u16 ImageWidth;
	    u16 ImageHeight;
	    u8 PixelDepth;
	    u8 ImageDescriptor;
    } PACK_STRUCT;

	struct STGAFooter
	{
		u32 ExtensionOffset;
		u32 DeveloperOffset;
		c8  Signature[18];
	} PACK_STRUCT;

// Default alignment
#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__) 
#	pragma pack( pop, packing )
#elif defined(__SYMBIAN32__) && defined(__WINS__)
#   pragma pack(4) //default alignment in Project settings 
#endif

#undef PACK_STRUCT

/*!
	Surface Loader for targa images
*/
class CImageLoaderTGA : public IImageLoader
{
public:

	//! constructor
	CImageLoaderTGA();

	//! destructor
	virtual ~CImageLoaderTGA();

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".tga")
	virtual bool isALoadableFileExtension(const c8* fileName);

	//! returns true if the file maybe is able to be loaded by this class
	virtual bool isALoadableFileFormat(irr::io::IReadFile* file);

	//! creates a surface from the file
	virtual IImage* loadImage(irr::io::IReadFile* file);

private:

	//! loads a compressed tga. Was written and sent in by Jon Pry, thank you very much!
	u8* loadCompressedImage(irr::io::IReadFile *file, const STGAHeader& header);

};


} // end namespace video
} // end namespace irr


#endif

