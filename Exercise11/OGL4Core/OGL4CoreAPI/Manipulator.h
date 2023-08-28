/**
 * @brief  Manipulator class - header.
 * 
 * @file  Manipulator.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#ifndef VISUS_MANIPULATOR_H_INCLUDED
#define VISUS_MANIPULATOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include <string>
#include "glm.hpp"
#include "trackball.h"

/**
 * @brief Manipulator class
 * 
 *  The predefined manipulators are controlled via left, mid, and right 
 *  button pressed and SHIFT-Key being pressed or not. The order of the 
 *  base actions represent the order in which they take effect. E.g., 
 *  the composited matrix for OBJ_PAN_ROT_SCALE_2D with pan matrix P, 
 *  rotation matrix R, and scale matrix S reads:  M = P * R * S.
 *  
 *  The manipulators in detail:
 * 
 *     - TRACKBALL:
 *        - rot:   left button
 *  
 *     - ORBIT_VIEW_3D:
 *        - rot:   left button
 *        - dolly: right button
 * 
 *     - OBJ_MOVE_SCALE_ROT:
 *        - rot:   left button
 *        - scale: mid button
 *        - move:  right button (dolly), right button + SHIFT (pan-xy)
 * 
 *     - OBJ_PAN_ROT_SCALE_2D:  
 *        - pan:   right button + SHIFT,
 *        - rot:   left button
 *        - scale: mid button
 * 
 *  To add a new Manipulator type, you have to do the following:
 *    - add ManipulatorType
 *    - ...
 */
class Manipulator {
public:

    //! Type of predefined manipulators.
    enum ManipulatorType {
        MANIPULATOR_TRACKBALL = 0,
        MANIPULATOR_ORBIT_VIEW_3D,
        MANIPULATOR_OBJ_MOVE_SCALE_ROT,
        MANIPULATOR_OBJ_PAN_ROT_SCALE_2D, 
        MANIPULATOR_SCALE_PAN_2D, 
        MANIPULATOR_PANZOOM
    };
    
    //! Base actions for manipulators.
    enum ManipulatorAction {
        MANIPULATOR_NOTHING = 0,
        MANIPULATOR_ORBIT   = 1,
        MANIPULATOR_ROTATE  = 2,
        MANIPULATOR_DOLLY   = 4,
        MANIPULATOR_PAN     = 8,
        MANIPULATOR_SCALE   = 16,
        MANIPULATOR_LOOKAT  = 32,
        MANIPULATOR_PAN_Z   = 64
    };
        
    //! Parameters for manipulators.
    typedef struct ManipulatorParamsStruct {
        Trackball t;
        float scale;
        float rotateAngle;
        float dolly;
        float panX;
        float panY;
    } ManipulatorParamT;

    /** Standard constructor.
     *     DO NOT USE THIS CONSTRUCTOR.
     */
    Manipulator();
    
    /** Standard cosntructor.
     * @param n          Name of the manipulator in the UI.
     * @param thematrix  Pointer to the 4x4 matrix that should be manipulated.
     * @param type       Type of manipulator.
     * @param autorotate Auto rotation.
     */
    Manipulator(std::string n, glm::mat4 *thematrix, ManipulatorType type = MANIPULATOR_TRACKBALL, bool autorotate = false);
    
    /** Destructor.
     */
    ~Manipulator();

    glm::mat4 *GetMatrix(void) const;
    const std::string &GetName(void) const;
        
    ManipulatorType  GetType(void);
    int              GetAllowedActions();
    
    void Animate(void);
    
    /** Enable manipulator
     *    The manipulator must be enabled in order to process UpdateMouse.
     */
    void Enable();
    
    /** Disable manipulator
     *    If manipulator is not enabled, UpdateMouse will immediately return
     */
    void Disable();
    
    bool IsAutoRotating();
    void Reset();
    void SetMatrix(glm::mat4 *matrix);
    
    void      SetDolly( float dolly );
    float     GetDolly();
    void      SetPan( glm::vec2 pan );
    glm::vec2 GetPan();
    void      SetRotationAngle( float angle );
    float     GetRotationAngle();
    void      SetRotation( glm::vec3 a, float angle );
    void      GetRotation( glm::vec3 &a, float &angle );
    void      SetScale( float scale );
    float     GetScale();
    
    void UpdateMouse( float p1x = 0.0f, float p1y = 0.0f, float p2x = 0.0f, float p2y = 0.0f, ManipulatorAction action = MANIPULATOR_NOTHING);
    void UpdateMatrix();
    
    void WriteParams( FILE* fptr = stderr );
    
    bool keyboard(unsigned char key, int x, int y );

    ManipulatorParamT  params;
    
protected:    
    void setAllowedActions();
    
private:
    glm::mat4 *matrix;
    std::string name;
    ManipulatorType type;
    int   allowedActions;
    bool  autorotate;  
    bool  isEnabled;
    glm::mat4 matTransf;
};

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_MANIPULATOR_H_INCLUDED */
