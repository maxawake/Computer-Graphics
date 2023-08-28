/**
 * @brief  Some definitions...
 * 
 * @file  Defs.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#ifndef VISUS_OGL4_DEFS_H
#define VISUS_OGL4_DEFS_H
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef M_PI
#define M_PI    3.141592653589793
#define M_PI_2  1.570796326794897
#endif

//! Standard quad vertices in 2D.
const float ogl4_2dQuadVerts[] = {
  0.0f, 0.0f,      // 1. vertex (x,y)
  1.0f, 0.0f,      // 2. vertex (x,y)
  0.0f, 1.0f,      // 3. vertex (x,y)
  1.0f, 1.0f       // 4. vertex (x,y)
};

//! Standard quad vertices in homogeneous coordinates.
const float ogl4_4dQuadVerts[] = {
  0.0f, 0.0f, 0.0f, 1.0f,      // 1. vertex (x,y)
  1.0f, 0.0f, 0.0f, 1.0f,      // 2. vertex (x,y)
  0.0f, 1.0f, 0.0f, 1.0f,      // 3. vertex (x,y)
  1.0f, 1.0f, 0.0f, 1.0f       // 4. vertex (x,y)
};

//! Number of box vertices in 3D.
const int   ogl4_num4dBoxVerts = 8;

//! Standard box vertices in homogeneous coordinates.
const float ogl4_4dBoxVerts[] = {
  0.0f,0.0f,0.0f,1.0f,
  1.0f,0.0f,0.0f,1.0f,
  1.0f,1.0f,0.0f,1.0f,
  0.0f,1.0f,0.0f,1.0f,
  0.0f,0.0f,1.0f,1.0f,
  1.0f,0.0f,1.0f,1.0f,
  1.0f,1.0f,1.0f,1.0f,
  0.0f,1.0f,1.0f,1.0f
};

//! Number of box faces in 3D.
const int ogl4_numBoxFaces = 6;

//! Standard box faces IDs.
const unsigned int ogl4_boxFaces[] = {
  0,3,2, 2,1,0,
  4,5,6, 6,7,4,
  0,1,5, 5,4,0,
  1,2,6, 6,5,1,
  2,3,7, 7,6,2,
  3,0,4, 4,7,3
};

//! Number of box edges in 3D.
const int ogl4_numBoxEdges = 12;

//! Standard box edges IDs.
const unsigned int ogl4_BoxEdges[] = {
  0,1, 1,2, 2,3, 3,0,
  4,5, 5,6, 6,7, 7,4,
  0,4, 1,5, 2,6, 3,7
};


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_OGL4_DEFS_H */

