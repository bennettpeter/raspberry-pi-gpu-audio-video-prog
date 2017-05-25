DMX_INC = -I/opt/vc/include/ -I /opt/vc/include/interface/vmcs_host/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux
EGL_INC =
GLES_INC = -ICommon/
INCLUDES = $(DMX_INC) $(EGL_INC) $(GLES_INC)
CFLAGS = $(INCLUDES)
DMX_LIBS = -L/opt/vc/lib/ -lbcm_host -lvcos -lvchiq_arm -lpthread
EGL_LIBS = -L/opt/vc/lib/ -lEGL -lGLESv2
ESP_LIBS = -LCommon -lRPes2pg
LDLIBS = $(DMX_LIBS) $(EGL_LIBS) $(ESP_LIBS) -lm
SRC = Hello_Triangle Hello_TriangleColour Simple_Image Simple_Texture2D Rotate_Image Hello_Square

all: $(SRC)

$(SRC): Common/libRPes2pg.a

Common/libRPes2pg.a:
	make -C Common