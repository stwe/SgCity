#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aIdColor;
layout (location = 3) in vec3 aNormal;

out vec2 vUv;
flat out vec3 vColor;
out vec3 vNormalColor;
flat out float intensity;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    vec3 n = normalize(normalMatrix * aNormal);

    float ambientIntensity = 0.4;
    vec3 lightDirection = vec3(0.0, 1.0, 0.0);
    vec3 baseColor = vec3(0.0, 0.8, 0.0);
    float intensity = max(dot(n, lightDirection), 0.0);

    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    vUv = aUv;

    vColor = max(intensity * baseColor, ambientIntensity * baseColor);
    vNormalColor = aNormal;
}
