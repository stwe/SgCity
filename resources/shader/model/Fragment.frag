#version 330

out vec4 fragColor;

in vec2 vUv;

uniform vec3 diffuseColor;

void main()
{
    //fragColor = texture(diffuseMap, vUv);
    fragColor = vec4(diffuseColor, 1.0);
}
