CC=g++
GLFW=-lglfw
GL=-lGLEW -framework OpenGL
OBJ=obj
CPPFLAGS=-std=c++11 -I./$(OBJ) -DPLATFORM=_UNIX


CXX_SRC_FILES=$(shell find src ! -name "test_*.cc" -name "*.cc")
CXX_HEADER_FILES=$(shell find src -name "*.h")
CXX_SRCS=$(patsubst src/%, %, $(CXX_SRC_FILES))
CXX_HEADERS=$(patsubst src/%, $(OBJ)/%, $(CXX_HEADER_FILES))
CXX_OBJS=$(addprefix $(OBJ)/, ${CXX_SRCS:.cc=.o})
TEST_SRCS=$(shell find . -name "test_*.cc")
TEST_OBJS=$(addprefix $(OBJ)/, ${TEST_SRCS:.cc=.o})
TEST_BINS=$(addprefix $(OBJ)/, ${TEST_SRCS:.cc=.bin})
LIB_OBJS := $(filter-out $(OBJ)/./engine.o, $(CXX_OBJS))

.PHONY: clean pre make

make: pre engine

test: pre $(TEST_BINS)

pre:
	@cp -r src $(OBJ)

engine: $(CXX_OBJS)
	$(CC) -o $@ $^ $(GLFW) $(GL)

$(OBJ)/%.bin: $(OBJ)/%.o | $(LIB_OBJS)
	$(CC) -o $@ $^ $(CPPFLAGS) $(LIB_OBJS) $(GLFW) $(GL)

$(OBJ)/%.o: src/%.cc
	$(CC) -o $@ -c $< $(CPPFLAGS)

clean:
	@if [ -d $(OBJ) ]; then \
	    rm -r $(OBJ); \
	 fi
	@if [ -e engine ]; then \
	    rm engine; \
	 fi

