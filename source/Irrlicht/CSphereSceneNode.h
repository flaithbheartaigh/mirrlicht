// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_SHPERE_SCENE_NODE_H_INCLUDED__
#define __C_SHPERE_SCENE_NODE_H_INCLUDED__

#include "ISceneNode.h"
#include "SMeshBuffer.h"

namespace irr
{
namespace scene
{
	class CSphereSceneNode : public ISceneNode
	{
	public:

		//! constructor
		CSphereSceneNode(f32 size, s32 polyCount, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! destructor
		virtual ~CSphereSceneNode();

		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! returns the material based on the zero based index i. To get the amount
		//! of materials used by this scene node, use getMaterialCount().
		//! This function is needed for inserting the node into the scene hirachy on a
		//! optimal position for minimizing renderstate changes, but can also be used
		//! to directly modify the material of a scene node.
		virtual video::SMaterial& getMaterial(u32 i);

		//! returns amount of materials used by this scene node.
		virtual u32 getMaterialCount();

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_SPHERE; }

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0);

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent=0, ISceneManager* newManager=0);

	private:

		void setSizeAndPolys();

		SMeshBuffer Buffer;
		f32 Radius;
		s32 PolyCount;
	};

} // end namespace scene
} // end namespace irr

#endif

