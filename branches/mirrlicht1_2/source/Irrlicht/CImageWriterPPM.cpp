
#include "CImageWriterPPM.h"
#include "IWriteFile.h"
#include "IImage.h"
#include "dimension2d.h"
#include <string.h>
#include <stdio.h>

#include "IrrCompileConfig.h"
// this should really go into the compile config header.
// there is a similar macro in irrlicht.h, but i feel that
// macro should be moved to compile config also.
#ifdef _IRR_WINDOWS_
#  define snprintf _snprintf
#endif

#if defined(__SYMBIAN32__) && defined(__WINS__)	
	#include <stdarg.h>
	class snprintf_work_around
	{
	public:
		snprintf_work_around() { }

		int operator()( char* buf, size_t max_num_of_byte, const char* format, ... )
		{
			va_list va;
			va_start(va, format);			  
			//  format the message as requested:
			int nRet = sprintf(buf, format, va);			  
			va_end(va);
			return nRet;
		}
	};
	#define snprintf (snprintf_work_around())
#endif

namespace irr
{
namespace video
{

IImageWriter* createImageWriterPPM()
{
	return new CImageWriterPPM;
}

CImageWriterPPM::CImageWriterPPM()
{
#ifdef _DEBUG
	setDebugName("CImageWriterPPM");
#endif
}

bool CImageWriterPPM::isAWriteableFileExtension(const c8* fileName)
{
	return strstr(fileName, ".ppm") != 0;
}

bool CImageWriterPPM::writeImage(io::IWriteFile *file, IImage *image)
{
	char cache[70];
	char size;

	const core::dimension2d<s32>& imageSize = image->getDimension();

#if defined(__SYMBIAN32__) && defined(__GCCE__)	
	size = sprintf(cache, "P3\n");
#else
	size = snprintf(cache, 70, "P3\n");
#endif
	if (file->write(cache, size) != size)
		return false;

#if defined(__SYMBIAN32__) && defined(__GCCE__)	
	size = sprintf(cache, "%d %d\n", imageSize.Width, imageSize.Height);
#else
	size = snprintf(cache, 70, "%d %d\n", imageSize.Width, imageSize.Height);
#endif
	if (file->write(cache, size) != size)
		return false;

#if defined(__SYMBIAN32__) && defined(__GCCE__)	
	size = sprintf(cache, "255\n");
#else
	size = snprintf(cache, 70, "255\n");
#endif

	if (file->write(cache, size) != size)
		return false;

	s32 n = 0;

	s32 r;
	for (r = 0; r < imageSize.Height; ++r)
	{
		s32 c;
		for (c = 0; c < imageSize.Width; ++c, ++n)
		{
			const video::SColor& pixel = image->getPixel(c, r);
#if defined(__SYMBIAN32__) && defined(__GCCE__)	
			size = sprintf(cache, "%.3d %.3d %.3d%s", pixel.getRed(), pixel.getGreen(), pixel.getBlue(), n % 5 == 4 ? "\n" : "  ");	
#else
			size = snprintf(cache, 70, "%.3d %.3d %.3d%s", pixel.getRed(), pixel.getGreen(), pixel.getBlue(), n % 5 == 4 ? "\n" : "  ");	
#endif
			if (file->write(cache, size) != size)
				return false;
		}
	}

	return true;
}

}; // namespace video
}; // namespace irr