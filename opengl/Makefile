OS := $(shell uname)
ifeq ($(OS),Darwin)
	#LDLIBS = -lGLEW -lglfw -std=c++14 -framework OpenGL
	LDLIBS = -lGLEW -lglfw -lassimp -std=c++14 -framework OpenGL
else
	LDLIBS = -lGLEW -lglfw -lassimp -lGl -lX11 -lpthread -lXi
endif

CC = g++

PROG = prog

end:
	$(CC) *.cpp $(LDLIBS) -o $(PROG)

clean:
	rm *.o $(PROG)
