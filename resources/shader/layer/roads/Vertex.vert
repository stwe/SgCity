#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aIdColor;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in float aTextureNr;
layout (location = 5) in float aSelected;

out vec2 vUv;
flat out float vSelected;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0);

    vUv = aUv;
    vSelected = aSelected;
}
