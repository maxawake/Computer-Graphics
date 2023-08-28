/**
 * @brief  Loader for wavefront obj files - header.
 * 
 * @file   ObjLoader.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
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

#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <map>

#include "glm.hpp"

//! Face point structure for OBJ files.
typedef struct obj_face_point_struct {
    int  vID;    // vertex ID
    int  texID;  // texture ID
    int  nID;    // normal ID
    obj_face_point_struct() {
        vID = -1;
        texID = -1;
        nID = -1;
    }
} obj_face_point_t;

typedef std::vector<obj_face_point_t>  obj_face_t;

//! Tag structure for OBJ files.
typedef struct obj_tag_struct {
    int              materialID;
    std::vector<int> vFaceNums;
    obj_tag_struct() {
        materialID = -1;
    }
} obj_tag_t;

typedef struct obj_draw_struct {
    std::vector<glm::vec4> vert;
    std::vector<glm::vec3> norm;
    std::vector<glm::vec2> tc;
    int  materialID;
    obj_draw_struct() {
        materialID = -1;
    }
} obj_draw_t;


//! Material structure for OBJ files.
typedef struct obj_material_struct {
    float Ns;      //!< Specular component of the Phong shading model ranges between 0 and 1000
    float Ni;      //
    float d;       //
    float Tr;      // alpha transparency
    float Tf[3];
    int   illum;   // the illumination model to be used by the material
                   //  0: no lighting
                   //  1: diffuse lighting only
                   //  2: both diffuse lighting and specular highlights
    float Ka[3];   //!< Ambient color
    float Kd[3];   //!< Diffuse color
    float Ks[3];   //!< Specular color
    float Ke[3];   //!< Emission color
    
    float mapTexOffset[3];  // "-o u v w" texture offset option
    float mapTexScale[3];   // "-s u v w" texture scaling option
     
    unsigned int  mapID;
    unsigned int  bumpMapID;    

    obj_material_struct() {
        Tr = 0.0f;
        illum = 0;
        Ka[0] = Ka[1] = Ka[2] = 1.0f;
        Kd[0] = Kd[1] = Kd[2] = 1.0f;
        Ks[0] = Ks[1] = Ks[2] = 0.0f;
        Ke[0] = Ke[1] = Ke[2] = 0.0f;
        mapTexOffset[0] = mapTexOffset[1] = mapTexOffset[2] = 0.0f;
        mapTexScale[0] = mapTexScale[1] = mapTexScale[2] = 1.0f;
        mapID = 0;
        bumpMapID = 0;
    }
} obj_material_t;


/**
 * @brief  Obj loader class
 */
class OGL4COREAPI_API ObjLoader
{
public: 
    /** Standard constructor.
     */
    ObjLoader();
    
    /** Standard cosntructor.
     * @param pathname  Path name of obj file.
     * @param filename  File name without path name.
     */
    ObjLoader( const char* pathname, const char* filename );

    virtual ~ObjLoader();
  
 
// --------- public methods -----------
public:
    /** Read object file.
     * @param pathname  Path name of obj file.
     * @param filename  File name without path name.
     */
    bool   ReadObjFile( const char* pathname, const char* filename );
    
    /** Read material parameters file.
     * @param pathname  Path name of obj file.
     * @param filename  File name without path name.
     */
    bool   ReadMtlFile( const char* pathname, const char* filename );
    
    /** Clear all.
     */
    void   ClearAll();

    /** Print face point.
     * @param fp    Reference to face point.
     * @param fptr  File pointer.
     */
    void   printFacePoint ( obj_face_point_t &fp, FILE* fptr = stderr );
    
    /** Print material.
     * @param materialID
     * @param fptr
     */
    void   printMaterial  ( int materialID, FILE* fptr = stderr );

    void   printAllTags   ( FILE* fptr = stderr );

    bool   GenDrawObjects( float*& vert, float*& norm, float*& tc );

    unsigned int* GetDrawOffsets();
    unsigned int  GetNumDrawObjects();
    unsigned int  GetNumDrawVertices();
   
    /** Get center of vertices.
     */
    glm::vec4     CenterOfVertices();
    
    obj_material_t*   GetMaterial( const unsigned int objNum );
    unsigned int      GetNumTextures();
    const char*       GetTextureName( unsigned int num, int &id );
   
// ---------- protected methods -------------
protected:
// we are not publishing any of that, thank you very much
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
    void clearObjPointers();

    bool readVertex   ( std::vector<std::string> &tokenrow, glm::vec4 &v );
    bool readNormal   ( std::vector<std::string> &tokenrow, glm::vec3 &n );
    bool readTexCoord ( std::vector<std::string> &tokenrow, glm::vec2 &tc );
    bool readFace     ( std::vector<std::string> &tokenrow, obj_face_t   &face );
    bool tokenizeFile ( const std::string filename, std::vector<std::vector<std::string>> &tokens, bool useStandardIgnoreTokens = true );
    void lowCase ( std::string &s );
    

// --------- protected attributes -----------
 protected:
    std::string   mPathname;
    std::string   mFilename;
    std::string   mMTLname;
    long          mNumVertices;
    long          mNumNormals;
    long          mNumTexCoords;
   
    std::vector<glm::vec4>   mVertices;
    std::vector<glm::vec3>   mNormals;
    std::vector<glm::vec2>   mTexCoords;
    std::vector<obj_face_t>  mFaces;     
    std::vector<obj_tag_t>   mTags;
   
    std::vector<obj_material_t*>  mMaterial;
    std::map<std::string,int>            mMaterialNames;       
    std::map<std::string,int>::iterator  mMaterialNamesItr;       
    std::map<std::string,int>            mTexNames;
    std::map<std::string,int>::iterator  mTexNamesItr;
   
    glm::vec4  mCenterOfVertices;
    
    unsigned int* mObjOffsets;
    unsigned int  mNumDrawObjects;   
    unsigned int  mNumAllObjVertices;

    std::vector<obj_draw_t> objList;
    
#ifdef _WIN32
#pragma warning(pop)
#endif       
};

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_OBJLOADER_H_INCLUDED */
