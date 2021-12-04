#version 330

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aUv;

out vec2 vUv;

uniform mat4 model;
//niform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(aPosition, 0.0, 1.0);
    vUv = aUv;
}
