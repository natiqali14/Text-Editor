#version 400 core
in vec2 tc;
out vec4 FragColor;

uniform sampler2D frameTexture;  // The single-channel texture for the glyph

void main() {
    // Sample the glyph texture (single-channel, grayscale)
    
    // Set the output color, applying the sampled alpha value
    FragColor = vec4(1.0,1.0,1.0,1.0);
}