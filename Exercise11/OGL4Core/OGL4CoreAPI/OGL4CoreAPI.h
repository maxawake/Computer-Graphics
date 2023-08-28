/**
 * @brief  Core application programming interface (API) of OGL4Core - header.
 * 
 * @file   OGL4CoreAPI.h
 * 
 * Copyright (c) 2012,2013 by Universitaet Stuttgart (VISUS).
 * All rights reserved.
 * 
 * Authors: Guido Reina, Thomas Mueller
 * 
 * This file is part of OGL4Core.
 */

#ifndef VISUS_OGL4COREAPI_H_INCLUDED
#define VISUS_OGL4COREAPI_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OGL4COREAPI_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OGL4COREAPI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef _WIN32
#include <Windows.h>
#ifdef OGL4COREAPI_EXPORTS
// http://support.microsoft.com/kb/168958/en-us
#define OGL4COREAPI_API __declspec(dllexport)
#else
#define OGL4COREAPI_API __declspec(dllimport)
#endif
#define OGL4COREAPI_CALL __stdcall
#else
#define OGL4COREAPI_API 
#define OGL4COREAPI_CALL
#undef APIENTRY
#define APIENTRY
#endif

class RenderPlugin;

#include "RenderPlugin.h"
#include <vector>
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "Manipulator.h"
//#include "Overlay.h"
#include <map>

// This class is exported from the OGL4CoreAPI.dll
/**
 *  @brief The COGL4CoreAPI class
 */
class OGL4COREAPI_API COGL4CoreAPI {    
public:
    
    enum MouseState {
       MOUSE_OGL_DOWN = 0,
       MOUSE_TBAR_DOWN = 1,
       MOUSE_UP
    };
    
    enum MouseButton {
        MOUSE_LEFT_BUTTON   = 0,
        MOUSE_MIDDLE_BUTTON = 1,
        MOUSE_RIGHT_BUTTON  = 2
    };
    
    //static inline COGL4CoreAPI *GetInstance() {
    //    if (currentInstance == nullptr) {
    //        currentInstance = new COGL4CoreAPI(0, NULL);
    //    }
    //    return currentInstance;
    //}

    typedef void(*LeaveEventLoopT)(void);
    typedef bool(*SetWindowSizeT)(int, int);
    typedef bool(*SetWindowPosT)(int, int);
    typedef bool(*SetFullScreenT)(bool);
    typedef void(*PostRedisplayT)(void *);

    typedef struct PluginLocStruct {
        std::string path;
        std::string fullname;
    } PluginLocT;

    // ctor.
    COGL4CoreAPI(int argc, char* argv[], const char *baseDir,
                           LeaveEventLoopT leaveEventLoop, SetWindowSizeT setWindowSize,
                           SetWindowPosT setWindowPosition,
                           SetFullScreenT setFullScreen, PostRedisplayT postRedisplay,
                           bool showUI, bool useDebugging,
                           void *userData = nullptr);

    ~COGL4CoreAPI();

    void Display(void);
    void Idle(void);
    void Keyboard(unsigned char key, int x, int y);
    void Motion(int x, int y);
    void Mouse(int button, int state, int x, int y);
    void MouseRelease(int button, int state, int x, int y);
    void PassiveMotion(int x, int y);
    void PostRedisplay();
    void MouseWheel(int wheel, int direction, int x, int y);
    void Resize(int w, int h);
    void SpecialKey(int key, int x, int y);
    static void SetRenderer(const void *value, void *clientData);
    static void GetRenderer(void *value, void *clientData);

    void PrintAvailableExtensios();
    bool IsExtensionAvailable( const char* extName );
    //void UseKHRDebugging( bool useIt );

    static void APIENTRY DeallocUI(void);

    static void APIENTRY DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, void* userParam);

    void ExitApplication();
  
    void SetGuiVisible(bool visible);

    bool GetGuiVisible();

    void SetGuiColor( int red, int green, int blue, int alpha = 128 );
    void ResetGuiColor();
    void SetGuiTextLightColor( bool lightText = true );

    bool AddPlugin(RenderPlugin *plug);

    void AddPluginParameter(OGLApiType type, const char *group, const char *name, const char *def,
        SetVarCallbackT setCallback, GetVarCallbackT getCallback, void *clientData);
    
    void SetPluginParamMinMax( OGLApiType type, const char *group, const char *paramName, double min, double max );
    void SetPluginParamStep( OGLApiType type, const char *group, const char *paramName, double step );
    void SetPluginParamPrec( OGLApiType type, const char *group, const char *paramName, int numSignificantDigits );
    void SetPluginParamReadOnly( OGLApiType type, const char *group, const char *paramName, bool readOnly );
    void SetPluginParamVisibility( OGLApiType type, const char *group, const char *paramName, bool isVisible );
         
    void SetPluginParamHelpMessage( OGLApiType type, const char *group, const char *paramName, const char *hm );
    void SetPluginParamAxes( OGLApiType type, const char *group, const char *paramName, OGLApiAxis axisX, OGLApiAxis axisY, OGLApiAxis axisZ );
    void SetPluginParamShortcut( OGLApiType type, const char *group, const char *paramName, const char *sc );
    void SetPluginParamShortcutIncr( OGLApiType type, const char *group, const char *paramName, const char *sc );
    void SetPluginParamShortcutDecr( OGLApiType type, const char *group, const char *paramName, const char *sc );
    
    void AddButton(const char* group, const char *name, const char *def,
        SetButtonCallbackT setCallback, void *clientData);
        
    int AddManipulator(const char *group, const char *name, glm::mat4 *matrix, 
        Manipulator::ManipulatorType type = Manipulator::MANIPULATOR_TRACKBALL,
        bool autorotate = false);    

    bool EnumeratePlugins(const char *path, bool isSelectable = true);
    bool RegisterPlugins();

    void SetWindowSize(int width, int height);
    void GetWindowSize(int &width, int &height);
    void SetWindowPosition(int x, int y);
    
    void SetFullScreen(bool fs);
    void SetModifiers(int mods);
    
    bool SelectCurrentRenderer(int pluginIndex); 
    bool SelectCurrentRenderer(const char* name);
    int  GetRendererID(const char* name);
    void ShowAvailableRenderers();
    
    bool SelectCurrentManipulator(int manipulatorIdx);
    void RegisterCurrentManipulator();
    
    bool EnableManipulator( const int h );
    bool DisableManipulator( const int h );
    bool SetManipulatorDolly( const int h, float dolly );
    bool GetManipulatorDolly( const int h, float &dolly );
    bool SetManipulatorPan( const int h, glm::vec2 pan );
    bool GetManipulatorPan( const int h, glm::vec2 &pan );
    bool SetManipulatorRotAngle( const int h, float angle );
    bool GetManipulatorRotAngle( const int h, float &angle );
    bool SetManipulatorRotation( const int h, glm::vec3 a, float angle );
    bool GetManipulatorRotation( const int h, glm::vec3 &a, float &angle );
    bool SetManipulatorScale( const int h, float scale );
    bool GetManipulatorScale( const int h, float &scale );

   
    //bool SelectCurrentOverlay(int overlayIdx);
    //void RegisterCurrentOverlay();

    //int AddOverlay(const char *group, const char *name, int x, int y, int width, int height, Overlay* overlay);

    RenderPlugin*  AddOverlay(const char *group, const char *name, int x, int y, int width, int height, const char *pluginname);

            
    void SetLastMousePos(int x, int y);
    void GetLastMousePos(int &x, int &y);
    void       SetLastMousePos(glm::ivec2 pos);
    glm::ivec2 GetLastMousePos();
    
    void SetLastMouseDownPos(int x, int y);
    void GetLastMouseDownPos(int &x, int &y);
    void       SetLastMouseDownPos(glm::ivec2 pos);
    glm::ivec2 GetLastMouseDownPos();
    
    void SetMouseState(int state);
    int  GetMouseState();
    void SetMouseButtonPressed(int button);
    int  GetMouseButtonPressed();
    
    bool IsLeftButtonPressed();
    bool IsMidButtonPressed();
    bool IsRightButtonPressed();
    bool IsMouseDownWindow();
    bool IsMouseDownUI();
    bool IsMouseReleased();
    
    bool IsCtrlPressed();
    bool IsShiftPressed();
    
    // you need to delete the returned value!
    const char *GetCurrentPluginPath() const;

    // you need to delete the returned value!
    const char *GetBasePath() const;

    const char *GetCurrentPluginParamName();
    
    int GetCurrentPluginParamEnumID( const char* paramName, const char* enumName );

    /** Write current plugin parameters to file.
     *   If no filename is given, then the plugin name is used as filename.
     * @param paramFilename   Parameter filename.
     */
    void WritePluginParamsToFile( const char* paramFilename = nullptr );

    /** Read current plugin parameters from file.
     *   If no filename is given, then the plugin name is used as filename.
     * @param paramFilename   Parameter filename.
     */
    void ReadPluginParamsFromFile( const char* paramFilename = nullptr );

    void GrabPluginFramebuffer();

    void ToggleGLState( unsigned int var, bool state );

    void SynchronizeGLState( unsigned int var, bool &state );
    bool StringEndsWith( const char* name, const char* ending );
    
    void EnablePicking();
    void DisablePicking();
    bool StartPickable( unsigned int id );
    void EndPickable();
    int  DoPick( int x, int y );

protected:
    std::string genUniqueName(const std::string barName, const char *group, const char *name);
    void replaceAll(std::string &str, const std::string &from, const std::string &to);    
    bool endsWith(const std::string &str, const std::string &ext);

private:
// we are not publishing any of that, thank you very much
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
    typedef struct PluginParStruct {
        std::string paramName;
        OGLApiType  paramType;
        std::map<int, std::string> enumData;
        void*       clientData;
    } PluginParStructT;

    //static COGL4CoreAPI *currentInstance;
    
    char *baseDir;

    struct private_t;
    private_t *m_private;
    std::string barName;
    
    RenderPlugin *currentPlugin;
       
    int currPluginIndex;    
    std::vector<RenderPlugin *> pluginList;
    std::vector<PluginParStructT> currPluginParameters;
    
    int currManipulatorIndex;
    int prevManipulatorIndex;
    std::vector<Manipulator> manipulators;
    
    int currOverlayIndex;
    int prevOverlayIndex;
    std::vector<RenderPlugin*> overlays;
    
    //std::string paramFilename;
    
    glm::ivec2  windowSize;
    glm::ivec2  lastMousePos;
    glm::ivec2  lastMouseDownPos;
    MouseState  mouseButtonState;
    int         mouseButton;
    int         modifiersState;
    bool        moving;
    
    bool        myTweakBarActive;
    //std::string myTweakBarName;
    int         myTweakBarNum;
    static bool tweakBarInitialized;

    LeaveEventLoopT leaveEventLoop;
    SetWindowSizeT setWindowSize;
    SetWindowPosT  setWindowPosition;
    SetFullScreenT setFullScreen;
    PostRedisplayT postRedisplay;
    void *callbackUserData;
    
    bool pickingEnabled;
    bool isPicking;
    
    
    
    
#ifdef _WIN32
#pragma warning(pop)
#endif
};

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* VISUS_OGL4COREAPI_H_INCLUDED */
