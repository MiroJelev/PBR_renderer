#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 texture_coordinates;
out vec3 world_position;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    texture_coordinates = aTexCoords;
    world_position = vec3(model * vec4(aPos, 1.0));
    normal = mat3(model) * aNormal;   

    gl_Position =  projection * view * vec4(world_position, 1.0);
}
