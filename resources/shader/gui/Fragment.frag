#version 330

in vec2 vUv;

out vec4 fragColor;

uniform sampler2D guiTexture;

void main()
{
    fragColor = texture(guiTexture, vec2(vUv.x, 1.0 - vUv.y));
}
