#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

uniform vec3 light_color;
uniform vec3 light_position;

uniform vec3 camera_position;

out vec3 object_normal;
out vec3 fragment_position;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    object_normal = mat3(transpose(inverse(model))) * aNormal;
    fragment_position = vec3(model * vec4(aPos, 1.0f));
}