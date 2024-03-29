VERSION = 1.3.0
# Irrlicht Engine 1.3
# Makefile for Linux, created by N.Gebhardt.
# 
# To use, just run:
#
# make
#
# This will compile Irrlicht, create a static lib (libIrrlicht.a), and copy it
# into the subdirectory lib/Linux. That's all.
#
# If you want Irrlicht to be compiled as shared lib (libIrrlicht.so.versionnumber), then run:
#
# make sharedlib
# make install
#
# Please note that Irrlicht as shared lib is just experimental and  
# probably not tested.
#

#List of object files, separated based on engine architecture
IRROBJ = C3DSMeshFileLoader.o COgreMeshFileLoader.o COBJMeshFileLoader.o CAnimatedMeshMD2.o CAnimatedMeshMD3.o CMD3MeshFileLoader.o CAnimatedMeshMS3D.o CAnimatedMeshB3d.o CAnimatedMeshSceneNode.o CBillboardSceneNode.o CCameraFPSSceneNode.o CCameraMayaSceneNode.o CCameraSceneNode.o CColladaFileLoader.o CCSMLoader.o CDefaultMeshFormatLoader.o CDMFLoader.o CDummyTransformationSceneNode.o CEmptySceneNode.o CGeometryCreator.o CLightSceneNode.o CLMTSMeshFileLoader.o CMeshManipulator.o CMeshSceneNode.o CMetaTriangleSelector.o CMY3DMeshFileLoader.o COCTLoader.o COctTreeSceneNode.o COctTreeTriangleSelector.o CParticleBoxEmitter.o CParticleFadeOutAffector.o CParticleGravityAffector.o CParticlePointEmitter.o CParticleSystemSceneNode.o CQ3LevelMesh.o CSceneCollisionManager.o CSceneManager.o CSceneNodeAnimatorCollisionResponse.o CSceneNodeAnimatorDelete.o CSceneNodeAnimatorFlyCircle.o CSceneNodeAnimatorFlyStraight.o CSceneNodeAnimatorFollowSpline.o CSceneNodeAnimatorRotation.o CSceneNodeAnimatorTexture.o CShadowVolumeSceneNode.o CSkyBoxSceneNode.o CSkyDomeSceneNode.o CTerrainSceneNode.o CTerrainTriangleSelector.o CCubeSceneNode.o CSphereSceneNode.o CTextSceneNode.o CTriangleBBSelector.o CTriangleSelector.o CWaterSurfaceSceneNode.o CXAnimationPlayer.o CXFileReader.o CXMeshFileLoader.o CMeshCache.o CDefaultSceneNodeAnimatorFactory.o CDefaultSceneNodeFactory.o CQuake3ShaderSceneNode.o
IRRVIDEOOBJ = COpenGLDriver.o COpenGLNormalMapRenderer.o COpenGLParallaxMapRenderer.o COpenGLShaderMaterialRenderer.o COpenGLTexture.o COpenGLSLMaterialRenderer.o CD3D8Driver.o CD3D8NormalMapRenderer.o CD3D8ParallaxMapRenderer.o CD3D8ShaderMaterialRenderer.o CD3D8Texture.o CColorConverter.o CFPSCounter.o CImage.o CImageLoaderBMP.o CImageLoaderJPG.o CImageLoaderPCX.o CImageLoaderPNG.o CImageLoaderPSD.o CImageLoaderTGA.o CImageWriterBMP.o CImageWriterJPG.o CImageWriterPCX.o CImageWriterPNG.o CImageWriterPPM.o CImageWriterPSD.o CImageWriterTGA.o CNullDriver.o CD3D9Driver.o CD3D9HLSLMaterialRenderer.o CD3D9NormalMapRenderer.o CD3D9ParallaxMapRenderer.o CD3D9ShaderMaterialRenderer.o CD3D9Texture.o CVideoModeList.o
IRRSWRENDEROBJ = CSoftwareDriver.o CSoftwareTexture.o CTRFlat.o CTRFlatWire.o CTRGouraud.o CTRGouraudWire.o CTRTextureFlat.o CTRTextureFlatWire.o CTRTextureGouraud.o CTRTextureGouraudAdd.o CTRTextureGouraudNoZ.o CTRTextureGouraudWire.o CZBuffer.o CTRTextureGouraudVertexAlpha2.o CTRTextureGouraudNoZ2.o CTRTextureLightMap2_M2.o CTRTextureLightMap2_M4.o CTRTextureLightMap2_M1.o CSoftwareDriver2.o CSoftwareTexture2.o CTRTextureGouraud2.o CTRGouraud2.o CTRGouraudAlpha2.o CTRGouraudAlphaNoZ2.o CTRTextureDetailMap2.o CTRTextureGouraudAdd2.o CTRTextureGouraudAddNoZ2.o CTRTextureWire2.o CTRTextureLightMap2_Add.o CTRTextureLightMapGouraud2_M4.o IBurningShader.o CTRTextureBlend.o CTRTextureGouraudAlpha.o CTRTextureGouraudAlphaNoZ.o CDepthBuffer.o
IRRIOOBJ = CFileList.o CFileSystem.o CLimitReadFile.o CMemoryReadFile.o CReadFile.o CWriteFile.o CXMLReader.o CXMLWriter.o CZipReader.o CPakReader.o irrXML.o CAttributes.o
IRROTHEROBJ = CIrrDeviceSDL.o CIrrDeviceLinux.o CIrrDeviceStub.o CIrrDeviceWin32.o CLogger.o COSOperator.o Irrlicht.o os.o
IRRGUIOBJ = CGUIButton.o CGUICheckBox.o CGUIComboBox.o CGUIContextMenu.o CGUIEditBox.o CGUIEnvironment.o CGUIFileOpenDialog.o CGUIFont.o CGUIImage.o CGUIInOutFader.o CGUIListBox.o CGUIMenu.o CGUIMeshViewer.o CGUIMessageBox.o CGUIModalScreen.o CGUIScrollBar.o CGUISkin.o CGUIStaticText.o CGUITabControl.o CGUIToolBar.o CGUIWindow.o CGUIColorSelectDialog.o CDefaultGUIElementFactory.o CGUISpriteBank.o
ZLIBOBJ = zlib/adler32.o zlib/compress.o zlib/crc32.o zlib/deflate.o zlib/inffast.o zlib/inflate.o zlib/inftrees.o zlib/trees.o zlib/uncompr.o zlib/zutil.o
JPEGLIBOBJ = jpeglib/jcapimin.o jpeglib/jcapistd.o jpeglib/jccoefct.o jpeglib/jccolor.o jpeglib/jcdctmgr.o jpeglib/jchuff.o jpeglib/jcinit.o jpeglib/jcmainct.o jpeglib/jcmarker.o jpeglib/jcmaster.o jpeglib/jcomapi.o jpeglib/jcparam.o jpeglib/jcphuff.o jpeglib/jcprepct.o jpeglib/jcsample.o jpeglib/jctrans.o jpeglib/jdapimin.o jpeglib/jdapistd.o jpeglib/jdatadst.o jpeglib/jdatasrc.o jpeglib/jdcoefct.o jpeglib/jdcolor.o jpeglib/jddctmgr.o jpeglib/jdhuff.o jpeglib/jdinput.o jpeglib/jdmainct.o jpeglib/jdmarker.o jpeglib/jdmaster.o jpeglib/jdmerge.o jpeglib/jdphuff.o jpeglib/jdpostct.o jpeglib/jdsample.o jpeglib/jdtrans.o jpeglib/jerror.o jpeglib/jfdctflt.o jpeglib/jfdctfst.o jpeglib/jfdctint.o jpeglib/jidctflt.o jpeglib/jidctfst.o jpeglib/jidctint.o jpeglib/jidctred.o jpeglib/jmemmgr.o jpeglib/jmemnobs.o jpeglib/jquant1.o jpeglib/jquant2.o jpeglib/jutils.o jpeglib/rdbmp.o jpeglib/rdcolmap.o jpeglib/rdgif.o jpeglib/rdppm.o jpeglib/rdrle.o jpeglib/rdswitch.o jpeglib/rdtarga.o jpeglib/transupp.o jpeglib/wrbmp.o jpeglib/wrgif.o jpeglib/wrppm.o jpeglib/wrrle.o jpeglib/wrtarga.o
LIBPNGOBJ = libpng/png.o libpng/pngerror.o libpng/pngget.o libpng/pngmem.o libpng/pngpread.o libpng/pngread.o libpng/pngrio.o libpng/pngrtran.o libpng/pngrutil.o libpng/pngset.o libpng/pngtrans.o libpng/pngwio.o libpng/pngwrite.o libpng/pngwtran.o libpng/pngwutil.o
# Next variable is for additional scene nodes etc. of customized Irrlicht versions
EXTRAOBJ =
LINKOBJ = $(IRROBJ) $(IRRVIDEOOBJ) $(IRRSWRENDEROBJ) $(IRRIOOBJ) $(IRROTHEROBJ) $(IRRGUIOBJ) $(ZLIBOBJ) $(JPEGLIBOBJ) $(LIBPNGOBJ) $(EXTRAOBJ)

###############
#Compiler flags
CXXINCS = -I../../include -Izlib -Ijpeglib -Ilibpng
CPPFLAGS = $(CXXINCS) -DIRRLICHT_EXPORTS=1
CXXFLAGS = -Wall
ifndef NDEBUG
CXXFLAGS += -g
else
CXXFLAGS += -fexpensive-optimizations -O3
endif
CFLAGS := -fexpensive-optimizations -O3 -DPNG_NO_MMX_CODE

#Linux specific options
STATIC_LIB = libIrrlicht.a
SHARED_LIB = libIrrlicht.so
IRRLICHT_DLL := ../../bin/Win32-gcc/Irrlicht.dll
LIB_PATH = ../../lib/$(SYSTEM)
INSTALL_DIR = /usr/local/lib
staticlib sharedlib install: SYSTEM = Linux
staticlib sharedlib : LDFLAGS = -L/usr/X11R6/lib --no-export-all-symbols --add-stdcall-alias -lGLU -lGL -lXxf86vm
staticlib sharedlib : CXXINCS += -I/usr/X11R6/include

#Windows specific options
sharedlib_win32 staticlib_win32: SYSTEM = Win32-gcc
sharedlib_win32: LDFLAGS = -lgdi32 -lwinspool -lcomdlg32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lglu32 -lopengl32
sharedlib_win32 staticlib_win32: CPPFLAGS += -D__GNUWIN32__ -D_WIN32 -DWIN32 -D_WINDOWS -D_MBCS -D_USRDLL
staticlib_win32: CPPFLAGS += -D_IRR_STATIC_LIB_

####################
# All target, builds Irrlicht as static lib (libIrrlicht.a) and copies it into /lib/Linux
all linux: staticlib

# Builds Irrlicht as shared lib (libIrrlicht.so.versionNumber) and copies it into /lib/Linux
sharedlib: $(LINKOBJ)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared -fPIC -o $(SHARED_LIB).$(VERSION) $^ $(LDFLAGS)
	cp $(SHARED_LIB).$(VERSION) $(LIB_PATH)

# Builds Irrlicht as static lib (libIrrlicht.a)
$(STATIC_LIB): $(LINKOBJ)
	$(AR) rs $@ $^

# Copies static lib into /lib/Linux
staticlib: $(STATIC_LIB)
	cp $^ $(LIB_PATH)

# Builds Irrlicht as dll (Irrlicht.dll) into ../../bin/Win32-gcc
win32: sharedlib_win32
sharedlib_win32: $(IRRLICHT_DLL)
../../bin/Win32-gcc/Irrlicht.dll: $(LINKOBJ)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared -o $@ $^ $(LDFLAGS) -Wl,--out-implib,../../lib/Win32-gcc/$(STATIC_LIB)
# Copies static lib into /lib/Win32-gcc
staticlib_win32: $(STATIC_LIB)
	cp $^ $(LIB_PATH)

# Installs Irrlicht if it was created as shared lib
install:
	cp $(LIB_PATH)/$(SHARED_LIB).$(VERSION) $(INSTALL_DIR)
	cd $(INSTALL_DIR) && ln -s libIrrlicht.so.$(VERSION) $(SHARED_LIB)
	ldconfig -n $(INSTALL_DIR)

# Create dependency files for automatic recompilation
%.d:%.cpp
	$(CXX) $(CPPFLAGS) -MM -MF $@ $<

-include $(LINKOBJ:.o=.d)

help: 
	@echo "Available targets for Irrlicht"
	@echo " sharedlib: Build shared library Irrlicht.so for Linux"
	@echo " staticlib: Build static library Irrlicht.a for Linux"
	@echo " install: Copy shared library to /usr/lib"
	@echo ""
	@echo " sharedlib_win32: Build shared library Irrlicht.dll for Windows"
	@echo " staticlib_win32: Build static library Irrlicht.a for Windows"
	@echo ""
	@echo " clean: Clean up directory"

# Cleans all temporary files and compilation results. 
clean:
	$(RM) $(LINKOBJ) $(SHARED_LIB).$(VERSION) $(STATIC_LIB) $(LINKOBJ:.o=.d)

.PHONY: all sharedlib staticlib sharedlib_win32 staticlib_win32 help install clean

