/**
 * @brief  Timer class
 * 
 * @file   Timer.h
 * 
 * Copyright (c) 2012-2014 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#ifndef VISUS_OBJLOADER_H_INCLUDED
#define VISUS_OBJLOADER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "OGL4CoreAPI.h"

#include <ctime>
#ifdef _WIN32
# define NOMINMAX
# include "windows.h"
#else
# include <sys/time.h>
#endif


#include "OGL4CoreAPI.h"

class OGL4COREAPI_API  WallTimer
{
public:
    WallTimer();

    /**
     *   Reset the WallTimer.
     */
    void reset();
    
    /**
     * Get the elapsed time in mirco-seconds since last call.
     */
    int   get_elapsed() const;
    
    /**
     * Get the elapsed time in seconds since last call.
     */
    float get_elapsed_sec() const;
    
private:
#ifdef _WIN32
    __int64 start;
    double pcfreq;
#else
    struct timeval start;
#endif
};


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_OBJLOADER_H_INCLUDED */
