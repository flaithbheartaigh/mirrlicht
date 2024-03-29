// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "C3DSMeshFileLoader.h"
#include "os.h"
#include "SMeshBuffer.h"
#include "SAnimatedMesh.h"
#include "IReadFile.h"

namespace irr
{
namespace scene
{


// Primary chunk
const u16 C3DS_MAIN3DS = 0x4D4D;

// Main Chunks
const u16 C3DS_EDIT3DS = 0x3D3D;
const u16 C3DS_KEYF3DS = 0xB000;
const u16 C3DS_VERSION = 0x0002;
const u16 C3DS_MESHVERSION = 0x3D3E;

// sub chunks of C3DS_EDIT3DS
const u16 C3DS_EDIT_MATERIAL = 0xAFFF;
const u16 C3DS_EDIT_OBJECT   = 0x4000;

// sub chunks of C3DS_EDIT_MATERIAL
const u16 C3DS_MATNAME       = 0xA000; 
const u16 C3DS_MATAMBIENT    = 0xA010;
const u16 C3DS_MATDIFFUSE    = 0xA020;
const u16 C3DS_MATSPECULAR   = 0xA030;
const u16 C3DS_MATSHININESS  = 0xA040;
const u16 C3DS_MATSHIN2PCT   = 0xA041;
const u16 C3DS_TRANSPARENCY  = 0xA050;
const u16 C3DS_TWO_SIDE      = 0xA081;
const u16 C3DS_WIRE          = 0xA085;
const u16 C3DS_SHADING       = 0xA100;
const u16 C3DS_MATTEXMAP     = 0xA200;
const u16 C3DS_MATSPECMAP    = 0xA204;
const u16 C3DS_MATOPACMAP    = 0xA210;
const u16 C3DS_MATREFLMAP    = 0xA220;
const u16 C3DS_MATBUMPMAP    = 0xA230;
const u16 C3DS_MATMAPFILE    = 0xA300;
const u16 C3DS_MAT_TEXTILING = 0xA351;
const u16 C3DS_MAT_USCALE    = 0xA354;
const u16 C3DS_MAT_VSCALE    = 0xA356;
const u16 C3DS_MAT_UOFFSET   = 0xA358;
const u16 C3DS_MAT_VOFFSET   = 0xA35A;

// subs of C3DS_EDIT_OBJECT
const u16 C3DS_OBJTRIMESH    = 0x4100;

// subs of C3DS_OBJTRIMESH
const u16 C3DS_TRIVERT       = 0x4110;
const u16 C3DS_POINTFLAGARRAY= 0x4111;
const u16 C3DS_TRIFACE       = 0x4120;
const u16 C3DS_TRIFACEMAT    = 0x4130;
const u16 C3DS_TRIUV         = 0x4140;
const u16 C3DS_TRISMOOTH     = 0x4150;
const u16 C3DS_TRIMATRIX     = 0x4160;
const u16 C3DS_MESHCOLOR     = 0x4165;
const u16 C3DS_DIRECT_LIGHT  = 0x4600;
const u16 C3DS_DL_INNER_RANGE= 0x4659;
const u16 C3DS_DL_OUTER_RANGE= 0x465A;
const u16 C3DS_DL_MULTIPLIER = 0x465B;
const u16 C3DS_CAMERA        = 0x4700;
const u16 C3DS_CAM_SEE_CONE  = 0x4710;
const u16 C3DS_CAM_RANGES    = 0x4720;

// subs of C3DS_KEYF3DS
const u16 C3DS_KF_HDR        = 0xB00A;
const u16 C3DS_AMBIENT_TAG   = 0xB001;
const u16 C3DS_OBJECT_TAG    = 0xB002;
const u16 C3DS_CAMERA_TAG    = 0xB003;
const u16 C3DS_TARGET_TAG    = 0xB004;
const u16 C3DS_LIGHTNODE_TAG = 0xB005;
const u16 C3DS_KF_SEG        = 0xB008;
const u16 C3DS_KF_CURTIME    = 0xB009;
const u16 C3DS_KF_NODE_HDR   = 0xB010;
const u16 C3DS_PIVOTPOINT    = 0xB013;
const u16 C3DS_BOUNDBOX      = 0xB014;
const u16 C3DS_POS_TRACK_TAG = 0xB020;
const u16 C3DS_ROT_TRACK_TAG = 0xB021;
const u16 C3DS_SCL_TRACK_TAG = 0xB022;
const u16 C3DS_NODE_ID       = 0xB030;

// different color chunk types
const u16 C3DS_COL_RGB    = 0x0010;
const u16 C3DS_COL_TRU    = 0x0011;
const u16 C3DS_COL_LIN_24 = 0x0012;
const u16 C3DS_COL_LIN_F  = 0x0013;

// percentage chunk types
const u16 C3DS_PERCENTAGE_I = 0x0030;
const u16 C3DS_PERCENTAGE_F = 0x0031;


//! Constructor
C3DSMeshFileLoader::C3DSMeshFileLoader(IMeshManipulator* manip,io::IFileSystem* fs, video::IVideoDriver* driver)
: FileSystem(fs), Driver(driver), Vertices(0), Indices(0), TCoords(0),
	CountVertices(0), CountFaces(0), CountTCoords(0), Mesh(0), Manipulator(manip)
{
	TransformationMatrix.makeIdentity();
	if (FileSystem)
		FileSystem->grab();

	if (Driver)
		Driver->grab();
}



//! destructor
C3DSMeshFileLoader::~C3DSMeshFileLoader()
{
	if (FileSystem)
		FileSystem->drop();

	if (Driver)
		Driver->drop();

	if (Vertices)
		delete [] Vertices;

	if (Indices)
		delete [] Indices;

	if (TCoords)
		delete [] TCoords;

	if (Mesh)
		Mesh->drop();
}



//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool C3DSMeshFileLoader::isALoadableFileExtension(const c8* filename)
{
	return strstr(filename, ".3ds")!=0;
}



//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IUnknown::drop() for more information.
IAnimatedMesh* C3DSMeshFileLoader::createMesh(io::IReadFile* file)
{
	ChunkData data;

	readChunkData(file, data);

	if (data.header.id != C3DS_MAIN3DS )
		return 0;

	CurrentMaterial.clear();
	Materials.clear();
	MeshBufferNames.clear();
	cleanUp();

	if (Mesh)
		Mesh->drop();

	Mesh = new SMesh();

	if (readChunk(file, &data))
	{
		// success
		SAnimatedMesh* am = new SAnimatedMesh();
		am->Type = EAMT_3DS;

		for (u32 i=0; i<Mesh->getMeshBufferCount(); ++i)
		{
			SMeshBuffer* mb = ((SMeshBuffer*)Mesh->getMeshBuffer(i));
			// drop empty buffers
			if (mb->getIndexCount() == 0 || mb->getVertexCount() == 0)
			{
				Mesh->MeshBuffers.erase(i--);
				mb->drop();
			}
			else
				mb->recalculateBoundingBox();
		}

		Mesh->recalculateBoundingBox();

		am->addMesh(Mesh);
		am->recalculateBoundingBox();
		Mesh->drop();
		Mesh = 0;
		return am;
	}

	Mesh->drop();
	Mesh = 0;

	return 0;
}


bool C3DSMeshFileLoader::readPercentageChunk(io::IReadFile* file,
					ChunkData* chunk, f32& percentage)
{
	ChunkData data;
	readChunkData(file, data);

	short intpercentage;
	float fpercentage;

	switch(data.header.id)
	{
	case C3DS_PERCENTAGE_I:
	{
		// read short
		file->read(&intpercentage, 2);
#ifdef __BIG_ENDIAN__
		intpercentage = os::Byteswap::byteswap(intpercentage);
#endif
		percentage=intpercentage/100.0f;
		data.read += 2;
	}
	break;
	case C3DS_PERCENTAGE_F:
	{
		// read float
		file->read(&fpercentage, sizeof(float));
		data.read += sizeof(float);
#ifdef __BIG_ENDIAN__
		percentage = os::Byteswap::byteswap(fpercentage);
#else
		percentage = (f32)fpercentage;
#endif
	}
	break;
	default:
	{
		// unknown percentage chunk
		os::Printer::log("Unknown percentage chunk in 3Ds file.", ELL_WARNING);
		file->seek(data.header.length - data.read, true);
		data.read += data.header.length - data.read;
	}
	}

	chunk->read += data.read;

	return true;
}

bool C3DSMeshFileLoader::readColorChunk(io::IReadFile* file, ChunkData* chunk,
					video::SColor& out)
{
	ChunkData data;
	readChunkData(file, data);

	u8 c[3];
	f32 cf[3];

	switch(data.header.id)
	{
	case C3DS_COL_TRU:
	case C3DS_COL_LIN_24:
	{
		// read 8 bit data
		file->read(c, sizeof(c));
		out.set(255, c[0], c[1], c[2]); 
		data.read += sizeof(c);
	}
	break;
	case C3DS_COL_RGB:
	case C3DS_COL_LIN_F:
	{
		// read float data
		file->read(cf, sizeof(cf));
#ifdef __BIG_ENDIAN__
		cf[0] = os::Byteswap::byteswap(cf[0]);
		cf[1] = os::Byteswap::byteswap(cf[1]);
		cf[2] = os::Byteswap::byteswap(cf[2]);
#endif
		out.set(255, (s32)(cf[0]*255.0f), (s32)(cf[1]*255.0f), (s32)(cf[2]*255.0f)); 
		data.read += sizeof(cf);
	}
	break;
	default:
	{
		// unknown color chunk size
		os::Printer::log("Unknown size of color chunk in 3Ds file.", ELL_WARNING);
		file->seek(data.header.length - data.read, true);
		data.read += data.header.length - data.read;
	}
	}

	chunk->read += data.read;

	return true;
}


bool C3DSMeshFileLoader::readMaterialChunk(io::IReadFile* file, ChunkData* parent)
{
	u16 matSection=0;

	while(parent->read < parent->header.length)
	{
		ChunkData data;
		readChunkData(file, data);

		switch(data.header.id)
		{
		case C3DS_MATNAME:
			{
				c8* c = new c8[data.header.length - data.read];
				file->read(c, data.header.length - data.read);

				if (strlen(c))
					CurrentMaterial.Name = c;

				data.read += data.header.length - data.read;
				delete [] c;
			}
			break;
		case C3DS_MATAMBIENT:
			readColorChunk(file, &data, CurrentMaterial.Material.AmbientColor);
			break;
		case C3DS_MATDIFFUSE:
			readColorChunk(file, &data, CurrentMaterial.Material.DiffuseColor);
			break;
		case C3DS_MATSPECULAR:
			readColorChunk(file, &data, CurrentMaterial.Material.SpecularColor);
			break;
		case C3DS_MATSHININESS:
			readPercentageChunk(file, &data, CurrentMaterial.Material.Shininess);
			break;
		case C3DS_TRANSPARENCY:
			{
				f32 percentage;
				readPercentageChunk(file, &data, percentage);
				if (percentage>0.0f)
				{
					CurrentMaterial.Material.MaterialTypeParam=percentage;
					CurrentMaterial.Material.MaterialType=video::EMT_TRANSPARENT_VERTEX_ALPHA;
				}
				else
				{
					CurrentMaterial.Material.MaterialType=video::EMT_SOLID;
				}
			}
			break;
		case C3DS_WIRE:
			CurrentMaterial.Material.Wireframe=true;
			break;
		case C3DS_TWO_SIDE:
			CurrentMaterial.Material.BackfaceCulling=false;
			break;
		case C3DS_SHADING:
			{
				s16 flags;
				file->read(&flags, 2);
#ifdef __BIG_ENDIAN__
				flags = os::Byteswap::byteswap(flags);
#endif
				switch (flags)
				{
					case 0:
						CurrentMaterial.Material.Wireframe=true;
						break;
					case 1:
						CurrentMaterial.Material.Wireframe=false;
						CurrentMaterial.Material.GouraudShading=false;
						break;
					case 2:
						CurrentMaterial.Material.Wireframe=false;
						CurrentMaterial.Material.GouraudShading=true;
						break;
					default:
						// phong and metal missing
						break;
				}
				data.read += data.header.length - data.read;
			}
			break;
		case C3DS_MATTEXMAP:
		case C3DS_MATSPECMAP:
		case C3DS_MATOPACMAP:
		case C3DS_MATREFLMAP:
		case C3DS_MATBUMPMAP:
			{
				matSection=data.header.id;
#if 0 // Should contain a percentage chunk, but does not work with the meshes
				f32 percentage=0;
				if (matSection!=C3DS_MATREFLMAP)
					readPercentageChunk(file, &data, percentage);
#endif
			}
			break;
		case C3DS_MATMAPFILE:
			{
				// read texture file name
				c8* c = new c8[data.header.length - data.read];
				file->read(c, data.header.length - data.read);
				if (matSection == C3DS_MATTEXMAP)
					CurrentMaterial.Filename[0] = c;
				else if (matSection == C3DS_MATSPECMAP)
					CurrentMaterial.Filename[1] = c;
				else if (matSection == C3DS_MATOPACMAP)
					CurrentMaterial.Filename[2] = c;
				else if (matSection == C3DS_MATREFLMAP)
					CurrentMaterial.Filename[3] = c;
				else if (matSection == C3DS_MATBUMPMAP)
					CurrentMaterial.Filename[4] = c;
				data.read += data.header.length - data.read;
				delete [] c;
			}
			break;
		case C3DS_MAT_TEXTILING:
			{
				s16 flags;
				file->read(&flags, 2);
#ifdef __BIG_ENDIAN__
				flags = os::Byteswap::byteswap(flags);
#endif
				data.read += 2;
			}
			break;
		case C3DS_MAT_USCALE:
		case C3DS_MAT_VSCALE:
		case C3DS_MAT_UOFFSET:
		case C3DS_MAT_VOFFSET:
			{
				f32 value;
				file->read(&value, 4);
#ifdef __BIG_ENDIAN__
				value = os::Byteswap::byteswap(value);
#endif
				data.read += 4;
			}
			break;
		default:
			// ignore chunk
			file->seek(data.header.length - data.read, true);
			data.read += data.header.length - data.read;
		}

		parent->read += data.read;
	}

	Materials.push_back(CurrentMaterial);
	CurrentMaterial.clear();

	return true;
}



bool C3DSMeshFileLoader::readTrackChunk(io::IReadFile* file, ChunkData& data,
					IMeshBuffer* mb, const core::vector3df& pivot)
{
	u16 flags;
	u32 flags2;
	// Track flags
	file->read(&flags, 2);
#ifdef __BIG_ENDIAN__
	flags = os::Byteswap::byteswap(flags);
#endif
	file->read(&flags2, 4);
#ifdef __BIG_ENDIAN__
	flags2 = os::Byteswap::byteswap(flags2);
#endif
	file->read(&flags2, 4);
#ifdef __BIG_ENDIAN__
	flags2 = os::Byteswap::byteswap(flags2);
#endif
	// Num keys
	file->read(&flags2, 4);
#ifdef __BIG_ENDIAN__
	flags2 = os::Byteswap::byteswap(flags2);
#endif
	file->read(&flags2, 4);
#ifdef __BIG_ENDIAN__
	flags2 = os::Byteswap::byteswap(flags2);
#endif
	// TCB flags
	file->read(&flags, 2);
#ifdef __BIG_ENDIAN__
	flags = os::Byteswap::byteswap(flags);
#endif
	data.read += 20;

	f32 angle=0.0f;
	if (data.header.id== C3DS_ROT_TRACK_TAG)
	{
		// Angle
		file->read(&angle, sizeof(f32));
#ifdef __BIG_ENDIAN__
		angle = os::Byteswap::byteswap(angle);
#endif
		data.read += sizeof(f32);
	}
	core::vector3df vec;
	file->read(&vec.X, sizeof(f32));
	file->read(&vec.Y, sizeof(f32));
	file->read(&vec.Z, sizeof(f32));
#ifdef __BIG_ENDIAN__
	vec.X = os::Byteswap::byteswap(vec.X);
	vec.Y = os::Byteswap::byteswap(vec.X);
	vec.Z = os::Byteswap::byteswap(vec.X);
#endif
	data.read += 12;
	vec-=pivot;

	// apply transformation to mesh buffer
	if (false)//mb)
	{
		video::S3DVertex *vertices=(video::S3DVertex*)mb->getVertices();
		if (data.header.id==C3DS_POS_TRACK_TAG)
		{
			for (u32 i=0; i<mb->getVertexCount(); ++i)
				vertices[i].Pos+=vec;
		}
		else if (data.header.id==C3DS_ROT_TRACK_TAG)
		{
			//TODO
		}
		else if (data.header.id==C3DS_SCL_TRACK_TAG)
		{
			//TODO
		}
	}
	// skip further frames
	file->seek(data.header.length - data.read, true);
	data.read += data.header.length - data.read;
	return true;
}



bool C3DSMeshFileLoader::readFrameChunk(io::IReadFile* file, ChunkData* parent)
{
	ChunkData data;

	//KF_HDR is always at the beginning
	readChunkData(file, data);
	if (data.header.id != C3DS_KF_HDR)
		return false;
	else
	{
		u16 flags;
		file->read(&flags, 2);
#ifdef __BIG_ENDIAN__
		flags = os::Byteswap::byteswap(flags);
#endif
		c8* c = new c8[data.header.length - data.read-4];
		file->read(c, data.header.length - data.read-4);
		file->read(&flags, 2);
#ifdef __BIG_ENDIAN__
		flags = os::Byteswap::byteswap(flags);
#endif

		data.read += data.header.length - data.read;
		parent->read += data.read;
		delete [] c;
	}

	IMeshBuffer* mb=0;
	core::vector3df pivot,bboxCenter;
	while(parent->read < parent->header.length)
	{
		readChunkData(file, data);

		switch(data.header.id)
		{
		case C3DS_OBJECT_TAG:
			{
				mb=0;
				pivot.set(0.0f, 0.0f, 0.0f);
			}
			break;
		case C3DS_KF_SEG:
			{
				u32 flags;
				file->read(&flags, 4);
#ifdef __BIG_ENDIAN__
		flags = os::Byteswap::byteswap(flags);
#endif
				file->read(&flags, 4);
#ifdef __BIG_ENDIAN__
		flags = os::Byteswap::byteswap(flags);
#endif
				data.read += 8;
			}
			break;
		case C3DS_KF_NODE_HDR:
			{
				s16 flags;
				c8* c = new c8[data.header.length - data.read-6];
				file->read(c, data.header.length - data.read-6);

				// search mesh buffer to apply these transformations to
				for (u32 i=0; i<MeshBufferNames.size(); ++i)
				{
					if (MeshBufferNames[i]==c)
					{
						mb=Mesh->getMeshBuffer(i);
						break;
					}
				}

				file->read(&flags, 2);
#ifdef __BIG_ENDIAN__
				flags = os::Byteswap::byteswap(flags);
#endif
				file->read(&flags, 2);
#ifdef __BIG_ENDIAN__
				flags = os::Byteswap::byteswap(flags);
#endif
				file->read(&flags, 2);
#ifdef __BIG_ENDIAN__
				flags = os::Byteswap::byteswap(flags);
#endif
				data.read += data.header.length - data.read;
				delete [] c;
			}
			break;
		case C3DS_KF_CURTIME:
			{
				u32 flags;
				file->read(&flags, 4);
#ifdef __BIG_ENDIAN__
				flags = os::Byteswap::byteswap(flags);
#endif
				data.read += 4;
			}
			break;
		case C3DS_NODE_ID:
			{
				u16 flags;
				file->read(&flags, 2);
#ifdef __BIG_ENDIAN__
				flags = os::Byteswap::byteswap(flags);
#endif
				data.read += 2;
			}
			break;
		case C3DS_PIVOTPOINT:
			{
				file->read(&pivot.X, sizeof(f32));
				file->read(&pivot.Y, sizeof(f32));
				file->read(&pivot.Z, sizeof(f32));
#ifdef __BIG_ENDIAN__
				pivot.X = os::Byteswap::byteswap(pivot.X);
				pivot.Y = os::Byteswap::byteswap(pivot.Y);
				pivot.Z = os::Byteswap::byteswap(pivot.Z);
#endif
				data.read += 12;
			}
			break;
		case C3DS_BOUNDBOX:
			{
				core::aabbox3df bbox;
				// abuse bboxCenter as temporary variable
				file->read(&bboxCenter.X, sizeof(f32));
				file->read(&bboxCenter.Y, sizeof(f32));
				file->read(&bboxCenter.Z, sizeof(f32));
#ifdef __BIG_ENDIAN__
				bboxCenter.X = os::Byteswap::byteswap(bboxCenter.X);
				bboxCenter.Y = os::Byteswap::byteswap(bboxCenter.Y);
				bboxCenter.Z = os::Byteswap::byteswap(bboxCenter.Z);
#endif
				bbox.reset(bboxCenter);
				file->read(&bboxCenter.X, sizeof(f32));
				file->read(&bboxCenter.Y, sizeof(f32));
				file->read(&bboxCenter.Z, sizeof(f32));
#ifdef __BIG_ENDIAN__
				bboxCenter.X = os::Byteswap::byteswap(bboxCenter.X);
				bboxCenter.Y = os::Byteswap::byteswap(bboxCenter.Y);
				bboxCenter.Z = os::Byteswap::byteswap(bboxCenter.Z);
#endif
				bbox.addInternalPoint(bboxCenter);
				bboxCenter=bbox.getCenter();
				data.read += 24;
			}
			break;
		case C3DS_POS_TRACK_TAG:
		case C3DS_ROT_TRACK_TAG:
		case C3DS_SCL_TRACK_TAG:
			readTrackChunk(file, data, mb, bboxCenter-pivot);
			break;
		default:
			// ignore chunk
			file->seek(data.header.length - data.read, true);
			data.read += data.header.length - data.read;
		}

		parent->read += data.read;
		data.read=0;
	}

	return true;
}



bool C3DSMeshFileLoader::readChunk(io::IReadFile* file, ChunkData* parent)
{
	while(parent->read < parent->header.length)
	{
		ChunkData data;
		readChunkData(file, data);

		switch(data.header.id)
		{
		case C3DS_VERSION:
			{
				u16 version;
				file->read(&version, sizeof(u16));
#ifdef __BIG_ENDIAN__
				version = os::Byteswap::byteswap(version);
#endif
				file->seek(data.header.length - data.read - 2, true);
				data.read += data.header.length - data.read;
				if (version != 0x03)
					os::Printer::log("3ds file version is other than 3.", ELL_ERROR);
			}
			break;
		case C3DS_EDIT_MATERIAL:
			readMaterialChunk(file, &data);
			break;
		case C3DS_KEYF3DS:
			readFrameChunk(file, &data);
			break;
		case C3DS_EDIT3DS:
			break;
		case C3DS_MESHVERSION:
		case 0x01:
			{
				u32 version;
				file->read(&version, sizeof(u32));
#ifdef __BIG_ENDIAN__
				version = os::Byteswap::byteswap(version);
#endif
				data.read += sizeof(u32);
			}
			break;
		case C3DS_EDIT_OBJECT:
			{
				core::stringc name;
				readString(file, data, name);
				readObjectChunk(file, &data);
				composeObject(file, name);
			}
			break;

		default:
			// ignore chunk
			file->seek(data.header.length - data.read, true);
			data.read += data.header.length - data.read;
		}

		parent->read += data.read;
	}

	return true;
}



bool C3DSMeshFileLoader::readObjectChunk(io::IReadFile* file, ChunkData* parent)
{
	while(parent->read < parent->header.length)
	{
		ChunkData data;
		readChunkData(file, data);

		switch(data.header.id)
		{
		case C3DS_OBJTRIMESH:
			readObjectChunk(file, &data);
			break;

		case C3DS_TRIVERT: 
			readVertices(file, data);
			break;

		case C3DS_POINTFLAGARRAY:
			{
				u16 numVertex, flags;
				file->read(&numVertex, sizeof(u16));
#ifdef __BIG_ENDIAN__
				numVertex= os::Byteswap::byteswap(numVertex);
#endif
				for (u16 i=0; i<numVertex; ++i)
				{
					file->read(&flags, sizeof(u16));
#ifdef __BIG_ENDIAN__
					flags = os::Byteswap::byteswap(flags);
#endif
				}
				data.read += (numVertex+1)*sizeof(u16);
			}
			break;

		case C3DS_TRIFACE: 
			readIndices(file, data);
			readObjectChunk(file, &data); // read smooth and material groups
			break;

		case C3DS_TRIFACEMAT: 
			readMaterialGroup(file, data);
			break;

		case C3DS_TRIUV: // getting texture coordinates
			readTextureCoords(file, data);
			break;

		case C3DS_TRIMATRIX:
			{
				f32 mat[4][3];
				file->read(&mat, 12*sizeof(f32));
				TransformationMatrix.makeIdentity();
				for (int i=0; i<4; ++i)
				{
					for (int j=0; j<3; ++j)
					{
#ifdef __BIG_ENDIAN__
						TransformationMatrix(i,j)=os::Byteswap::byteswap(mat[i][j]);
#else
						TransformationMatrix(i,j)=mat[i][j];
#endif
					}
				}
				data.read += 12*sizeof(f32);
			}
			break;
		case C3DS_MESHCOLOR:
			{
				u8 flag;
				file->read(&flag, sizeof(u8));
				++data.read;
			}
			break;
		case C3DS_TRISMOOTH: // TODO
			{
				u32 flags;
				for (u16 i=0; i<CountFaces; ++i)
					file->read(&flags, sizeof(u32));
#ifdef __BIG_ENDIAN__
				for (u16 i=0; i<CountFaces; ++i)
					flags = os::Byteswap::byteswap(flags);
#endif
				data.read += CountFaces*sizeof(u32);
			}
			break;

		default:
			// ignore chunk
			file->seek(data.header.length - data.read, true);
			data.read += data.header.length - data.read;
		}

		parent->read += data.read;
	}

	return true;
}


void C3DSMeshFileLoader::composeObject(io::IReadFile* file, const core::stringc& name)
{
	if (Mesh->getMeshBufferCount() != Materials.size())
		loadMaterials(file);

	if (MaterialGroups.empty())
	{
		// no material group, so add all
		SMaterialGroup group;
		group.faceCount = CountFaces;
		group.faces = new s16[group.faceCount];
		for (int i=0; i<group.faceCount; ++i)
			group.faces[i] = i;
		MaterialGroups.push_back(group);

		// if we've got no material, add one without a texture
		if (Materials.empty())
		{
			SCurrentMaterial m;
			Materials.push_back(m);
			SMeshBuffer* mb = new scene::SMeshBuffer();
			Mesh->addMeshBuffer(mb);
			mb->Material = Materials[0].Material;
			mb->drop();
			// add an empty mesh buffer name
			core::stringc c = "";
			MeshBufferNames.push_back(c);
		}
	}

	for (u32 i=0; i<MaterialGroups.size(); ++i)
	{
		SMeshBuffer* mb = 0;
		video::SMaterial* mat=0;
		u32 mbPos;
		// -3 because we add three vertices at once
		u32 maxPrimitives = core::min_(Driver->getMaximalPrimitiveCount(), (u32)((1<<16)-1))-3; // currently hardcoded s16 max value for index pointers

		// find mesh buffer for this group
		for (mbPos=0; mbPos<Materials.size(); ++mbPos)
		{
			if (MaterialGroups[i].MaterialName == Materials[mbPos].Name)
			{
				mb = (SMeshBuffer*)Mesh->getMeshBuffer(mbPos);
				mat=&Materials[mbPos].Material;
				MeshBufferNames[mbPos]=name;
				break;
			}
		}

		if (mb != 0)
		{
			// add geometry to the buffer.

			video::S3DVertex vtx;
			core::vector3df vec;
			vtx.Color=mat->DiffuseColor;
			if (mat->MaterialType==video::EMT_TRANSPARENT_VERTEX_ALPHA)
			{
				vtx.Color.setAlpha((int)(255.0f*mat->MaterialTypeParam));
			}
			vtx.Normal.set(0,0,0);

			for (s32 f=0; f<MaterialGroups[i].faceCount; ++f)
			{
				u32 vtxCount = mb->Vertices.size();
				if (vtxCount>maxPrimitives)
				{
					Mesh->addMeshBuffer(new SMeshBuffer());
					IMeshBuffer* tmp = mb;
					mb=(SMeshBuffer*)(Mesh->MeshBuffers[mbPos]=Mesh->MeshBuffers.getLast());
					Mesh->MeshBuffers[Mesh->MeshBuffers.size()-1]=tmp;
					mb->Material=*mat;
					vtxCount=0;
				}

				for (s32 v=0; v<3; ++v)
				{
					s32 idx = Indices[MaterialGroups[i].faces[f]*4 +v];

					if (CountVertices > idx)
					{
						vtx.Pos.X = Vertices[idx*3 + 0];
						vtx.Pos.Z = Vertices[idx*3 + 1];
						vtx.Pos.Y = Vertices[idx*3 + 2];
//						TransformationMatrix.transformVect(vtx.Pos);
					}

					if (CountTCoords > idx)
					{
						vtx.TCoords.X = TCoords[idx*2 + 0];
						vtx.TCoords.Y = 1.0f -TCoords[idx*2 + 1];
					}

					mb->Vertices.push_back(vtx);
				}

				// compute normal
				core::plane3d<f32> pl(mb->Vertices[vtxCount].Pos, mb->Vertices[vtxCount+2].Pos,
						mb->Vertices[vtxCount+1].Pos);

				mb->Vertices[vtxCount].Normal = pl.Normal;
				mb->Vertices[vtxCount+1].Normal = pl.Normal;
				mb->Vertices[vtxCount+2].Normal = pl.Normal;

				// add indices

				mb->Indices.push_back(vtxCount);
				mb->Indices.push_back(vtxCount+2);
				mb->Indices.push_back(vtxCount+1);
			}
		}
		else
			os::Printer::log("Found no matching material for Group in 3ds file.", ELL_WARNING);
	}

	cleanUp();
}


core::stringc C3DSMeshFileLoader::getTextureFileName(const core::stringc& texture, 
						 core::stringc& model)
{
	s32 idx = -1;
	idx = model.findLast('/');

	if (idx == -1)
		idx = model.findLast('\\');

	if (idx == -1)
		return core::stringc();

	core::stringc p = model.subString(0, idx+1);
	p.append(texture);
	return p;
}


void C3DSMeshFileLoader::loadMaterials(io::IReadFile* file)
{
	// create a mesh buffer for every material
	core::stringc modelFilename = file->getFileName();

	if (Materials.empty())
		os::Printer::log("No materials found in 3ds file.", ELL_INFORMATION);

	MeshBufferNames.reallocate(Materials.size());
	for (u32 i=0; i<Materials.size(); ++i)
	{
		MeshBufferNames.push_back("");
		SMeshBuffer* m = new scene::SMeshBuffer();
		Mesh->addMeshBuffer(m);

		m->Material = Materials[i].Material;
		if (Materials[i].Filename[0].size())
		{
			m->Material.Textures[0] = Driver->getTexture(Materials[i].Filename[0].c_str());
			
			if (!m->Material.Textures[0])
			{
				core::stringc fname = getTextureFileName(
					Materials[i].Filename[0], modelFilename);
				if (fname.size())
					m->Material.Textures[0] = Driver->getTexture(fname.c_str());				
			}

			if (!m->Material.Textures[0])
				os::Printer::log("Could not load a texture for entry in 3ds file",
					Materials[i].Filename[0].c_str(), ELL_WARNING);
		}

		if (Materials[i].Filename[2].size())
		{
			m->Material.Textures[0] = Driver->getTexture(Materials[i].Filename[2].c_str());
			
			if (!m->Material.Textures[0])
			{
				core::stringc fname = getTextureFileName(
					Materials[i].Filename[2], modelFilename);
				if (fname.size())
					m->Material.Textures[0] = Driver->getTexture(fname.c_str());				
			}

			if (!m->Material.Textures[0])
			{
				os::Printer::log("Could not load a texture for entry in 3ds file",
					Materials[i].Filename[2].c_str(), ELL_WARNING);
			}
			else
			{
				m->Material.MaterialType=video::EMT_TRANSPARENT_ADD_COLOR;
			}
		}

		if (Materials[i].Filename[3].size())
		{
			m->Material.Textures[1]=m->Material.Textures[0];
			m->Material.Textures[0]=0;
			m->Material.Textures[0] = Driver->getTexture(Materials[i].Filename[3].c_str());
			
			if (!m->Material.Textures[0])
			{
				core::stringc fname = getTextureFileName(
					Materials[i].Filename[3], modelFilename);
				if (fname.size())
					m->Material.Textures[0] = Driver->getTexture(fname.c_str());				
			}

			if (!m->Material.Textures[0])
			{
				os::Printer::log("Could not load a texture for entry in 3ds file",
					Materials[i].Filename[3].c_str(), ELL_WARNING);
				m->Material.Textures[0]=m->Material.Textures[1];
				m->Material.Textures[1]=0;
			}
			else
			{
				m->Material.MaterialType=video::EMT_REFLECTION_2_LAYER;
			}
		}

		if (Materials[i].Filename[4].size())
		{
			m->Material.Textures[1] = Driver->getTexture(Materials[i].Filename[4].c_str());
			
			if (!m->Material.Textures[1])
			{
				core::stringc fname = getTextureFileName(
					Materials[i].Filename[4], modelFilename);
				if (fname.size())
					m->Material.Textures[1] = Driver->getTexture(fname.c_str());				
			}

			if (!m->Material.Textures[1])
				os::Printer::log("Could not load a texture for entry in 3ds file",
					Materials[i].Filename[4].c_str(), ELL_WARNING);
			else
			{
				Driver->makeNormalMapTexture(m->Material.Textures[1], 9.0f);
				m->Material.MaterialType=video::EMT_PARALLAX_MAP_SOLID;
				m->Material.MaterialTypeParam=0.035f;
			}
		}

		m->drop();
	}
}


void C3DSMeshFileLoader::cleanUp()
{
	delete [] Vertices;
	CountVertices = 0;
	Vertices = 0;
	delete [] Indices;
	Indices = 0;
	CountFaces = 0;
	delete [] TCoords;
	TCoords = 0;
	CountTCoords = 0;

	MaterialGroups.clear();
}




void C3DSMeshFileLoader::readTextureCoords(io::IReadFile* file, ChunkData& data)
{
	file->read(&CountTCoords, sizeof(CountTCoords));
#ifdef __BIG_ENDIAN__
	CountTCoords = os::Byteswap::byteswap(CountTCoords);
#endif
	data.read += sizeof(CountTCoords);

	s32 tcoordsBufferByteSize = CountTCoords * sizeof(f32) * 2;

	if (data.header.length - data.read != tcoordsBufferByteSize)
	{
		os::Printer::log("Invalid size of tcoords found in 3ds file.", ELL_WARNING);
		return;
	}

	TCoords = new f32[CountTCoords * 3];
	file->read(TCoords, tcoordsBufferByteSize);
#ifdef __BIG_ENDIAN__
	for (int i=0;i<CountTCoords*2;i++) TCoords[i] = os::Byteswap::byteswap(TCoords[i]);
#endif
	data.read += tcoordsBufferByteSize;
}


void C3DSMeshFileLoader::readMaterialGroup(io::IReadFile* file, ChunkData& data)
{
	SMaterialGroup group;

	readString(file, data, group.MaterialName);

	file->read(&group.faceCount, sizeof(group.faceCount));
#ifdef __BIG_ENDIAN__
	group.faceCount = os::Byteswap::byteswap(group.faceCount);
#endif
	data.read += sizeof(group.faceCount);

	// read faces
	group.faces = new s16[group.faceCount];
	file->read(group.faces, sizeof(s16) * group.faceCount);
#ifdef __BIG_ENDIAN__
	for (int i=0;i<group.faceCount;i++) group.faces[i] = os::Byteswap::byteswap(group.faces[i]);
#endif
	data.read += sizeof(s16) * group.faceCount;

	MaterialGroups.push_back(group);
}


void C3DSMeshFileLoader::readIndices(io::IReadFile* file, ChunkData& data)
{
	file->read(&CountFaces, sizeof(CountFaces));
#ifdef __BIG_ENDIAN__
	CountFaces = os::Byteswap::byteswap(CountFaces);
#endif
	data.read += sizeof(CountFaces);

	s32 indexBufferByteSize = CountFaces * sizeof(u16) * 4;

	// Indices are u16s.
	// After every 3 Indices in the array, there follows an edge flag.
	Indices = new u16[CountFaces * 4];
	file->read(Indices, indexBufferByteSize);
#ifdef __BIG_ENDIAN__
	for (int i=0;i<CountFaces*4;++i) Indices[i] = os::Byteswap::byteswap(Indices[i]);
#endif
	data.read += indexBufferByteSize;
}



void C3DSMeshFileLoader::readVertices(io::IReadFile* file, ChunkData& data)
{
	file->read(&CountVertices, sizeof(CountVertices));
#ifdef __BIG_ENDIAN__
	CountVertices = os::Byteswap::byteswap(CountVertices);
#endif
	data.read += sizeof(CountVertices);

	s32 vertexBufferByteSize = CountVertices * sizeof(f32) * 3;

	if (data.header.length - data.read != vertexBufferByteSize)
	{
		os::Printer::log("Invalid size of vertices found in 3ds file.", ELL_WARNING);
		return;
	}

	Vertices = new f32[CountVertices * 3];
	file->read(Vertices, vertexBufferByteSize);
#ifdef __BIG_ENDIAN__
	for (int i=0;i<CountVertices*3;i++) Vertices[i] = os::Byteswap::byteswap(Vertices[i]);
#endif
	data.read += vertexBufferByteSize;
}



void C3DSMeshFileLoader::readChunkData(io::IReadFile* file, ChunkData& data)
{
	file->read(&data.header, sizeof(ChunkHeader));
#ifdef __BIG_ENDIAN__
	data.header.id = os::Byteswap::byteswap(data.header.id);
	data.header.length = os::Byteswap::byteswap(data.header.length);
#endif
	data.read += sizeof(ChunkHeader);
}


void C3DSMeshFileLoader::readString(io::IReadFile* file, ChunkData& data, core::stringc& out)
{
	c8 c = 1;
	out = "";

	while (c)
	{
		file->read(&c, sizeof(c8));
		if (c)
			out.append(c);

		++data.read;
	}
}


} // end namespace scene
} // end namespace irr

