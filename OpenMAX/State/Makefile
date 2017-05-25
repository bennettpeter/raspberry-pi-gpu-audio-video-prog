DMX_INC = -I/opt/vc/include/ -I /opt/vc/include/interface/vmcs_host/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
EGL_INC =
OMX_INC = -I /opt/vc/include/IL
INCLUDES = $(DMX_INC) $(EGL_INC) $(OMX_INC)
CFLAGS=-g -DRASPBERRY_PI -DOMX_SKIP64BIT $(INCLUDES)
CPPFLAGS =
DMX_LIBS = -L/opt/vc/lib/ -lbcm_host -lvcos -lvchiq_arm -lpthread
EGL_LIBS = -L/opt/vc/lib/ -lEGL -lGLESv2
OMX_LIBS = -lopenmaxil
LDLIBS = $(DMX_LIBS) $(EGL_LIBS) $(OMX_LIBS)
EXE = wontwork working event

all: $(EXE)