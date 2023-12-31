
OUT_DIR         = ./
BASE_DIR        = ../../..

INCPATH  += -I/usr/local/include -I/usr/X11R6/include -I/usr/include -I../include\
            -I$(BASE_DIR)/OGL4Core \
            -I$(BASE_DIR)/OGL4CoreAPI \
            -I$(BASE_DIR)/AntTweakBar/include \
            -I$(BASE_DIR)/gl3w/include \
            -I$(BASE_DIR)/glm

LIBS 	 	+= -L$(BASE_DIR)/lib 

C_SOURCES	+= $(BASE_DIR)/gl3w/src/gl3w.c


#first:	depend all
first:	all

include $(BASE_DIR)/common.mk
all: 	Makefile $(TARGET)

$(TARGET): $(CPP_OBJS) $(CC_OBJS) $(C_OBJS)
	@echo "===== Link $@ ====="
	$(Q)$(LINK) $(LFLAGS) -shared -o $(OUT_DIR)/lib$(TARGET)$(BITS)$(DEBREL)$(SO_EXT) $(CPP_OBJS) $(CC_OBJS) $(C_OBJS) $(LIBS)
