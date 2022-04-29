#version 330

out vec4 fragColor;

in vec2 vUv;
flat in float vSelected;

uniform sampler2D diffuseMap;

void main()
{
    fragColor = texture(diffuseMap, vUv);

    if (vSelected > 0.5)
    {
        fragColor = fragColor / 2.0;
    }
}
