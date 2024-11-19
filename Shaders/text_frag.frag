#version 400 core
in vec2 tc;
out vec4 FragColor;

uniform sampler2D frameTexture;  // The single-channel texture for the glyph
uniform vec3 font_color;
void main() {
    // Sample the glyph texture (single-channel, grayscale)
    vec2 uv = tc;
    uv.y = 1.0 - tc.y;
    float alpha = texture(frameTexture, uv).r;
    
    // Set the output color, applying the sampled alpha value
    FragColor = vec4(vec3(font_color),alpha);
}