#version 330

// gui/Fragment.frag

// In

in vec2 vUv;

// Out

out vec4 fragColor;

// Uniforms

uniform sampler2D guiTexture;

// Main

void main()
{
    fragColor = texture(guiTexture, vec2(vUv.x, 1.0 - vUv.y));
}
