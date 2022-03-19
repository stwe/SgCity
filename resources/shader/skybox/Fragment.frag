#version 330

in vec3 vUv;

out vec4 fragColor;

uniform samplerCube cubeSampler;

void main()
{
    fragColor = texture(cubeSampler, vUv);
}
