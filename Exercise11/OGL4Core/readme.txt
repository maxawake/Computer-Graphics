## -------------------------------------------------------
##   OGL4Core -- A Framework for Visualization Education
##
##   Authors:    Guido Reina, Thomas Mueller
##   Copyright:  2013, 
##               Universitaet Stuttgart, VISUS
##               Allmandring 19
##               70569 Stuttgart
##   
##   Last modified:   2014-04-13
## -------------------------------------------------------
    

CONTENT:
--------
*) Build a new plugin
*) OpenGL >3 support
*) Add a 
    .) float variable to the UI
    .) file selector to the UI
    .) enum variable to the UI
    .) button to the UI
    .) manipulator to the UI
*) Convenience classes
    .) DatRawLoader
    .) FramebufferObject
    .) GLShader
    .) ObjLoader
    .) VertexArray


====================================================================

BUILD A NEW PLUGIN:
-------------------
    1.) Open a command shell (Linux) or a powershell (Windows) 
        and go to the "Plugins" directory.

    2.) To create a new plugin called "MyFirstPlugin" within the
        subfolder "MyPlugins", call
    
        Linux:    perl makePlugin.pl  MyPlugins/MyFirstPlugin
        Windows:  .\makePlugin.ps1  MyPlugins/MyFirstPlugin
    
        Now, the following files should have been generated:
    
        MyPlugins/
        +-- MyFirstPlugin
            +-- CoreDirectory.props
            +-- Makefile
            +-- MyFirstPlugin.cpp
            +-- MyFirstPlugin.h
            +-- MyFirstPlugin.sln
            +-- MyFirstPlugin.vcxproj
            +-- MyFirstPlugin.vcxproj.filters
            +-- MyFirstPlugin.vcxproj.user
            +-- OGL4Plug.make
            +-- stdafx.h
            +-- targetver.h
    
        Windows only: because of signing problems you might have to call
            Set-ExecutionPolicy unrestricted
        as administrator from within a powershell if your machine
        refuses to execute the script.
    
    3.) On Linux machines, go to the MyFirstPlugin folder and
        call make. 
        
        Note that your compiler has to support c++11. You might
        need to call make via:
            CXX=g++-4.7 CC=gcc-4.7 LINK=g++-4.7 make -e
        or similar.    
        
        On Windows machines, open the MyFirstPlugin.sln within
        Visual Studio and press 'F7' to build the solution.
        
    Now, your new plugin should be available.
    
    
    
PLEASE NOTICE:
--------------
    A standard plugin has to override the following pure virtual methods
    inherited from the 'RenderPlugin' class:
        virtual bool Activate(void);
        virtual bool Deactivate(void);
        virtual bool Init(void);
        virtual bool Render(void);    
     
    The 'Init' method is called once when OGL4Core collects all available 
    plugins. Here, you should test, for example, if the system can support 
    your plugin (see OPENGL >3 SUPPORT).

    When a plugin is selected, the core first deactivates the previous plugin by
    calling 'Deactivate', and then it calls 'Activate' of the current plugin.

    Within 'Activate' you can add parameters, buttons, or file selectors
    to the UI. Here, you should also prepare all things that your plugin
    needs. For example, you can set the clear color, load textures, set
    vertex arrays (VAs) or frame buffer objects (FBOs), load shaders,...

    Please take care that you free all memory and deallocate all OpenGL resources
    in 'Deactivate' that you have allocated in 'Activate' !!!

    The 'Render' method is exactly that what you think ;-)

    Like in any other GLUT program, you can also set callback functions
    for keyboard and mouse handling. For that, override the following
    methods provided by the 'RenderPlugin' class:
       virtual bool Keyboard(unsigned char key, int x, int y);
       virtual bool Motion(int x, int y);
       virtual bool Mouse(int button, int state, int x, int y);
       virtual bool PassiveMotion(int x, int y)

    And the other callback functions:
       virtual bool Idle(void);
       virtual bool Resize(int width, int height);

    If your plugin has handled all the keyboard or mouse events by itself,
    and wants to hide the user input from the core, it should return 'true',
    otherwise 'false'.

    There are also some convenience methods for mouse handling,
    have a look at 'RenderPlugin.h'.

    
OPENGL >3 SUPPORT:
------------------
    If your plugin needs OpenGL 3 or higher, you should add a call 
    to gl3wInit within the Init() method:
    
    if (gl3wInit()) {
       fprintf(stderr,"Error: Failed to initialize gl3w.\n");
       return false;
    }  
    
    You also have to replace  
    #include "GL/gl.h"
    by
        #include "GL3/gl3w.h"
    within your MyFirstPlugin.cpp file.

    To test for a specific extension, you can call
       IsExtensionAvailable("GL...");
    
    With OpenGL 3 you are also responsible to add a shader program
    which is simplified by means of the GLShader convenience class.

    Windows only: Do not forget to add "gl3w.c" to the sources 
    of your solution as 'Add existing item'. The file can be found
    in "<OGL4Core>/gl3w/src" folder. 
    

ADD A FLOAT VARIABLE TO THE UI:
-------------------------------
    In order to have a variable available in the UI, you have to
    register it within the Activate() method. But first, you have
    to declare the variable in the plugin's header file. 

    To add a float variable 'param', you should do the following:

    1.) In MyFirstPlugin.h, add
            APIVar<MyFirstPlugin, FloatVarPolicy>  param;

    2.) In MyFirstPlugin.cpp, you have to register the parameter
        within the Activate() method:
            param.Set(this, "ParamName");
            param.Register();
          
        Now, you can access the variable 'param' via the setter
        and getter methods 'SetValue()' and 'GetValue()', or
        you can use 'param' like a usual float value.

    3.) If you also need a callback function for your variable,
        replace
            param.Set(this, "ParamName");
        by
            param.Set(this, "ParamName", &MyFirstPlugin::ParamChanged);

        and add the method
            void ParamChanged(APIVar<MyFirstPlugin, FloatVarPolicy> &var);
        to your plugin.

    4.) If your parameter shall also be bounded by minimum and
        maximum values, add the following lines after the call 
            param.Register();
            param.SetMinMax(0.0,10.0);

        Step size and precision can be set via
            param.SetStep(0.01);
            param.SetPrecision(2);

        And if the parameter shall be readonly, set
            param.SetReadonly(true);

    It is not necessary to remove the parameter from the UI by yourself.
    This is handled by OGL4Core.



ADD A FILE SELECTOR TO THE UI:
------------------------------    
    Files within a subfolder 'myfolder' having the same extension
    can be made available from within the UI by means of a FileEnumVar.
    For that, you have to do the following:

    1.) In MyFirstPlugin.h, add
            FileEnumVar<MyFirstPlugin> myFiles;
            
        A callback method would look like this
            void  doSomething( FileEnumVar<MyFirstPlugin> &var );

    2.) In MyFistPlugin.cpp you have to register the file selctor
        within the Activate() method:
            std::string pathName = this->GetCurrentPluginPath();
            myFiles.Set(this, "Filename", (pathName+"/myfolder/").c_str(), ".fe1;.fe2;.fe3", 
                        &MyFirstPlugin::FileChanged);
            myFiles.Register();

        GetCurrentPluginPath() is convenience method which lets your plugin
        know where its dll is located. You can construct relative paths to
        your resources from there if desired.

        'myfolder' should be located below your plugin folder.

        '.fe1;.fe2;.fe3' is a semicolon seperated list of file endings
        that shall be available from the file selector.

     3.) If your plugin shall do something with the selected file,
         you have to add the method
            void FileChanged(FileEnumVar<MyFirstPlugin> &var);

    It is not necessary to remove the file selector from the UI by yourself.
    This is handled by OGL4Core.



ADD AN ENUM VARIABLE TO THE UI:
-------------------------------
    An enum variable can be used, e.g., to select a filter by its name
    that is internally handled by an integer.

    1.) In MyFirstPlugin.h  add
            EnumVar<MyFirstPlugin> filterID; 
            
        A callback method would look like this
            void  doSomething( EnumVar<MyFirstPlugin> &var );

    2.) In the Activate() method of MyFirstPlugin.cpp you first have to
        create the list of enum pairs:
        
            EnumPair ep[] = {{0,"none"},{1,"Gray"},{2,"Gradient"}};
                    
        Note that the filter names must NOT contain blanks!
            
        Afterwards, you can set and register the enum variable:
            filterID.Set(this, "Filter", ep, 3);            
            filterID.Register();
            
        You can also select a standard filter when the plugin is activated:
            filterID = 0;



ADD A BUTTON TO THE UI:
-----------------------
    To add a button to the UI, you have to do the following:

    1.) In MyFirstPlugin.h  add
            ButtonVar<MyFirstPlugin>  myButton;

    2.) In MyFirstPlugin.cpp you have to register the button
        within the Activate() method:
            myButton.Set(this,"Do something",&MyFirstPlugin::doSomething);
            myButton.Register();
    
    3.) For a button it is crucial to add a callback:
            void doSomething( Button<MyFirstPlugin> &var);

        Otherwise, the button would be useless.

    It is not necessary to remove the button from the UI by yourself.
    This is handled by OGL4Core.



ADD A MANIPULATOR TO THE UI:
----------------------------
    The idea of a manipulator is to modify a 4x4 matrix that can be
    used for example as a VIEW- or a MODEL-matrix. In OGL4Core there
    are already several manipulators implemented which can be accessed
    by there enum value.
     
    A manipulator can be added to a plugin within the Activate method.
    For example:
    
    this->AddManipulator( "View",
                          &this->viewTrafo,
                          Manipulator::MANIPULATOR_ORBIT_VIEW_3D,
                          true );
                          
    'viewTrafo' must be of type 'glm::mat4' and is the matrix that will
    be modified by the manipulator. 



CONVENIENCE CLASSES:  DatRawLoader
---------------------------------------
TODO


CONVENIENCE CLASSES:  FramebufferObject
---------------------------------------
    OpenGL offline rendering can be realized using a framebuffer object. 

    1.) In your plugin's .h-File add:
          #include "FramebufferObject.h"
          ...
          FramebufferObject fbo;

    2.) In your plugin's .cpp-File you have to initialize the framebuffer
        object. In the following example, the framebuffer object consists
        of a color attachment and a depth attachment where the depth is
        stored within a texture. The stencil attachment is not used.

          FramebufferObject::ColorAttachParams cap;
          FramebufferObject::DepthAttachParams dap;
          dap.state = FramebufferObject::ATTACHMENT_TEXTURE;
          FramebufferObject::StencilAttachParams sap;
          fbo.Create( fboWidth, fboHeight, 1, cap, dap, sap );

    3.) To render into the framebuffer object, you have to enable the
        corresponding color attachment:
          fbo.Enable();
          ...
          fbo.Disable();

    4.) In the Render() method, e.g., you can access the framebuffer
        color content via binding to a texture unit:

           glActiveTexture( GL_TEXTURE0 );
           fbo.BindColorTexture(0);
           ...
           fbo.UnbindTexture();

    5.) Do not forget to release the framebuffer object if you resize it
        or when you leave the plugin.

           fbo.Release();



CONVENIENCE CLASSES:  GLShader
------------------------------
    OpenGL shader programs can be build with the GLShader convenience class.
    E.g., if the vertex and fragment shaders are given as text files, you
    have to do the following:

      1.) In your plugin's .h-File add:
            #include "GLShader.h"
            ...
            GLShader  shader;

      2.) In your plugin's .cpp-File go to the Activate() method and obtain
          the plugin path via:
            std::string pathName = this->GetCurrentPluginPath();
          Then, set the paths to your vertex and fragment shader files:
            std::string vShaderName = pathName + std::string("/resources/vert.c");
            std::string fShaderName = pathName + std::string("/resources/frag.c");

      3.) Now, you can create the shader program via:
            bool isOkay = shader.CreateProgramFromFile(vShaderName.c_str(),fShaderName.c_str());

      4.) Within your Render() method you can bind or release the shader via:
            shader.Bind();
            ...
            shader.Release();

      5.) To set a uniform variable like, e.g. a float, you can use the following method
            glUniform1f( shader.GetUniformLocation("varName"), varValue );

      6.) Do not forget to call  
            shader.RemoveAllShaders()
          in your Deactivate() method.
            


CONVENIENCE CLASSES:  ObjLoader
-------------------------------
TODO



CONVENIENCE CLASSES:  VertexArray
---------------------------------
	The VertexArray convenience class represents a vertex array object that can
    be filled with several vertex buffer objects.

      1.) In your plugin's .h-File add:
            #include "VertexArray.h"
            ...
            VertexArray va;

      2.) To create a new vertex array object, you first have to set, how many
          vertices shall be stored in the array:
            va.Create(4);

      3.) Until now, the vertex array object is empty. If your vertices are 
          stored on the host in a float array
            float verts[] = { 0.0f,0.0f, 1.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f };
          you can put them into the vertex buffer object with index 'idx' of
          the vertex array via
            va.SetArrayBuffer( idx, GL_FLOAT, 2, verts );
          Here, the third entry defines the dimension of your vertices.

      4.) In you Render() method, you can bind or release the vertex array via
            va.Bind();
            ... // draw command
            va.Release();

      5.) Do not forget to delete the vertex array content via
            va.Delete()
          within your Deactivate() method or if you refill the vertex array.


