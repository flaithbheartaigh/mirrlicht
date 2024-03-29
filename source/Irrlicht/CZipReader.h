// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_ZIP_READER_H_INCLUDED__
#define __C_ZIP_READER_H_INCLUDED__

#include "IUnknown.h"
#include "IReadFile.h"
#include "irrArray.h"
#include "irrString.h"
#include "IFileSystem.h"

namespace irr
{
namespace io
{

	const s16 ZIP_FILE_ENCRYPTED =		0x0001; // set if the file is encrypted
	const s16 ZIP_INFO_IN_DATA_DESCRITOR =	0x0008; // the fields crc-32, compressed size
														// and uncompressed size are set to zero in the local
														// header

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined (__BCPLUSPLUS__) 
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
#	error compiler not supported
#endif

#if defined(__SYMBIAN32__) && defined(__ARMCC__)
	__packed 
#endif 
	struct SZIPFileDataDescriptor
	{
		s32 CRC32;
		s32 CompressedSize;
		s32 UncompressedSize;
	} PACK_STRUCT;

	struct SZIPFileHeader
	{
		s32 Sig;
		s16 VersionToExtract;
		s16 GeneralBitFlag;
		s16 CompressionMethod;
		s16 LastModFileTime;
		s16 LastModFileDate;
		SZIPFileDataDescriptor DataDescriptor;
		s16 FilenameLength;
		s16 ExtraFieldLength;
	} PACK_STRUCT;

// Default alignment
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined (__BCPLUSPLUS__) 
#	pragma pack( pop, packing )
#elif defined(__SYMBIAN32__) && defined(__WINS__)
#   pragma pack(4) //default alignment in symbian project settings 
#endif

#undef PACK_STRUCT


	struct SZipFileEntry
	{
		core::stringc zipFileName;
		core::stringc simpleFileName;
		core::stringc path;
		s32 fileDataPosition; // position of compressed data in file
		SZIPFileHeader header;

		bool operator < (const SZipFileEntry& other) const
		{
			return simpleFileName < other.simpleFileName;
		}


		bool operator == (const SZipFileEntry& other) const
		{
			return simpleFileName == other.simpleFileName;
		}
	};



/*!
	Zip file Reader written April 2002 by N.Gebhardt.
	Doesn't decompress data, only reads the file and is able to
	open uncompressed entries.
*/
	class CZipReader : public virtual IUnknown
	{
	public:

		CZipReader(IReadFile* file, bool ignoreCase, bool ignorePaths);
		virtual ~CZipReader();

		//! opens a file by file name
		virtual IReadFile* openFile(const c8* filename);

		//! opens a file by index
		IReadFile* openFile(s32 index);

		//! returns count of files in archive
		s32 getFileCount();

		//! returns data of file
		const SZipFileEntry* getFileInfo(s32 index) const;

		//! returns fileindex
		s32 findFile(const c8* filename);

	private:
		
		//! scans for a local header, returns false if there is no more local file header.
		bool scanLocalHeader();
		IReadFile* File;

	protected:

		//! splits filename from zip file into useful filenames and paths
		void extractFilename(SZipFileEntry* entry);

		//! deletes the path from a filename
		void deletePathFromFilename(core::stringc& filename);


		bool IgnoreCase;
		bool IgnorePaths;
		core::array<SZipFileEntry> FileList;
	};


	class CUnZipReader : public CZipReader
	{
	public:

		CUnZipReader( IFileSystem *parent, const c8* basename, bool ignoreCase, bool ignorePaths);

		//! opens a file by file name
		virtual IReadFile* openFile(const c8* filename);

		//! returns fileindex
		s32 findFile(const c8* filename);

	private:

		IFileSystem *Parent;
		void buildDirectory ();

		core::stringc Base;
	};

} // end namespace io
} // end namespace irr

#endif

