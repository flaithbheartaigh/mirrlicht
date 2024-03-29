// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_3DS_MESH_FILE_LOADER_H_INCLUDED__
#define __C_3DS_MESH_FILE_LOADER_H_INCLUDED__

#include "IMeshLoader.h"
#include "IFileSystem.h"
#include "IVideoDriver.h"
#include "irrString.h"
#include "SMesh.h"
#include "IMeshManipulator.h"
#include "matrix4.h"

namespace irr
{
namespace scene
{

//! Meshloader capable of loading 3ds meshes.
class C3DSMeshFileLoader : public IMeshLoader
{
public:

	//! Constructor
	C3DSMeshFileLoader(IMeshManipulator* manip,io::IFileSystem* fs, video::IVideoDriver* driver);

	//! destructor
	virtual ~C3DSMeshFileLoader();

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".cob")
	virtual bool isALoadableFileExtension(const c8* fileName);

	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IUnknown::drop() for more information.
	virtual IAnimatedMesh* createMesh(irr::io::IReadFile* file);

private:

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
	#	error compiler not supported
	#endif

	#if defined(__SYMBIAN32__) && defined(__ARMCC__)
	__packed 
	#endif 
	struct ChunkHeader
	{
		u16 id;
		s32 length;
	} PACK_STRUCT;

	// Default alignment
	#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__) 
	#	pragma pack( pop, packing )
	#elif defined(__SYMBIAN32__) && defined(__WINS__)
	#   pragma pack(4) //default alignment in Project settings 
	#endif

	#undef PACK_STRUCT


	struct ChunkData
	{
		ChunkData() : read(0) {}

		ChunkHeader header;
		s32 read;
	};

	struct SCurrentMaterial
	{
		SCurrentMaterial() {};

		~SCurrentMaterial() { 	};

		void clear() {
			Material=video::SMaterial();
			Name="";
			Filename[0]="";
			Filename[1]="";
			Filename[2]="";
			Filename[3]="";
			Filename[4]="";
		}

		video::SMaterial Material;
		core::stringc Name;
		core::stringc Filename[5];
	};

	struct SMaterialGroup
	{
		SMaterialGroup() : faceCount(0), faces(0) {};

		SMaterialGroup(const SMaterialGroup& o)
		{
			*this = o;
		}

		~SMaterialGroup() 
		{ 
			clear();
		}

		void clear() 
		{ 
			if (faces) delete [] faces;
			faces = 0;
			faceCount = 0;
		}

		void operator =(const SMaterialGroup& o)
		{
			MaterialName = o.MaterialName;
			faceCount = o.faceCount;
			faces = new s16[faceCount];
			for (s32 i=0; i<faceCount; ++i)
				faces[i] = o.faces[i];
		}

		core::stringc MaterialName;
		u16 faceCount;
		s16* faces;
	};

	bool readChunk(io::IReadFile* file, ChunkData* parent);
	bool readMaterialChunk(io::IReadFile* file, ChunkData* parent);
	bool readFrameChunk(io::IReadFile* file, ChunkData* parent);
	bool readTrackChunk(io::IReadFile* file, ChunkData& data,
				IMeshBuffer* mb, const core::vector3df& pivot);
	bool readObjectChunk(io::IReadFile* file, ChunkData* parent);
	bool readPercentageChunk(io::IReadFile* file, ChunkData* chunk, f32& percentage);
	bool readColorChunk(io::IReadFile* file, ChunkData* chunk, video::SColor& out);

	void readChunkData(io::IReadFile* file, ChunkData& data);
	void readString(io::IReadFile* file, ChunkData& data, core::stringc& out);
	void readVertices(io::IReadFile* file, ChunkData& data);
	void readIndices(io::IReadFile* file, ChunkData& data);
	void readMaterialGroup(io::IReadFile* file, ChunkData& data);
	void readTextureCoords(io::IReadFile* file, ChunkData& data);

	void composeObject(io::IReadFile* file, const core::stringc& name);
	void loadMaterials(io::IReadFile* file);
	void cleanUp();
	core::stringc getTextureFileName(const core::stringc& texture, core::stringc& model);

	io::IFileSystem* FileSystem;
	video::IVideoDriver* Driver;

	f32* Vertices;
	u16* Indices;
	core::array<u16> TempIndices;
	f32* TCoords;
	u16 CountVertices;
	u16 CountFaces; // = CountIndices/4
	u16 CountTCoords;
	core::array<SMaterialGroup> MaterialGroups;

	SCurrentMaterial CurrentMaterial;
	core::array<SCurrentMaterial> Materials;
	core::array<core::stringc> MeshBufferNames;
	core::matrix4 TransformationMatrix;

	SMesh* Mesh;

	IMeshManipulator* Manipulator;
};

} // end namespace scene
} // end namespace irr

#endif

