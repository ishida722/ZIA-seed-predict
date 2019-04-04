include ../../../../env.mk

INC = -I../include

# source files.
SRC =  	dmp_network.cpp util_draw.cpp util_input.cpp nanojpeg.cpp v4l2-cam.cpp usb-mouse.cpp agg_font_freetype.cpp demo_common.cpp

OBJ = $(SRC:.cpp=.o)

OUT = ../lib/libdv700_util.a

# include directories
INCLUDES = -I. -I../include/ -I../../../dv-user-driver/include -I/usr/include/agg2 `freetype-config --cflags`

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS =  -pthread -std=c++11 $(OPT) -Wall -Werror

# compiler
CCC = $(GPP)

# library paths
LIBS = -L../ -L/usr/local/lib -lm

# compile flags
LDFLAGS = $(OPT)

.SUFFIXES: .cpp

all: $(OUT)

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT)
