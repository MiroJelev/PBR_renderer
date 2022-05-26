
OBJS = main.cpp shader.cpp texture.cpp vertex_buffer.cpp vertex_array.cpp element_buffer.cpp camera.cpp mesh.cpp model.cpp image_loading.cpp framebuffer.cpp render_buffer.cpp

CC = g++


LINKER_FLAGS = -lsfml-window -lsfml-system -lGL -lGLEW -lassimp
#	-lGLU
# -lsfml-audio

EX_NAME = start

all:$(OBJS)
	$(CC) $(OBJS) $(LINKER_FLAGS) -o $(EX_NAME)

clean:
	rm $(EX_NAME)
