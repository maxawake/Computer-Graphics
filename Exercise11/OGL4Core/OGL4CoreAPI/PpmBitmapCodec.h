/*
 * PpmBitmapCodec.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 * 
 * Originates from Sebastian Grottel,
 * edited by Alexander Straub.
 */

#ifndef _PPMBITMAPCODEC_H_
#define _PPMBITMAPCODEC_H_
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

#include "BitmapImage.h"

    /**
     * Bitmap codec for the portable pixmap file format (ppm).
     *
     * This is a very simple file format which is implemented mainly for
     * development purpose. This codec implements all functions, however, it is
     * not recommended to use this file format in production systems.
     *
     * Includes experimental support for float PPMs (magic: PF/Pf). It is unclear
     * how much of the float format can be seen as standardized, however
     * floatmaps are assumed to only come in binary flavors (asBinary is ignored).
     * Exceptions are thrown when trying to load/save floatmaps on systems
     * with exotic byte order (e.g. middle endian).
     *
     * This code might be used as reference when implementing additional
     * bitmap codec classes.
     */
    class OGL4COREPLUGIN_API PpmBitmapCodec
    {
    public:

        /** Ctor. */
        PpmBitmapCodec(void);

        /** Dtor. */
        ~PpmBitmapCodec(void);

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
      
      /**
         * Loads an image from file.
         *
         * You must set 'Image' to a valid BitmapImage object before calling
         * this method.
         *
         * @param mem Memory containing the image data.
         * @param size Size of the memory in bytes
         *
         * @return 'true' if the file was successfully loaded.
         */
      bool loadFromMemory(const void* mem, size_t size);
      
      /**
         * Saves the image to memory.
         *
         * You must set 'Image' to a valid BitmapImage object before calling
         * this method. Floatmaps are always saved as binary.
         *
         * @param outmem Memory in which the image data shall be written.
         * @param binary
         *
         * @return Size of the allocated memory.
         */
      int saveToMemory(char** outmem, bool binary = true) const;
      
      int PPM_Shift_ParseInt(const char* cd, size_t p1, size_t p2) const;
      
      double PPM_Shift_ParseDouble(const char* cd, size_t p1, size_t p2) const;
      
      /** possible values of machine byte order / endianness */
        enum Endianness
        {
            ENDIANNESS_BIG_ENDIAN,
            ENDIANNESS_LITTLE_ENDIAN,
            ENDIANNESS_MIDDLE_ENDIAN,
            ENDIANNESS_UNKNOWN
        };
      
      /**
         * Returns the endianness of the system running this vislib application.
         *
         * @return The endianness of the machine.
         */
        inline static Endianness SystemEndianness(void)
        {
            uint32_t endianTestInt = 0x12345678;
            uint8_t endianTestBytes[4];
            ::memcpy(endianTestBytes, &endianTestInt, 4);
            
            bool machineBigEndian = ((endianTestBytes[0] == 0x12)
                && (endianTestBytes[1] == 0x34)
                && (endianTestBytes[2] == 0x56)
                && (endianTestBytes[3] == 0x78));
            bool machineMiddleEndian = ((endianTestBytes[0] == 0x34)
                && (endianTestBytes[1] == 0x12)
                && (endianTestBytes[2] == 0x78)
                && (endianTestBytes[3] == 0x56));
            bool machineLittleEndian = ((endianTestBytes[0] == 0x78)
                && (endianTestBytes[1] == 0x56)
                && (endianTestBytes[2] == 0x34)
                && (endianTestBytes[3] == 0x12));
                
            if (machineBigEndian)
            {
                return ENDIANNESS_BIG_ENDIAN;
            }
            else if (machineLittleEndian)
            {
                return ENDIANNESS_LITTLE_ENDIAN;
            }
            else if (machineMiddleEndian)
            {
                return ENDIANNESS_MIDDLE_ENDIAN;
            }
            else
            {
                return ENDIANNESS_UNKNOWN;
            }
        }

    };


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* _PPMBITMAPCODEC_H_ */
