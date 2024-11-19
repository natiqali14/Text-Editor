#version 400 core
in vec2 tc;
out vec4 FragColor;
uniform float hide;
uniform vec3 color;
void main() {
    FragColor = vec4(vec3(color),hide);
}