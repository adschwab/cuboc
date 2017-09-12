CC=g++
GLFW=-lglfw
GL=-lGLEW -framework OpenGL
CFLAGS=-I.
OBJ=obj

.PHONY: clean pre make

make: pre engine

pre:
	@mkdir -p $(OBJ)

engine: $(OBJ)/engine.o $(OBJ)/window.o $(OBJ)/camera.o  $(OBJ)/loadfile.o $(OBJ)/programloader.o $(OBJ)/image_loader.o  $(OBJ)/buffer.o
	$(CC) -o $@ $^ $(GLFW) $(GL)

$(OBJ)/loadfile.o: util/loadfile.cc
	$(CC) -o $@ -c $^ $(CFLAGS)

$(OBJ)/programloader.o: loader/programloader.cc
	$(CC) -o $@ -c $^ $(CFLAGS)

$(OBJ)/image_loader.o: loader/image_loader.cc
	$(CC) -o $@ -c $^ $(CFLAGS)

$(OBJ)/buffer.o: loader/buffer.cc
	$(CC) -o $@ -c $^ $(CFLAGS)

$(OBJ)/camera.o: camera.cc
	$(CC) -o $@ -c $^ $(CFLAGS)

$(OBJ)/window.o: window.cc
	$(CC) -o $@ -c $^ $(CFLAGS)

$(OBJ)/engine.o: engine.cc
	$(CC) -o $@ -c $^ $(CFLAGS)

clean:
	@rm engine
	@rm -r $(OBJ)
