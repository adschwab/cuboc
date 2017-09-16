CC=g++
GLFW=-lglfw
GL=-lGLEW -framework OpenGL
CFLAGS=-I.
OBJ=obj

CXX_SRCS=$(shell find . ! -name "test_*.cc" -name "*.cc")
CXX_OBJS=$(addprefix $(OBJ)/, ${CXX_SRCS:.cc=.o})

.PHONY: clean pre make

make: pre engine

pre:
	@mkdir -p $(OBJ)
	@mkdir -p $(OBJ)/loader
	@mkdir -p $(OBJ)/util

engine: $(CXX_OBJS)
	$(CC) -o $@ $^ $(GLFW) $(GL)

$(OBJ)/%.o: %.cc
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@if [ -d $(OBJ) ]; then \
	    rm -r $(OBJ); \
	 fi
	@if [ -e engine ]; then \
	    rm engine; \
	 fi

