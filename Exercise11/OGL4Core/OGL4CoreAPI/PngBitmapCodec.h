/*
 * PngBitmapCodec.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 *
 */

#ifndef _PNGBITMAPCODEC_H_
#define _PNGBITMAPCODEC_H_
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#ifdef _WIN32
#define OGL4COREPLUGIN_API __declspec(dllexport)
#define OGL4COREPLUGIN_CALL __cdecl
#else
#define OGL4COREPLUGIN_API
#define OGL4COREPLUGIN_CALL
#endif


#ifndef UINT_MAX
#define UINT_MAX 0xffffffff
#endif

#include <iostream>
#include <fstream>
#include <stdint.h>

#ifndef _WIN32
#include <cstdlib>
#endif

#include "png.h"
#include "BitmapImage.h"

 
    class OGL4COREPLUGIN_API PngBitmapCodec
    {
    public:

        /** Ctor. */
        PngBitmapCodec(void);

        /** Dtor. */
        ~PngBitmapCodec(void);

      /**
         * Loads an image from file.
         *
         * You must set 'Image' to a valid BitmapImage object before calling
         * this method.
         *
         * @param filename Path to the file containing the image data.
         *
         * @return 'true' if the file was successfully loaded.
         */
        bool LoadFromFile(const char* filename);

        /**
         * Saves the image to a file.
         *
         * You must set 'Image' to a valid BitmapImage object before calling
         * this method. Floatmaps are always saved as binary.
         *
         * @param filename Path to the file to be written.
         * @param binary
         *
         * @return 'true' if the file was successfully saved.
         */
        bool SaveToFile(const char* filename, bool binary = true) const;

      /**
         * Accesses the pointer to the image to be used by the codec. Use this
         * to set the pointer to an image object to be used before calling
         * 'Load' or 'Save'. The image objects memory will not be released by
         * the codec object if the image pointer is changed or the codec
         * object is destroied. The caller is responsible to handling the
         * memory.
         *
         * @return Reference to the image object pointer of the codec.
         */
        BitmapImage*& Image(void)
        {
            return this->image;
        }

        /**
         * Gets the pointer to the image object that is used by the codec.
         *
         * @return The image object pointer that is used by the codec.
         */
        const BitmapImage* Image(void) const
        {
            return this->image;
        }

    private:

      /** The bitmap image used by the codec */
        BitmapImage* image;
           
    };


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* _PNGBITMAPCODEC_H_ */
