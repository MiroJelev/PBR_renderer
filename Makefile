
OBJS = new_main.cpp shader.cpp texture.cpp vertex_buffer.cpp vertex_array.cpp element_buffer.cpp camera.cpp mesh.cpp model.cpp image_loading.cpp framebuffer.cpp render_buffer.cpp
IMGUI_OBJS = ImGui/imgui_widgets.cpp  ImGui/imgui_tables.cpp  ImGui/imgui-SFML.cpp  ImGui/imgui_impl_opengl3.cpp  ImGui/imgui_draw.cpp  ImGui/imgui_demo.cpp ImGui/imgui.cpp

CC = g++


LINKER_FLAGS = -lsfml-window -lsfml-system -lGL -lGLEW -lassimp -L/usr/local/lib/ -lBulletDynamics -lBulletCollision -lLinearMath -Ofast
#	-lGLU
# -lsfml-audio

EX_NAME = start

all:$(OBJS) $(IMGUI_OBJS)
	$(CC) $(OBJS) $(IMGUI_OBJS) $(LINKER_FLAGS) -o $(EX_NAME)

clean:
	rm $(EX_NAME)
