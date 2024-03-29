// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_OPENGL_MATERIAL_RENDERER_H_INCLUDED__
#define __C_OPENGL_MATERIAL_RENDERER_H_INCLUDED__

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_OPENGL_

#include "COpenGLDriver.h"
#include "IMaterialRenderer.h"

namespace irr
{
namespace video
{

//! Base class for all internal OpenGL material renderers
class COpenGLMaterialRenderer : public IMaterialRenderer
{
public:

	//! Constructor
	COpenGLMaterialRenderer(video::COpenGLDriver* driver)
		: Driver(driver)
	{
	}

protected:

	video::COpenGLDriver* Driver;
};


//! Solid material renderer
class COpenGLMaterialRenderer_SOLID : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_SOLID(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.Textures[0]);
		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			// thanks to Murphy, the following line removed some
			// bugs with several OpenGL implementations.
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glDisable(GL_ALPHA_TEST);
		}
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
	}
};


//! Generic Texture Blend
class COpenGLMaterialRenderer_ONETEXTURE_BLEND : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_ONETEXTURE_BLEND(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.Textures[0]);

		services->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

//		if (material.MaterialType != lastMaterial.MaterialType ||
//			material.MaterialTypeParam != lastMaterial.MaterialTypeParam ||
//			resetAllRenderstates)
		{
			E_BLEND_FACTOR srcFact,dstFact;
			E_MODULATE_FUNC modulate;
			unpack_texureBlendFunc ( srcFact, dstFact, modulate, material.MaterialTypeParam );

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);

			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, (f32) modulate );

			glBlendFunc( getGLBlend(srcFact), getGLBlend(dstFact) );
			glEnable(GL_BLEND);

			if ( getTexelAlpha ( srcFact ) + getTexelAlpha ( dstFact ) )
			{
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);

				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT);
			}
		}
	}

	virtual void OnUnsetMaterial()
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.f );
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);

		glDisable(GL_BLEND);
	}

	private:

		u32 getGLBlend ( E_BLEND_FACTOR factor ) const
		{
			u32 r = 0;
			switch ( factor )
			{
				case EBF_ZERO:			r = GL_ZERO; break;
				case EBF_ONE:			r = GL_ONE; break;
				case EBF_DST_COLOR:		r = GL_DST_COLOR; break;
				case EBF_ONE_MINUS_DST_COLOR:	r = GL_ONE_MINUS_DST_COLOR; break;
				case EBF_SRC_COLOR:		r = GL_SRC_COLOR; break;
				case EBF_ONE_MINUS_SRC_COLOR:	r = GL_ONE_MINUS_SRC_COLOR; break;
				case EBF_SRC_ALPHA:		r = GL_SRC_ALPHA; break;
				case EBF_ONE_MINUS_SRC_ALPHA:	r = GL_ONE_MINUS_SRC_ALPHA; break;
				case EBF_DST_ALPHA:		r = GL_DST_ALPHA; break;
				case EBF_ONE_MINUS_DST_ALPHA:	r = GL_ONE_MINUS_DST_ALPHA; break;
				case EBF_SRC_ALPHA_SATURATE:	r = GL_SRC_ALPHA_SATURATE; break;
			}
			return r;
		}

		u32 getTexelAlpha ( E_BLEND_FACTOR factor ) const
		{
			u32 r;
			switch ( factor )
			{
				case EBF_SRC_ALPHA:		r = 1; break;
				case EBF_ONE_MINUS_SRC_ALPHA:	r = 1; break;
				case EBF_DST_ALPHA:		r = 1; break;
				case EBF_ONE_MINUS_DST_ALPHA:	r = 1; break;
				case EBF_SRC_ALPHA_SATURATE:	r = 1; break;
				default:			r = 0; break;
			}
			return r;
		}
};


//! Solid 2 layer material renderer
class COpenGLMaterialRenderer_SOLID_2_LAYER : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_SOLID_2_LAYER(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(1, material.Textures[1]);
		Driver->setTexture(0, material.Textures[0]);
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTextureARB(GL_TEXTURE1_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

				Driver->extGlActiveTextureARB(GL_TEXTURE0_ARB);
			}

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glDisable(GL_ALPHA_TEST);
		}
	}
};


//! Transparent add color material renderer
class COpenGLMaterialRenderer_TRANSPARENT_ADD_COLOR : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_TRANSPARENT_ADD_COLOR(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.Textures[0]);
		if ((material.MaterialType != lastMaterial.MaterialType) || resetAllRenderstates)
		{
			glDisable(GL_ALPHA_TEST);

			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_BLEND);
		}

		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
	}

	virtual void OnUnsetMaterial()
	{
		glDisable(GL_BLEND);
	}

	//! Returns if the material is transparent.
	virtual bool isTransparent()
	{
		return true;
	}
};


//! Transparent vertex alpha material renderer
class COpenGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.Textures[0]);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			glDisable(GL_ALPHA_TEST);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT );

			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PRIMARY_COLOR_EXT );
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);

			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
		}
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
		glDepthMask(GL_FALSE);
	}

	virtual void OnUnsetMaterial()
	{
		// default values
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE );
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE );
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, GL_PREVIOUS_EXT );
		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE );
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);

		glDisable(GL_BLEND);
	}

	//! Returns if the material is transparent.
	virtual bool isTransparent()
	{
		return true;
	}
};


//! Transparent alpha channel material renderer
class COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.Textures[0]);

		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates
			|| material.MaterialTypeParam != lastMaterial.MaterialTypeParam )
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);

			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);

			f32 refValue = material.MaterialTypeParam;
			if ( refValue == 0.0f )
				refValue = 0.5f;

			glAlphaFunc(GL_GREATER, refValue);
		}
	}

	virtual void OnUnsetMaterial()
	{
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
	}

	//! Returns if the material is transparent.
	virtual bool isTransparent()
	{
		return true;
	}
};



//! Transparent alpha channel material renderer
class COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.Textures[0]);

		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			glEnable(GL_ALPHA_TEST);

			glAlphaFunc(GL_GREATER, 0.5);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}
	}

	virtual void OnUnsetMaterial()
	{
		glDisable(GL_ALPHA_TEST);
	}

	//! Returns if the material is transparent.
	virtual bool isTransparent()
	{
		return false;  // this material is not really transparent because it does no blending.
	}
};


//! material renderer for all kinds of lightmaps
class COpenGLMaterialRenderer_LIGHTMAP : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_LIGHTMAP(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(1, material.Textures[1]);
		Driver->setTexture(0, material.Textures[0]);

		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			glDisable(GL_ALPHA_TEST);

			// diffuse map

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			switch (material.MaterialType)
			{
				case EMT_LIGHTMAP_LIGHTING:
				case EMT_LIGHTMAP_LIGHTING_M2:
				case EMT_LIGHTMAP_LIGHTING_M4:
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
					break;
				case EMT_LIGHTMAP_ADD:
				case EMT_LIGHTMAP:
				case EMT_LIGHTMAP_M2:
				case EMT_LIGHTMAP_M4:
				default:
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
					break;
			}
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR );

			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				// lightmap

				Driver->extGlActiveTextureARB(GL_TEXTURE1_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

				if (material.MaterialType == EMT_LIGHTMAP_ADD)
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD_SIGNED_ARB);
				else
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);

				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);

				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PREVIOUS_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_EXT, GL_SRC_ALPHA);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, GL_PREVIOUS_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_EXT, GL_SRC_ALPHA);

				switch (material.MaterialType)
				{
					case EMT_LIGHTMAP_M4:
					case EMT_LIGHTMAP_LIGHTING_M4:
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 4.0f);
						break;
					case EMT_LIGHTMAP_M2:
					case EMT_LIGHTMAP_LIGHTING_M2:
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 2.0f);
						break;
					default:
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.0f);
				}
			}
		}
	}

	virtual void OnUnsetMaterial()
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->extGlActiveTextureARB(GL_TEXTURE1_ARB);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.f );
		}
	}
};



//! detail map  material renderer
class COpenGLMaterialRenderer_DETAIL_MAP : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_DETAIL_MAP(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(1, material.Textures[1]);
		Driver->setTexture(0, material.Textures[0]);

		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			glDisable(GL_ALPHA_TEST);

			// diffuse map

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);

			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				// detailmap

				Driver->extGlActiveTextureARB(GL_TEXTURE1_ARB);

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD_SIGNED_EXT);

				glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_RGB_EXT,GL_PREVIOUS_EXT);
				glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND0_RGB_EXT,GL_SRC_COLOR);

				glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_RGB_EXT, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND1_RGB_EXT,GL_SRC_COLOR);
			}
		}
	}
};


//! sphere map material renderer
class COpenGLMaterialRenderer_SPHERE_MAP : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_SPHERE_MAP(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.Textures[0]);

		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTextureARB(GL_TEXTURE1_ARB);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

				Driver->extGlActiveTextureARB(GL_TEXTURE0_ARB);
			}

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glDisable(GL_ALPHA_TEST);
#ifndef _IRR_USE_OPENGL_ES_
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
#endif
		}
	}

	virtual void OnUnsetMaterial()
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
			Driver->extGlActiveTextureARB(GL_TEXTURE0_ARB);
#ifndef _IRR_USE_OPENGL_ES_
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
#endif
	}
};


//! reflection 2 layer material renderer
class COpenGLMaterialRenderer_REFLECTION_2_LAYER : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_REFLECTION_2_LAYER(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(1, material.Textures[1]);
		Driver->setTexture(0, material.Textures[0]);

		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			glDisable(GL_ALPHA_TEST);

			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE );
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT );

				Driver->extGlActiveTextureARB(GL_TEXTURE1_ARB);

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);
#ifndef _IRR_USE_OPENGL_ES_
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
#endif
			}
			else
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifndef _IRR_USE_OPENGL_ES_
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
#endif
			}
		}
	}

	virtual void OnUnsetMaterial()
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->extGlActiveTextureARB(GL_TEXTURE0_ARB);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // default value
			Driver->extGlActiveTextureARB(GL_TEXTURE1_ARB);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifndef _IRR_USE_OPENGL_ES_
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
#endif
		}

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#ifndef _IRR_USE_OPENGL_ES_
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
#endif
	}
};


//! reflection 2 layer material renderer
class COpenGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER : public COpenGLMaterialRenderer
{
public:

	COpenGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER(video::COpenGLDriver* d)
		: COpenGLMaterialRenderer(d) {}

	virtual void OnSetMaterial(SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(1, material.Textures[1]);
		Driver->setTexture(0, material.Textures[0]);

		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTextureARB(GL_TEXTURE0_ARB);

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE );
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT );

				Driver->extGlActiveTextureARB(GL_TEXTURE1_ARB);

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);
			}

			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
#ifndef _IRR_USE_OPENGL_ES_
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
#endif
		}
		glDepthMask(GL_FALSE);
	}

	virtual void OnUnsetMaterial()
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
			Driver->extGlActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_BLEND);
#ifndef _IRR_USE_OPENGL_ES_
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
#endif
	}

	//! Returns if the material is transparent.
	virtual bool isTransparent()
	{
		return true;
	}
};

} // end namespace video
} // end namespace irr

#endif
#endif

