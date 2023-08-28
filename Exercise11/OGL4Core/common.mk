
Q = #@

CXX      	= g++
CC        	= gcc 
LINK     	= g++

ifeq ($(shell uname -m), x86_64)
	BITS := 64
else
	BITS := 32
endif


ifeq ($(mode),release)
	CXXFLG += -O3 -DNDEBUG
	DEBREL = 
else
	mode = debug
	CXXFLG += -g3 -ggdb3 -pg -DDEBUG_ALL
	CFLAGS += -Wall -Wno-comments
	DEBREL = d
endif

SO_EXT     	= .so
SO_VERSION 	= 1

# replace the cuda dir in the command line
CUDA_DIR     = /usr/local/cuda
NVCC         = $(CUDA_DIR)/bin/nvcc

# -Xcompiler is necessary for -fPIC !!
NVCCFLAGS    = -Xcompiler -fPIC


CXXFLAGS       += -DBITSD='"$(BITS)$(DEBREL)"'
CFLAGS         += -fPIC -fno-strict-aliasing  -D_UNIX -D__PLACEMENT_NEW_INLINE 
CXXFLAGS       += $(CXXFLG) -std=c++11 $(CFLAGS) -fpermissive

LFLAGS		= -Wl,-rpath=lib/ -Wl,-rpath=AntTweakBar/lib/ \
                  -Wl,-rpath=visglut/lib -Wl,-rpath=libpng/ \
                  -Wl,-rpath=freetyp/lib 

AR       	= ar cqs
RANLIB   	=
TAR      	= tar -cf
GZIP     	= gzip -9f
DEL_FILE 	= rm -f
SYMLINK  	= ln -sf
DEL_DIR  	= rmdir
MOVE     	= mv
NO_STDERR	= 2> /dev/null

INCPATH        += -I../AntTweakBar/include -I../gl3w/include -I../glm \
                  -I../visglut/include -I../visglut/freeglut/include \
                  -I../libpng -I../datraw/ -I.. \
                  -I../freetype/include/ \
                  -I../include -I/usr/local/include -I/usr/X11R6/include -I/usr/include \
                  -I.          
          

BUILD_DIR       =  $(OUT_DIR)compiled/$(mode)/


# build object list from source files
C_OBJS          = $(addprefix $(BUILD_DIR), $(notdir $(patsubst %.c, %.o, $(C_SOURCES))))
CC_OBJS         = $(addprefix $(BUILD_DIR), $(notdir $(patsubst %.cc, %.o, $(CC_SOURCES))))
CPP_OBJS        = $(addprefix $(BUILD_DIR), $(notdir $(patsubst %.cpp, %.o, $(CPP_SOURCES))))
CU_OBJS         = $(addprefix $(BUILD_DIR), $(notdir $(patsubst %.cu, %.cu_o, $(CU_SOURCES))))

C_SRC_DIR       = $(dir $(C_SOURCES))
CC_SRC_DIR      = $(dir $(CC_SOURCES))
CPP_SRC_DIR     = $(dir $(CPP_SOURCES))
CU_SRC_DIR      = $(dir $(CU_SOURCES))

C_DEPS          = $(addprefix $(BUILD_DIR), $(notdir $(patsubst %.c, %.dep, $(C_SOURCES))))
CC_DEPS         = $(addprefix $(BUILD_DIR), $(notdir $(patsubst %.cc, %.dep, $(CC_SOURCES))))
CPP_DEPS        = $(addprefix $(BUILD_DIR), $(notdir $(patsubst %.cpp, %.dep, $(CPP_SOURCES))))
CU_DEPS         = $(addprefix $(BUILD_DIR), $(notdir $(patsubst %.cu, %.dep, $(CU_SOURCES))))

vpath %.cu  $(CU_SRC_DIR)
vpath %.cpp $(CPP_SRC_DIR)
vpath %.cc  $(CC_SRC_DIR)
vpath %.c   $(C_SRC_DIR)

.PHONY:	clean show

$(CPP_OBJS): $(BUILD_DIR)%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "===== Compile $< ====="
	$(Q)$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

$(CPP_DEPS): $(BUILD_DIR)%.dep: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(Q)$(CXX) $(INCPATH) -MM $(CXXFLAGS) -MT $(BUILD_DIR)$*.o  $< > $@
	

$(CC_DEPS): $(BUILD_DIR)%.dep: %.cc
	@mkdir -p $(BUILD_DIR)
	$(Q)$(CXX) $(INCPATH) -MM $(CFLAGS) -MT $(BUILD_DIR)$*.o  $< > $@		
	
$(CC_OBJS): $(BUILD_DIR)%.o: %.cc
	@mkdir -p $(BUILD_DIR)
	@echo "===== Compile $< ====="
	$(Q)$(CXX) -c $(CFLAGS) $(INCPATH) -o $@ $<


$(C_DEPS): $(BUILD_DIR)%.dep: %.c
	@mkdir -p $(BUILD_DIR)
	$(Q)$(CC) $(INCPATH) -MM $(CFLAGS) -MT $(BUILD_DIR)$*.o  $< > $@	
	
$(C_OBJS): $(BUILD_DIR)%.o: %.c
	@mkdir -p $(BUILD_DIR)
	@echo "===== Compile $< ====="
	$(Q)$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<


$(CU_OBJS): $(BUILD_DIR)%.cu_o: %.cu
	@mkdir -p $(BUILD_DIR)
	@echo "===== Compile $< ====="
	$(Q)$(NVCC) -c $(NVCCFLAGS) $(INCPATH) -o $@ $<

$(CU_DEPS): $(BUILD_DIR)%.dep: %.cu
	@mkdir -p $(BUILD_DIR)
	$(Q)$(NVCC) $(INCPATH) -MM $(NVCCFLAGS) -MT $(BUILD_DIR)$*.o  $< > $@
		


clean:
	@echo "===== Clean ====="
	$(Q)$(DEL_FILE) $(C_OBJS) $(CPP_OBJS) $(CC_OBJS) $(CU_OBJS) $(BUILD_DIR)*.o $(BUILD_DIR)*.dep 
	$(Q)$(DEL_FILE) *~ core *.core *.stackdump  $(TARGET) ./lib$(TARGET)$(BITS)$(DEBREL)$(SO_EXT) ../ogl4core$(BITS)$(DEBREL)


show:
	@echo $(BUILD_DIR)
	@echo $(C_OBJS)
	@echo $(C_SOURCES)
	@echo $(C_SRC_DIR)
	@echo $(C_DEPS)
	@echo $(CC_OBJS)
	@echo $(CC_SOURCES)
	@echo $(CC_SRC_DIR)
	@echo $(CC_DEPS)
	@echo $(CPP_OBJS)
	@echo $(CPP_SOURCES)
	@echo $(CPP_SRC_DIR)
	@echo $(CPP_DEPS)
	@echo $(CU_OBJS)
	@echo $(CU_SOURCES)
	@echo $(CU_SRC_DIR)
	@echo $(CU_DEPS)
	
-include $(CPP_DEPS) 
-include $(CC_DEPS)
-include $(C_DEPS)

