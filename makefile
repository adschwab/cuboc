CC=g++
GLFW=-lglfw
GL=-lGLEW -framework OpenGL
CFLAGS=-std=c++11 -I. -D_UNIX
OBJ=obj

CXX_SRCS=$(shell find . ! -name "test_*.cc" -name "*.cc")
CXX_OBJS=$(addprefix $(OBJ)/, ${CXX_SRCS:.cc=.o})
TEST_SRCS=$(shell find . -name "test_*.cc")
TEST_OBJS=$(addprefix $(OBJ)/, ${TEST_SRCS:.cc=.o})
TEST_BINS=$(addprefix $(OBJ)/, ${TEST_SRCS:.cc=.bin})
LIB_OBJS := $(filter-out $(OBJ)/./engine.o, $(CXX_OBJS))

.PHONY: clean pre make

make: pre engine

test: pre $(TEST_BINS)

pre:
	@mkdir -p $(OBJ)
	@mkdir -p $(OBJ)/loader
	@mkdir -p $(OBJ)/util
	@mkdir -p $(OBJ)/objects
	@mkdir -p $(OBJ)/gen

engine: $(CXX_OBJS)
	$(CC) -o $@ $^ $(GLFW) $(GL)

$(OBJ)/%.bin: $(OBJ)/%.o | $(LIB_OBJS)
	$(CC) -o $@ $^ $(LIB_OBJS) $(GLFW) $(GL)

$(OBJ)/%.o: %.cc
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@if [ -d $(OBJ) ]; then \
	    rm -r $(OBJ); \
	 fi
	@if [ -e engine ]; then \
	    rm engine; \
	 fi

