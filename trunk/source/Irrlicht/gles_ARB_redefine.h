/** \file GLES_ARB_redefine.h define some ARB macros and functions used in Irrlicht so that
*                             it can be compiled with OpenGL ES.
*   
*/
#ifndef GLES_ARB_REDEFINE_H
#define GLES_ARB_REDEFINE_H

/*------------- macros ------------------*/

//GL_ARB_multitexture
#define GL_TEXTURE0_ARB                   0x84C0
#define GL_TEXTURE1_ARB                   0x84C1
#define GL_TEXTURE2_ARB                   0x84C2
#define GL_TEXTURE3_ARB                   0x84C3

//GL_ARB_texture_env_combine
#define GL_COMBINE_ALPHA_ARB              0x8572

//GL_ARB_multisample
#define GL_MULTISAMPLE_ARB                0x809D

/*------------- functions ------------------*/
#define glActiveTextureARB                glActiveTexture
#define glClientActiveTextureARB          glClientActiveTexture

/* Later we might add macro and function redefinitions for OpenGL ES 2.0 */

#endif