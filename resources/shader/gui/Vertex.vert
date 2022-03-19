#version 330

layout (location = 0) in vec2 aPosition;

out vec2 vUv;

uniform mat4 transformationMatrix;

void main()
{
    gl_Position = transformationMatrix * vec4(aPosition, 0.0, 1.0);
    vUv = vec2((aPosition.x + 1.0) / 2.0, 1.0 - (aPosition.y + 1.0) / 2.0);
}
