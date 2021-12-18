#version 330

out vec4 fragColor;

in vec2 vUv;
flat in vec3 vColor;
in vec3 vNormalColor;
flat in float intensity;

uniform sampler2D diffuseMap;

void main()
{
    float ambientIntensity = 0.4;

    //vColor = max(intensity * baseColor, ambientIntensity * baseColor);

    fragColor = texture(diffuseMap, vUv);
    //fragColor = vec4(vColor, 1.0);
    //fragColor = vec4(vNormalColor, 1.0);
}
