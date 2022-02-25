#version 330

out vec4 fragColor;

in vec2 vUv;

uniform sampler2D diffuseMap;

void main()
{
    //fragColor = texture(diffuseMap, vUv);
    fragColor = vec4(0.6, 0.1, 0.1, 1.0);
}
