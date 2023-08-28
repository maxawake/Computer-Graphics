/*
 * trackball.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#ifndef VISUS_TRACKBALL_H_INCLUDED
#define VISUS_TRACKBALL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "glm.hpp"
#include "gtc/quaternion.hpp"

class Trackball {
public:
    Trackball() {

    }

    /**
     * This function computes a quaternion based on an axis (defined by
     * the given vector) and an angle about which to rotate.  The angle is
     * expressed in radians.  The result is put into the third argument.
     */
    Trackball(glm::vec3 a, float phi);
    ~Trackball() {}

    void Animate(void);

    /**
     * A useful function, builds a rotation matrix in Matrix based on
     * given quaternion.
     */
    glm::mat4 GetRotation();

    /** Get rotation matrix out of quaternion which is defined axis and rotation angle.
     * @param a  Rotation axis.
     * @param phi  Rotation angle in radians.
     */
    glm::mat4 GetRotation( glm::vec3 a, float angle );
    
        
    void Reset();
    
    /** Set quaternion by axis and rotation angle.
     * @param a  Rotation axis.
     * @param phi  Rotation angle in radians.
     */
    void SetAxisRot( glm::vec3 a, float phi );
    
    void GetAxisRot( glm::vec3 &a, float &phi );
    
    /**
     * Pass the x and y coordinates of the last and current positions of
     * the mouse, scaled so they are from (-1.0 ... 1.0).
     *
     * The resulting rotation is returned as a quaternion rotation in the
     * first paramater.
     * 
     * @param p1x
     * @param p1y
     * @param p2x
     * @param p2y
     */
    void UpdateMouse(float p1x, float p1y, float p2x, float p2y);

    glm::fquat q;
    
private:
    float toSphere(float, float, float);

    glm::vec3  u;
    float      phi;    
    glm::fquat rq;
};

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_TRACKBALL_H_INCLUDED */
