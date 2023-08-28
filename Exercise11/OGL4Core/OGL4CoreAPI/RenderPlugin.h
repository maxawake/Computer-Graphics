/**
 * @brief  Base class for all render plugins - header.
 * 
 * @file   RenderPlugin.h
 * 
 * Copyright (c) 2012-2014 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#ifndef VISUS_RENDERPLUGIN_H_INCLUDED
#define VISUS_RENDERPLUGIN_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef _WIN32
#include <iostream>
#include "unistd.h"
#include <cstring>
#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>      // for chdir
#include <sys/param.h>   // for MAXPATHLEN
#else
#include <string>
#endif
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>

class COGL4CoreAPI;
typedef void (* SetVarCallbackT)(const void *value, void *clientData);
typedef void (* GetVarCallbackT)(void *value, void *clientData);
typedef void (* SetButtonCallbackT)(void *clientData);

// todo umdefinieren nach int und im .cpp aus der tweakbar holen


typedef enum EOGLAPIType {
    OGLAPI_TYPE_UNDEF   = 0,
#ifdef __cplusplus
    OGLAPI_TYPE_BOOLCPP = 1,
#endif // __cplusplus
    OGLAPI_TYPE_BOOL8   = 2,
    OGLAPI_TYPE_BOOL16,
    OGLAPI_TYPE_BOOL32,
    OGLAPI_TYPE_CHAR,
    OGLAPI_TYPE_INT8,
    OGLAPI_TYPE_UINT8,
    OGLAPI_TYPE_INT16,
    OGLAPI_TYPE_UINT16,
    OGLAPI_TYPE_INT32,
    OGLAPI_TYPE_UINT32,
    OGLAPI_TYPE_FLOAT,
    OGLAPI_TYPE_DOUBLE,
    OGLAPI_TYPE_COLOR32,    // 32 bits color. Order is RGBA if API is OpenGL or Direct3D10, and inversed if API is Direct3D9 (can be modified by defining 'colorOrder=...', see doc)
    OGLAPI_TYPE_COLOR3F,    // 3 floats color. Order is RGB.
    OGLAPI_TYPE_COLOR4F,    // 4 floats color. Order is RGBA.
    OGLAPI_TYPE_CDSTRING,   // Null-terminated C Dynamic String (pointer to an array of char dynamically allocated with malloc/realloc/strdup)
#ifdef __cplusplus
# if defined(_MSC_VER) && (_MSC_VER > 1500)
    OGLAPI_TYPE_STDSTRING = (0x2ffe0000+sizeof(std::string)),  // VS2010 or higher C++ STL string (std::string)
# else
    OGLAPI_TYPE_STDSTRING = (0x2fff0000+sizeof(std::string)),  // C++ STL string (std::string)
# endif
#endif // __cplusplus
    OGLAPI_TYPE_QUAT4F = OGLAPI_TYPE_CDSTRING+2, // 4 floats encoding a quaternion {qx,qy,qz,qs}
    OGLAPI_TYPE_QUAT4D,     // 4 doubles encoding a quaternion {qx,qy,qz,qs}
    OGLAPI_TYPE_DIR3F,      // direction vector represented by 3 floats
    OGLAPI_TYPE_DIR3D,       // direction vector represented by 3 doubles
    OGLAPI_TYPE_ENUM,
    OGLAPI_TYPE_BUTTON
} OGLApiType;


enum OGLApiAxis {
    AXIS_X = 0,
    AXIS_MX,
    AXIS_Y,
    AXIS_MY,
    AXIS_Z,
    AXIS_MZ
};

//#define OGLAPI_TYPE_CSSTRING(n) ((TwType)(0x30000000+((n)&0xfffffff))) // Null-terminated C Static String of size n (defined as char[n], with n<2^28)

#include "../OGL4CoreAPI/OGL4CoreAPI.h"
#include <memory>
#include "../OGL4CoreAPI/ReferenceCounted.h"
#include "../OGL4CoreAPI/Manipulator.h"
#include "glm.hpp"

#ifdef _WIN32
#define OGL4COREPLUGIN_API __declspec(dllexport)
#define OGL4COREPLUGIN_CALL __cdecl
#else
#define OGL4COREPLUGIN_API
#define OGL4COREPLUGIN_CALL
#endif

class RenderPlugin;
typedef RenderPlugin* (OGL4COREPLUGIN_CALL *CreateInstanceProc) (COGL4CoreAPI *Api);

void OGL4COREPLUGIN_API CopyStringToLibrary(char **dest, const char *src);

/**
 * @brief RenderPlugin is the base class for all render plugins.
 */
class OGL4COREAPI_API RenderPlugin  : public ReferenceCounted {
public:
    RenderPlugin(COGL4CoreAPI *Api) : myName(nullptr), myDescription(nullptr),
            myPath(nullptr), api(Api) {
    }

    virtual ~RenderPlugin(void);

    /** Set visibility of the GUI.
     * @param visible
     */
    void SetGuiVisible(bool visible);

    /** Get visibility of the GUI.
     */
    bool GetGuiVisible();
    
    /** Set visibility of a plugin parameter
     * @param type
     * @param paramName
     * @param isVisible
     */
    void SetPluginParamVisibility( OGLApiType type, const char *paramName, bool isVisible );

    /** Set help message for the UI parameter.
     * @param type
     * @param paramName
     * @param hm      Help message text.
     */
    void SetPluginParamHelpMessage( OGLApiType type, const char *paramName, const char *hm );
    
    /** Set axes for the UI parameter.
     * @param type
     * @param paramName
     * @param axisX
     * @param axisY
     * @param axisZ
     */
    void SetPluginParamAxes( OGLApiType type, const char *paramName, OGLApiAxis axisX, OGLApiAxis axisY, OGLApiAxis axisZ );
        
    /** Get current plugin parameter enumeration id.
     * @param paramName
     * @param enumName
     */
    int GetCurrentPluginParamEnumID( const char* paramName, const char* enumName );
    
    

    /** Add manipulator to UI
     * @param name
     * @param matrix  pointer to glm::m4d matrix that shall be manipulated
     * @param type    
     * @param autorotate
     */
    int AddManipulator(const char *name, glm::mat4 *matrix,
        Manipulator::ManipulatorType type = Manipulator::MANIPULATOR_TRACKBALL,
        bool autorotate = false);
    
    /** Enable manipulator
     *    The manipulator must be enabled in order to accept mouse events.
     *  @param h  Manipulator handle.
     */
    bool EnableManipulator( const int h );
    
    /** Disable manipulator
     *    If manipulator is disabled, it will not accept mouse events.
     *  @param h  Manipulator handle.
     */
    bool DisableManipulator( const int h );
    
    /** Set dolly of manipulator.
     * @param h   Manipulator handle.
     * @param dolly  Dolly value.
     * @return false  Manipulator does not exist.
     */
    bool SetManipulatorDolly( const int h, float dolly );
    
    /** Get dolly of manipulator.
     * @param h   Manipulator handle.
     * @param dolly  Reference to dolly value.
     * @return false  Manipulator does not exist.
     */
    bool GetManipulatorDolly( const int h, float &dolly );
    
    /** Set pan of manipulator.
     * @param h   Manipulator handle.
     * @param pan  Pan value.
     * @return false  Manipulator does not exist.
     */
    bool SetManipulatorPan( const int h, glm::vec2 pan );
    
    /** Get pan of manipulator.
     * @param h   Manipulator handle.
     * @param pan  Reference to pan value.
     * @return false  Manipulator does not exist.
     */
    bool GetManipulatorPan( const int h, glm::vec2 &pan );
    
    /** Set rotation angle of manipulator (2D only).
     * @param h   Manipulator handle.
     * @param angle  Rotation angle in degree.
     * @return false  Manipulator does not exist.
     */
    bool SetManipulatorRotAngle( const int h, float angle );
    
    /** Get rotation angle of manipulator (2D only).
     * @param h   Manipulator handle.
     * @param angle  Reference to rotation angle in degree.
     * @return false  Manipulator does not exist.
     */
    bool GetManipulatorRotAngle( const int h, float &angle );
    
    /** Set rotation of manipulator (3D only).
     * @param h   Manipulator hendle.
     * @param a   Rotation axis.
     * @param angle Rotation angle in degree.
     * @return false  Manipulator does not exist.
     */
    bool SetManipulatorRotation( const int h, glm::vec3 a, float angle );
    
    /** Get rotation of manipulator (3D only).
     * @param h   Manipulator hendle.
     * @param a   Rotation axis.
     * @param angle Rotation angle in degree.
     * @return false  Manipulator does not exist.
     */
    bool GetManipulatorRotation( const int h, glm::vec3 &a, float &angle );
        
    /** Set scale of manipulator.
     * @param h   Manipulator handle.
     * @param scale  Scale factor.
     * @return false  Manipulator does not exist.
     */
    bool SetManipulatorScale( const int h, float scale );
    
    /** Get scale of manipulator.
     * @param h   Manipulator handle.
     * @param scale  Reference to scale factor.
     * @return false  Manipulator does not exist.
     */
    bool GetManipulatorScale( const int h, float &scale );
    
    //bool SetTransferEditor(const int mode);
    
    /** Set window size
     *    You should not call this function from your Activate() method,
     *    because selecting the plugin via command line causes an error.
     * 
     * @param width   new window width
     * @param height  new window height
     */
    void SetWindowSize(int width, int height);
    
    /** Get window size
     * @param width   current window width
     * @param height  current window height
     */
    void GetWindowSize(int &width, int &height);
    
    /** Post a redisplay
     */
    void PostRedisplay();
    
    /** Get the last mouse position in windows coordinates measured from...
     *    This mouse position will be updated at every mouse move or
     *    mouse button event in contrast to the LastMouseDownPos.
     * @param x
     * @param y
     */
    void GetLastMousePos(int &x, int &y);
    
    /** Get the last mouse position in windows coordinates measured from...
     *    See GetLastMousePos(x,y)
     */
    glm::ivec2 GetLastMousePos();
    
    /** Get the position where a mouse button was pressed
     * @param x
     * @param y
     */
    void GetLastMouseDownPos(int &x, int &y);
    
    /** Get the position where a mouse button was pressed
     */
    glm::ivec2 GetLastMouseDownPos();
    
    /** Get the state of the mouse
     * \return 0: mouse button was pressed within OpenGL window, 
     *         1: mouse button was pressed within UI
     *         2: mouse button was released
     */
    int  GetMouseState();
    
    bool IsMouseDownWindow();
    bool IsMouseDownUI();
    bool IsMouseReleased();
    
    /** Get the id of the last pressed mouse button
     * \return  0: left button,  1: middle button,  2: right button
     */
    int  GetMouseButtonPressed();
    
    /** Is left mouse button pressed
     *  @return  true only when pressed, not when released
     */
    bool IsLeftButtonPressed();
    
    /** Is mid mouse button pressed
     *  @return  true only when pressed, not when released
     */
    bool IsMidButtonPressed();
    
    /** Is right mouse button pressed
     *  @return  true only when pressed, not when released
     */
    bool IsRightButtonPressed();
    
    bool IsCtrlPressed();
    bool IsShiftPressed();
    
    /** Set the plugin path
     * @param path
     */
    void SetPluginPath(const char* path);
    
    /** Select the current manipulator
     * @param manipulatorIdx
     */
    bool SelectCurrentManipulator(int manipulatorIdx);
        
    /** Get the current plugin path
     *    This method should be used e.g. if the plugin loads stuff from 
     *    its resources subfolder.
     *
     *    You need to delete the returned value!  ??
     */
    const char *GetCurrentPluginPath() const;

    // you need to delete the returned value!
    const char *GetBasePath() const;

    /** Get current plugin parameter name
     */
    const char *GetCurrentPluginParamName() const;

    /** Get the plugin name
     */
    inline const char *GetName() const {
        return this->myName;
    }
    
    /** Get the plugin description.
     *   This description text is shown in the global help.
     */
    virtual const char *GetDescription(void) const {
        return this->myDescription;
    }

        
    /** Activate the selected plugin.
     *   Here, you can register your UI parameters, allocate memory for
     *   your VBO's, textures etc.
     */
    virtual bool Activate(void) = 0;
    
    /** Deactivate the current plugin.
     *    Take care that the plugin shoud deallocate all that it has allocated
     *    when it was activated.
     */
    virtual bool Deactivate(void) = 0;


    /** Idle callback function
     */
    virtual bool Idle(void) {
        // does nothing, really
        return false;
    }
    
    /** Initialization of...
     *   Here, you should call gl3wInit() if necessary
     */
    virtual bool Init(void) = 0;
    
    /** Keyboard callback function
     * @param key
     * @param x    current x position of the mouse
     * @param y    current y position of the mouse
     * \return     true if the keys were handled by the plugin
     */
    virtual bool Keyboard(unsigned char key, int x, int y) {
        // does nothing, really
        return false;
    }
    
    /** Mouse motion callback function
     * @param x    current x position of the mouse
     * @param y    current y position of the mouse
     * \return    true if the mouse motion was handled by the plugin
     */
    virtual bool Motion(int x, int y) {
        // does nothing, really
        return false;
    }
    
    /** Mouse button callback function
     * @param button
     * @param state
     * @param x    current x position of the mouse
     * @param y    current y position of the mouse
     * \return   true if the mouse button was handled by the plugin
     */
    virtual bool Mouse(int button, int state, int x, int y) {
        // does nothing, really
        return false;
    }
    
    /** Passive mouse motion callback function
     * @param x
     * @param y
     */
    virtual bool PassiveMotion(int x, int y) {
        // does nothing, really
        return false;
    }
    
    /** This is the render method
     */
    virtual bool Render(void) = 0;
    
    /** Resize callback function
     * @param width
     * @param height
     * \return   true if the resize event was handled by the plugin
     */
    virtual bool Resize(int width, int height) {
        // does nothing, really
        return false;
    }
    
    
    /** Print list of all available extensions to stderr
     */
    void PrintAvailableExtensios();
    
    /** Test if extension is available
     * @param extName
     * \return  true if extension is available
     */
    bool IsExtensionAvailable( const char* extName );
    
    /** Set color of user interface.
     * @param red    Red color value [0,255].
     * @param green  Green color value [0,255].
     * @param blue   Blue color value [0,255].
     * @param alpha  Transparency value [0,255], 0:full transparent, 255:opaque.
     */
    void SetGuiColor( int red, int green, int blue, int alpha = 128 );

    void ResetGuiColor();

    void SetGuiTextLightColor( bool lightText = true );

    /**
     *  Enable or disable OpenGL state variable.
     *  Helper function for GLStateVar.
     *
     * @param var    OpenGL state variable (GLenum) like e.g. GL_DEPTH_TEST...
     * @param state  New state: true=enable, false=disable
     */
    void ToggleGLState( unsigned int var, bool state );

    /**
     *  Requests the current OpenGL state of the variable with glGetBooleanv.
     *  Helper function for GLStateVar.
     *
     * @param varOpenGL state variable (GLenum) like e.g. GL_DEPTH_TEST...
     * @param state  New state: true=enable, false=disable
     */
    void SynchronizeGLState( unsigned int var, bool &state );

    /** 
     * Helper method to check for example the file ending of an image.
     * 
     * @param name    String that has to be ckecked.
     * @param ending  String ending.
     * @return true if the string 'name' ends with 'ending'.
     */
    bool StringEndsWith( const char *name, const char *ending );

    void EnablePicking();
    void DisablePicking();
    bool StartPickable( unsigned int id );
    void EndPickable();
    int  DoPick( int x, int y );
    
public:
    /** Add button to UI
     *    DO NOT USE THIS FUNCTION DIRECTLY -> See APIButton parameters.
     * @param name
     * @param def
     * @param setCallback
     * @param clientData
     */
    void AddButton(const char *name, const char *def,
        SetButtonCallbackT setCallback, void *clientData);

    RenderPlugin*  AddOverlay( const char *name, int x, int y, int width, int height, const char *pluginname);

    /** Add plugin parameter    
     *    DO NOT USE THIS FUNCTION DIRECTLY -> See APIVar parameters.
     * @param type
     * @param name
     * @param def
     * @param setCallback
     * @param getCallback
     * @param clientData
     */
    void AddPluginParameter( OGLApiType type, const char *name, const char *def,
        SetVarCallbackT setCallback, GetVarCallbackT getCallback, void *clientData );
            
    /** Set minimum and maximum values for the UI parameter
     *    DO NOT USE THIS FUNCTION DIRECTLY -> See APIVar parameters.
     * @param type  
     * @param paramName
     * @param min
     * @param max
     */ 
    void SetPluginParamMinMax( OGLApiType type, const char *paramName, double min, double max );
    
    /** Set step value for the UI parameter
     *    DO NOT USE THIS FUNCTION DIRECTLY -> See APIVar parameters.
     * @param type
     * @param paramName
     * @param step
     */
    void SetPluginParamStep( OGLApiType type, const char *paramName, double step );
    
    /** Set precision of the UI parameter
     *    DO NOT USE THIS FUNCTION DIRECTLY -> See APIVar parameters.
     * @param type
     * @param paramName
     * @param numSignificantDigits
     */
    void SetPluginParamPrec( OGLApiType type, const char *paramName, int numSignificantDigits );
    
    /** Set whether the UI parameter shall be read only or not
     *    DO NOT USE THIS FUNCTION DIRECTLY -> See APIVar parameters.
     * @param type
     * @param paramName
     * @param readOnly
     */
    void SetPluginParamReadOnly( OGLApiType type, const char *paramName, bool readOnly );
    
    /** Set shortcut to toogle UI parameter or press UI button
     *    DO NOT USE THIS FUNCTION DIRECTLY -> See APIVar parameters.
     * @param type
     * @param paramName
     * @param sc
     */
    void SetPluginParamKeyShortcut(OGLApiType type, const char *paramName, const char *sc );
    
    /** Set shortcut to increase UI parameter
     *    DO NOT USE THIS FUNCTION DIRECTLY -> See APIVar parameters.
     * @param type
     * @param paramName
     * @param sc
     */
    void SetPluginParamKeyShortcutIncr(OGLApiType type, const char *paramName, const char *sc );

    /** Set shortcut to decrease UI parameter
     *    DO NOT USE THIS FUNCTION DIRECTLY -> See APIVar parameters.
     * @param type
     * @param paramName
     * @param sc
     */
    void SetPluginParamKeyShortcutDecr(OGLApiType type, const char *paramName, const char *sc );
        
    void SetSelectable( bool selectable );
    bool IsSelectable();    
    
    void SetFactory( CreateInstanceProc p );
    CreateInstanceProc GetFactory();
    
protected:
    const char *myName;
    const char *myDescription;
    char *myPath;
    bool selectable;
    CreateInstanceProc p;

private:
    COGL4CoreAPI *api;
};


/**
 *  @brief  Base class for all UI variables.
 * 
 *  DO NOT USE THIS CLASS DIRECTLY !
 */
class OGL4COREAPI_API APIVarBase {
public:

    static void SetCallBack(const void *val, void *clientData);
    static void GetCallBack(void *val, void *clientData);
    static void SetButtonCB(void *clientData);

    virtual void Register(const char *def = nullptr) = 0;

    virtual ~APIVarBase(void) {
        delete[] this->name;
    }

protected:

    APIVarBase(const char *name) : name(NULL) {
        this->setName(name);
    }

    void setName(const char *name) {
        delete[] this->name;
        this->name = nullptr;
        if (name != nullptr) {
            size_t strl = strlen(name) + 1;
            this->name = new char[strl];
#ifndef _WIN32
            strcpy(this->name, name);
#else
            strcpy_s(this->name, strl, name);
#endif
        }
    }

    virtual void set(const void *val) = 0;
    virtual void get(void *val) = 0;

    char *name;
};

/**
 *  @brief Template structure for variable policies.
 */
template<class T, OGLApiType U> struct PodVarPolicy {
    typedef T Type;
    static const OGLApiType TypeName = U;
    static inline void set_copy(void *dst, const void *src) {
        memcpy(dst, src, sizeof(Type));
    }
    static inline void get_copy(void *dst, const void *src) {
        memcpy(dst, src, sizeof(Type));
    }
    static inline void delete_var(Type& var) {
        // intentionally empty
    }
};

typedef PodVarPolicy<bool, OGLAPI_TYPE_BOOLCPP>          BoolVarPolicy;      //!< Variable policy for boolean
typedef PodVarPolicy<char, OGLAPI_TYPE_CHAR>             CharVarPolicy;      //!< Variable policy for characters
typedef PodVarPolicy<signed char, OGLAPI_TYPE_INT8>      Int8VarPolicy;      //!< Variable policy for 8bit integers
typedef PodVarPolicy<unsigned char, OGLAPI_TYPE_UINT8>   UInt8VarPolicy;     //!< Variable policy for 8bit unsigned integers
typedef PodVarPolicy<short,  OGLAPI_TYPE_INT16>          Int16VarPolicy;     //!< Variable policy for 16bit integers 
typedef PodVarPolicy<unsigned short, OGLAPI_TYPE_UINT16> UInt16VarPolicy;    //!< Variable policy for 16bit unsigned integers
typedef PodVarPolicy<int,  OGLAPI_TYPE_INT32>            IntVarPolicy;       //!< Variable policy for 32bit integers
typedef PodVarPolicy<unsigned int, OGLAPI_TYPE_UINT32>   UIntVarPolicy;      //!< Variable policy for 32bit unsigned integers
typedef PodVarPolicy<float, OGLAPI_TYPE_FLOAT>           FloatVarPolicy;     //!< Variable policy for 32bit floating point values
typedef PodVarPolicy<double, OGLAPI_TYPE_DOUBLE>         DoubleVarPolicy;    //!< Variable policy for 64bit floating point values
typedef PodVarPolicy<unsigned int, OGLAPI_TYPE_COLOR32>  Color32VarPolicy;   //!< Variable policy for 32bit colors
typedef PodVarPolicy<glm::vec3, OGLAPI_TYPE_COLOR3F>     Color3FVarPolicy;   //!< Variable policy for float colors (r,g,b)
typedef PodVarPolicy<glm::vec4, OGLAPI_TYPE_COLOR4F>     Color4FVarPolicy;   //!< Variable policy for float colors (r,g,b,a)
typedef PodVarPolicy<glm::vec3, OGLAPI_TYPE_DIR3F>       Dir3FVarPolicy;     //!< Variable policy for 32bit float directions (x,y,z)
typedef PodVarPolicy<glm::dvec3, OGLAPI_TYPE_DIR3D>      Dir3DVarPolicy;     //!< Variable policy for 64bit float directions (x,y,z)
typedef PodVarPolicy<glm::vec4, OGLAPI_TYPE_QUAT4F>      Quat4FVarPolicy;    //!< Variable policy for 32bit float quaternions
typedef PodVarPolicy<glm::dvec4, OGLAPI_TYPE_QUAT4D>     Quat4DVarPolicy;    //!< Variable policy for 64bit float quaternions
typedef PodVarPolicy<int, OGLAPI_TYPE_ENUM>              EnumVarPolicy;      //!< Variable policy for enums:  USE EnumVar<T> !!

/**
 * @brief Template structure for button policy
 */
template<class T, OGLApiType U> struct ButtVarPolicy {
    typedef T Type;
    static const OGLApiType TypeName = U;
    static inline void set_copy(void *dst, const void *src) {
    }
    static inline void get_copy(void *dst, const void *src) {
    }
    static inline void delete_var(T& var) {
        // intentionally empty
    }
};
typedef ButtVarPolicy<void*, OGLAPI_TYPE_BUTTON>          ButtonPolicy;


/**
 * @brief Template structure for string policy
 */
template<class T, OGLApiType U> struct StrVarPolicy {
    typedef T Type;
    static const OGLApiType TypeName = U;

    static inline void set_copy(void *dst, const void *src) {
        const char *m_src = *reinterpret_cast<const char *const*>(static_cast<const char*>(src));
        //char **m_dst = (char **)dst;
        char **m_dst = reinterpret_cast<char **>(static_cast<char*>(dst));
        delete[] *m_dst;

        size_t len = strlen(m_src) + 1;
        *m_dst = new char[len];
#ifdef _WIN32
        strncpy_s(*m_dst, len, m_src, len);
#else
        strncpy(*m_dst, m_src, len);
#endif
    }
    static inline void get_copy(void *dst, const void *src) {
        const char *m_src = *reinterpret_cast<const char *const*>(static_cast<const char*>(src));
        CopyStringToLibrary(static_cast<char **>(dst), m_src);
    }
    static inline void delete_var(T& var) {
        delete[] var;
        var = nullptr;
    }
};

typedef StrVarPolicy<char*, OGLAPI_TYPE_CDSTRING>   CStringVarPolicy;


/**
 * @brief Template class vor UI parameters
 *
 * A variable can be added to the graphical user interface using the APIVar class.
 * The different types that can be used as variables is listed above; see BoolVarPolicy etc.
 *
 * In case of a float variable, adding a variable would look like the following example.
 * The callback function can also be omitted. "R" is the name of the plugin.
 *
 * header:
 *    APIVar<R, FloatVarPolicy>  myValue;
 *    void doSomething( APIVar<R, FloatVarPolicy> &var );
 *
 * source:
 *    myValue.Set(this,"MyValue",&R::doSomething);
 *    myValue.Register();
 *    myValue.SetMinMax(0.0f,10.0f);
 *    myValue.SetStep(0.01f);
 *
 * Note that you have to register the variable before you can set further attributes
 * like min/max or the step size.
 *
 * For enum variables, please you EnumVar<R>...
 *
 */
template<class R, class T> class APIVar : public APIVarBase {
public:
    typedef T Policy;
    typedef typename T::Type Type;

    typedef void (R::*ChangedHandler)(APIVar& var);

    /** Standard constructor.
     *   DO NOT USE THIS CONSTRUCTOR.
     */
    APIVar() : APIVarBase(nullptr), payload(), thePlugin(nullptr), handler(nullptr) {
    }
    
    /** Standard constructor.
     *   DO NOT USE THIS CONSTRUCTOR.
     * @param val
     */
    APIVar(const Type &val) : APIVarBase(nullptr), payload(val), thePlugin(nullptr), handler(nullptr) {
    }

    /** Standard constructor.
     * @param rp    Reference pointer to the plugin class, must be "this" !
     * @param name  Name of the parameter in the UI.
     * @param h     Pointer to member function.
     */
    APIVar(R *rp, const char *name, ChangedHandler h) : APIVarBase(name), payload(), thePlugin(rp),
        handler(h) {
        
    }

    /** Destructor.
     *    DO NOT USE THIS DESTRUCTOR.
     *    UI parameters will be destroyed by the core api !
     */
    virtual ~APIVar() {
        Policy::delete_var(this->payload);
    }

    /** Register the new UI parameter
     *    You have to call Register() after Set().  
     *    Do not use the def string. Better use the convenience functions.
     * @param def  configure the UI parameter by ATB like string
     */
    virtual void Register(const char *def = nullptr) {
        if (this->thePlugin != nullptr && this->name != nullptr) {
            this->thePlugin->AddPluginParameter(T::TypeName, name, def,
                APIVarBase::SetCallBack, APIVarBase::GetCallBack, this);
        }
    }
    
    char* GetName() {
        return name;
    }


    /** Initialize a new UI parameter
     *    This function must be called in the Activate method of the derived
     *    render plugin. After initializing, you have to call Register.
     * @param rp    Reference pointer to the plugin class, must be "this" !
     * @param name  Name of the parameter in the UI.
     * @param h     Pointer to member function.
     */
    void Set(R *rp, const char *name, ChangedHandler h = nullptr) {
        this->setName(name);
        this->thePlugin = rp;
        this->handler = h;
    }
    
    /** Set minimum and maximum for UI parameter
     * @param minVal
     * @param maxVal
     */
    void SetMinMax(double minVal, double maxVal) {
        this->thePlugin->SetPluginParamMinMax(T::TypeName,this->name,minVal,maxVal);
    }
    
    /** Set step size for UI parameter
     * @param step
     */
    void SetStep(double step) {
        this->thePlugin->SetPluginParamStep(T::TypeName,this->name,step);
    }
    
    /** Set precision for UI parameter
     * @param prec
     */
    void SetPrecision(int prec) {
        this->thePlugin->SetPluginParamPrec(T::TypeName,this->name,prec);
    }   
    
    /** Make UI parameter readonly
     * @param readonly
     */
    void SetReadonly(bool readonly) {
        this->thePlugin->SetPluginParamReadOnly(T::TypeName,this->name,readonly);
    }
    
    /** Set help message.
     * @param hm   Help message text.
     */
    void SetHelpMessage( const char *hm ) {
        this->thePlugin->SetPluginParamHelpMessage(T::TypeName,this->name,hm);
    }
    
    /** Set axes for direction and quaternion parameters.
     *    The axes can only be in the main directions (x,y,z):
     *    AXIS_X,AXIS_MX, AXIS_Y,AXIS_MY, AXIS_Z,AXIS_MZ.
     * @param axisX  X-axis direction.
     * @param axisY  Y-axis direction.
     * @param axisZ  Z-axis direction.
     */
    void SetAxes( OGLApiAxis axisX, OGLApiAxis axisY, OGLApiAxis axisZ ) {
        this->thePlugin->SetPluginParamAxes(T::TypeName,this->name,axisX,axisY,axisZ);
    }
    
    /** Set keyboard shortcut to toggle a UI parameter
     * @param sc  shortcut string: e.g. m or M
     */
    void SetKeyShortcut( const char *sc ) {
        this->thePlugin->SetPluginParamKeyShortcut(T::TypeName,this->name,sc);
    }
    
    /** Set keyboard shortcut to increase a UI parameter
     * @param sc  shortcut string: e.g. m or M
     */
    void SetKeyShortcutIncr( const char *sc ) {
        this->thePlugin->SetPluginParamKeyShortcutIncr(T::TypeName,this->name,sc);
    }
    
    /** Set keyboard shortcut to decrease a UI parameter
     * @param sc  shortcut string: e.g. m or M
     */
    void SetKeyShortcutDecr( const char *sc ) {
        this->thePlugin->SetPluginParamKeyShortcutDecr(T::TypeName,this->name,sc);
    }

    void SetVisible( bool isVisible ) {
        this->thePlugin->SetPluginParamVisibility(T::TypeName,this->name,isVisible);
    }

    inline operator const Type&(void) const {
        return this->payload;
    }
    
    /** Set parameter value
     * @param val
     */
    virtual void SetValue(Type val) { 
        set(&val);
    }

    /** Get parameter value
     */
    Type GetValue() {
        return this->payload;
    }

    Type& operator=( const Type& rhs) {
        set(&rhs);
        return this->payload;
    }
    
    Type& operator+=( const Type& rhs) {
        this->payload += rhs;
        return this->payload;
    }

    Type& operator-=( const Type& rhs) {
        this->payload -= rhs;
        return this->payload;
    }
    

protected:
    virtual void set(const void *val) {
        T::set_copy(&this->payload, val);
        if ((this->thePlugin != NULL) && (this->handler != NULL)) {
            (this->thePlugin->*this->handler)(*this);
        }
    }

    virtual void get(void *val) {
        T::get_copy(val, &this->payload);
    }
    Type payload;

protected:
    R *thePlugin;
    ChangedHandler handler;
};

/**
 * Set the enum key-value pairs using this structure:
 *   EnumPair ep[] = {{0,"value1"},{1,"value2"}};
 *
 *   Please note that the string values must not have any spaces!
 */
typedef struct EnumPair_t {
    unsigned int id;
    const char*  text;
} EnumPair;


/**
 * @brief Template class for UI enum values.
 *
 * Enum variables have to be set using this APIVar variable:
 *
 *  header:
 *     EnumVar<R>  myEnumValue;
 *     void DoSomething( EnumVar<R> &var );
 *
 *  source:
 *     EnumPair ep[] = {{0,"value1"},{1,"value2"}};
 *     myEnumValue.Set(this,"MyEnum",ep,2,&R::DoSomething);
 *     myEnumValue.Register();
 *
 */
template<class R> class EnumVar : public APIVar<R, EnumVarPolicy> {
public:
#ifndef _WIN32
    typedef void (R::*ChangedHandler)(APIVar<R,EnumVarPolicy>& var);
#endif
    typedef void (R::*EnumChangedHandler)(EnumVar& var);

    EnumVar() {
    }

    void Set(R *rp, const char *name,  const EnumPair *value, unsigned int length,
            EnumChangedHandler h = nullptr) {
        this->Set(rp, name, h);
        if (value==nullptr || length==0) {
            return;
        }
        enumPairs.clear();
        for(unsigned int i=0; i<length; i++) {
            EnumPair ep = { value[i].id, value[i].text };
            enumPairs.push_back(ep);
        }
    }

    void Set(R *rp, const char *name, EnumChangedHandler h = nullptr) {
        APIVar<R, EnumVarPolicy>::Set(rp, name, reinterpret_cast<ChangedHandler>(h));
    }

    virtual void Register(const char *def = nullptr);

    void SetValue( const char* enumName ) { 
        if (this->thePlugin!=nullptr) {
            int val = this->thePlugin->GetCurrentPluginParamEnumID(this->GetName(),enumName);
            if (val>=0) {
                APIVar<R,EnumVarPolicy>::set(&val);
            }
        }
    }

    int& operator=( const int& rhs) {
        this->payload = rhs;
        return this->payload;
    }

    // Cannot be used in the C'tor of a Plugin!
    int& operator=( const char* rhs) {
        SetValue(rhs);
        return this->payload;
    }

private:
    std::vector<EnumPair> enumPairs;
};


template<class R>
    void EnumVar<R>::Register(const char *def) {
        std::string mydef;
        std::stringstream es;
        if (enumPairs.size()>0) {
            es << "enum='";
            for (unsigned int x = 0; x < enumPairs.size(); x++) {
                es << enumPairs[x].id << " {" << enumPairs[x].text << "}";
                if (x+1 < enumPairs.size()) es << ", ";
            }
            es << "'";
        }
        if (def != nullptr) {
            mydef = std::string(def) + " " + es.str();
        } else {
            mydef = es.str();
        }
        APIVar<R, EnumVarPolicy>::Register(mydef.c_str());
    }


/**
 * @brief Template class for OpenGL state variables.
 *
 *  This is just a convenience template to toggle OpenGL state variables.
 *  Set the variable to  true to enable and false to disable the state.
 *  For example:
 *    header:    GLStateVar<PluginName>  useBlending;
 *    source:    useBlending.Set(this,"UseBlending", GL_BLEND, &PluginName::selectBlending);
 *               useBlending.Register();
 *               useBlending = true;
 *
 *    void PluginName::selectBlending( GLStateVar<PluginName> &var ) {
 *         if (var) {
 *             glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 *         }
 *    }
 */
template<class R> class GLStateVar : public APIVar<R, BoolVarPolicy> {
public:
#ifndef _WIN32
    typedef void (R::*ChangedHandler)(APIVar<R,BoolVarPolicy>& var);
#endif    
    typedef void (R::*GLStateHandler)(GLStateVar& var);

    GLStateVar() : myEnum(0) {}
    
    void Set(R *rp, const char *name, unsigned int myEnum, GLStateHandler h = nullptr) {
        this->setName(name);
        this->thePlugin = rp;
        this->myEnum = myEnum;

        this->Set(rp, name, reinterpret_cast<ChangedHandler>(h));
    }
    
    void SetValue( bool val ) {
        this->payload = val;
        if (this->thePlugin!=nullptr) {
            this->thePlugin->ToggleGLState(myEnum,val);
            if (this->handler != NULL) {
                (this->thePlugin->*this->handler)(*this);
            }
        }
    }

    void Synchronize() {
        if (this->thePlugin!=nullptr) {
            bool state;
            this->thePlugin->SynchronizeGLState( this->myEnum, state );
            SetValue(state);
        }
    }

    bool& operator=( const bool& rhs ) {
        SetValue(rhs);
        return this->payload;
    }

protected:
    void Set(R *rp, const char *name, ChangedHandler h = nullptr) {
        APIVar<R, BoolVarPolicy>::Set(rp, name, h);
    }

    virtual void set(const void *val) {
        BoolVarPolicy::set_copy(&this->payload, val);
        if (this->thePlugin != NULL) {                      
            this->thePlugin->ToggleGLState(myEnum,this->payload);
            if (this->handler != NULL) {
                (this->thePlugin->*this->handler)(*this);
            }
        }
    }

protected:
    unsigned int myEnum;
};


/**
 * @brief Template class for UI string values.
 */
template<class R> class StringVar : public APIVar<R, CStringVarPolicy> {
public:
    StringVar() {}

    void SetValue( const char* val ) {
        if (this->thePlugin!=nullptr) {
            char* s = const_cast<char*>(val);
            APIVar<R,CStringVarPolicy>::set(&s);
        }
    }    
    
    char*& operator=( const char* rhs) {
        char* s = const_cast<char*>(rhs);
        APIVar<R,CStringVarPolicy>::set(&s);
        return this->payload;
    }
};

/**
 * @brief Template class for UI file selector.
 * 
 *  A combo box will be created in the UI where all filenames
 *  with a predefined file ending and file path are selectable. 
 *  Within the "Activate()" method, call "Set" and "Register".
 * 
 */
template<class R> class FileEnumVar : public APIVar<R, EnumVarPolicy> {
public:
#ifndef _WIN32
    typedef void (R::*ChangedHandler)(APIVar<R,EnumVarPolicy>& var);
#endif    
    typedef void (R::*FileEnumChangedHandler)(FileEnumVar& var);

    /** Standard constructor.
     *   DO NOT CALL THIS CONSTRUCTOR.
     */
    FileEnumVar() : filePathNames(), basePath() {
    }

    /** Register the file selector UI
     *    You should not need to configure the file selector.
     * @param def   configure the UI parameter by ATB like string
     */
    virtual void Register(const char *def = nullptr);
    
    /** Initialize the file selector UI
     *    The path can be made relative by using the GetCurrentPluginPath()
     *    method.
     * @param rp    Reference pointer to the plugin class, must be "this" !
     * @param name  Name of the parameter in the UI.
     * @param path  Absolute path to file directory.
     * @param extensions  File endings (separated by semicolon).
     * @param h     Pointer to member function.
     */
    void Set(R *rp, const char *name, const char *path, const char *extensions,
            FileEnumChangedHandler h = nullptr);

    /** Get selected filename.
     *   Take care that every call to this function allocates memory
     *   that must be free'd by the calling method !!
     * @return pointer to filename.
     */
    const char *GetSelectedFileName() {
        if (this->payload < 1 || this->payload > static_cast<int>(filePathNames.size())) {
            return nullptr;
        }        
        return filePathNames[this->payload-1].c_str();
    }
    
    /** Get the number of files in the list.
     * @return Number of files.
     */
    unsigned int GetNumberOfFiles() {
        return filePathNames.size();
    }

    /** Compare file endings.
     * @param  ext  File ending to test with.
     * @return  true if selected file has the extension given by 'ext'.
     */
    bool SelectedFileEndsWith( const char *ext ) {
        if (this->payload < 1 || this->payload > static_cast<int>(filePathNames.size())) {
            return false;
        }
        return this->thePlugin->StringEndsWith( filePathNames[this->payload-1].c_str(), ext );
    }

    /** Find filename by name.
     * @param name  Name of the file.
     * @return true if file exists.
     */
    bool Find( const char* name ) {
        std::string sn = std::string(name);
        size_t flen = sn.length();
        
        unsigned int id = 0;
        while(id<filePathNames.size()) {
            int pos = static_cast<int>(filePathNames[id].length() - flen);
            if (pos>=0) {
                std::string str = filePathNames[id].substr(pos,flen);
                if (filePathNames[id].compare(pos,flen,sn)==0) {      
                    int n_payload = id + 1;
                    this->set((void*)&n_payload);
                    return true;
                }
            }
            id++;
        }
        return false;
    }

    /** Select file by id
     * @param id
     */
    void Select( unsigned int id ) {
        if (id < filePathNames.size()) {
            this->payload = id;
            this->set((void*)&id);
        }
    }
    
    int& operator=( const int& rhs) {
        this->payload = rhs;
        return this->payload;
    }

private:
    void Set(R *rp, const char *name, ChangedHandler h = nullptr) {
        APIVar<R, EnumVarPolicy>::Set(rp, name, h);
    }

    std::vector<std::string> filePathNames;
    std::string basePath;
};


/**
 * @brief Template class for UI buttons.
 * 
 *  To add a button to the UI for use in your plugin, you have to add
 *  "ButtonVar<PluginName> varName;" to the header. 
 *  Within the "Activate()" method, call "Set" and "Register".
 * 
 */
template<class R> class ButtonVar : public APIVar<R, ButtonPolicy> {
public:
#ifndef _WIN32
    typedef void (R::*ChangedHandler)(APIVar<R,ButtonPolicy>& var);
#endif    
    typedef void (R::*ButtonChangedHandler)(ButtonVar& var);
    
    ButtonVar() {}
    
    virtual void Register( const char *def = nullptr ) {
        if (this->thePlugin != nullptr) {
            this->thePlugin->AddButton( APIVar<R, ButtonPolicy>::GetName(), def, APIVarBase::SetButtonCB, this );
        }
    }
    
    void Set(R *rp, const char *name, ButtonChangedHandler h = nullptr ) {
        this->Set(rp,name, reinterpret_cast<ChangedHandler>(h));
    }
    
private:
    void Set(R *rp, const char *name, ChangedHandler h = nullptr) {
        APIVar<R, ButtonPolicy>::Set(rp,name,h);
    }        
};    


template<class R> class FPSCounterVar : public APIVar<R, FloatVarPolicy> {
public:
    FPSCounterVar() {}
    
    virtual void Register(const char *def = nullptr) {
        APIVar<R, FloatVarPolicy>::Register();
        APIVar<R, FloatVarPolicy>::SetReadonly(true);    
        APIVar<R, FloatVarPolicy>::SetPrecision(1);    
        frameCounter  = 0;
    }
    
    void Set(R *rp, const char *name, int resetNum) {
        APIVar<R, FloatVarPolicy>::Set(rp, name, nullptr);
        this->frameResetNum = resetNum;
    }
    
    void Start() {
        if (frameCounter % frameResetNum == 0) {
            startTime = std::chrono::system_clock::now();
            frameCounter = 0;
        }
    }
    
    void Stop() {
        frameCounter++;
        if (frameCounter % frameResetNum == 0) {
            currTime = std::chrono::system_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(currTime-startTime).count();
            float val = static_cast<float>( frameCounter / static_cast<double>(elapsed_time*1e-3) );
            APIVar<R,FloatVarPolicy>::SetValue(val);
        }
    }
    
    void SetResetNum( int num ) {
        frameResetNum = num;
        frameCounter = 0;
    }
    
private:
    std::chrono::time_point<std::chrono::system_clock>  startTime, currTime;
    int frameCounter;
    int frameResetNum;
};


template<class R>
void FileEnumVar<R>::Set(R *rp, const char *name, const char *path, const char *extensions,
        FileEnumChangedHandler h ) {
    
    this->Set(rp, name, reinterpret_cast<ChangedHandler>(h));
    if (extensions == nullptr || path == nullptr) {
        return;
    }
    std::istringstream iss(extensions);
    std::vector<std::string> suffixes;
    std::string s;
    while(std::getline(iss, s, ';')) {
        if (s[0] != '.') {
            s = "." + s;
        }
        suffixes.push_back(s);
    }
    filePathNames.clear();
    basePath = std::string(path);
#ifdef _WIN32
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATAA ffd;
    hFind = FindFirstFileA((std::string(path) + "\\*").c_str(), &ffd);
    if (hFind != INVALID_HANDLE_VALUE)  {
        do {
            if (strcmp(ffd.cFileName, ".") != 0 && 
                strcmp(ffd.cFileName, "..") != 0) {
                    if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        //dirs.push(path + "\\" + ffd.cFileName);
                    } else {
                        std::string f(ffd.cFileName);
                        for (auto ext : suffixes) {
                            size_t offset = f.rfind(ext);
                            if (offset != std::string::npos && offset + ext.length() == f.length()) {
                                filePathNames.push_back(basePath + "\\" + std::string(ffd.cFileName));
                            }
                        }
                    }
            }
        } while (FindNextFileA(hFind, &ffd) != 0);
        FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }
#else
    DIR *d = nullptr;
    struct dirent *currDir;
    basePath += "/";  
    if (chdir(basePath.c_str())!=0) {
        fprintf(stderr,"Folder %s not found.\n",basePath.c_str());
        return;
    }

    if ((d = opendir(".")) == NULL) {
        fprintf(stderr,"Cannot open dir.\n");
        return;
    }

    while ((currDir = readdir(d))) {
        if (strcmp(currDir->d_name,".")==0 ||
                strcmp(currDir->d_name,"..")==0 ||
                strncmp(currDir->d_name,".",1)==0) {
            continue;
        }
        if (currDir->d_type != DT_DIR) {
            auto fn = std::string(currDir->d_name);
            for (auto ext : suffixes) {
                if (ext.length() <= fn.length()) {
                    auto pos = fn.compare(fn.length()-ext.length(),ext.length(),ext);
                    if (pos==0) {    
                        filePathNames.push_back(basePath + std::string(currDir->d_name));
                    }
                }
            }
        }
    }
    closedir(d);
#endif
    //std::sort(fileNames.begin(),fileNames.end());
    std::sort(filePathNames.begin(),filePathNames.end());
}


template<class R>
void FileEnumVar<R>::Register(const char *def) {
    std::string mydef;
    std::stringstream es;
    es << "enum='0 {none} ";
    for (unsigned int x = 0; x < filePathNames.size(); x++) {
        size_t fileLen = filePathNames[x].length();
#ifdef _WIN32
        size_t posSep = filePathNames[x].find_last_of("\\");
        std::string filename = filePathNames[x].substr(posSep+1,fileLen-posSep);
#else
        size_t posSep = filePathNames[x].find_last_of("/");
        std::string filename = filePathNames[x].substr(posSep+1,fileLen-posSep);    
#endif                
        es << ", " << x+1 << " {" << filename << "}";
    }
    es << "'";
    if (def != nullptr) {
        mydef = std::string(def) + " " + es.str();
    } else {
        mydef = es.str();
    }
    APIVar<R, EnumVarPolicy>::Register(mydef.c_str());
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_RENDERPLUGIN_H_INCLUDED */
