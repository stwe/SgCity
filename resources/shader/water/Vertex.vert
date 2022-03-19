#version 330

layout (location = 0) in vec2 aPosition;

out vec4 vClipSpace;
out vec2 vUv;
out vec3 vToCameraVector;
out vec3 vFromLightVector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

const float tiling = 4.0;

void main()
{
    vec4 worldPosition = model * vec4(aPosition.x, 0.0, aPosition.y, 1.0);
    vClipSpace = projection * view * worldPosition;
    gl_Position = vClipSpace;
    vUv = vec2(aPosition.x / 2.0 + 0.5, aPosition.y / 2.0 + 0.5) * tiling;
    vToCameraVector = cameraPosition - worldPosition.xyz;
    vFromLightVector = worldPosition.xyz - lightPosition;
}
