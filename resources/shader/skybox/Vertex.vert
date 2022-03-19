#version 330

layout (location = 0) in vec3 aPosition;

out vec3 vUv;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    vUv = aPosition;
    vec4 position = projectionMatrix * viewMatrix * vec4(aPosition, 1.0);
    gl_Position = position.xyww;
}
