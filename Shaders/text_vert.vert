#version 400 core
layout(location = 0) in vec2 vertex;
layout (location = 1) in vec2 texCoord; 
out vec2 tc;

uniform mat4 projection;
uniform mat4 model;
void main() {
    gl_Position =  model * vec4(vertex, 0.0, 1.0);
    tc = texCoord;
}