CC = g++
CFLAGS = -Wall
PROG = cg

SRCS = cg.cpp imageloader.cpp md2model.cpp vec3f.cpp

ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT
else
	LIBS = -ggdb -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm `pkg-config opencv libpng --cflags --libs` texture.o dcaSeams.o
endif

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
