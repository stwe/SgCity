#version 330

out vec4 fragColor;

in vec2 vUv;
flat in vec3 vColor;
in vec3 vNormalColor;
flat in float vIntensity;

uniform sampler2D diffuseMap;

void main()
{
    vec4 col = texture(diffuseMap, vUv);
    float ambientIntensity = 0.4;
    fragColor = max(vIntensity * col, ambientIntensity * col);

    //fragColor = texture(diffuseMap, vUv);
    //fragColor = vec4(vColor, 1.0);
    //fragColor = vec4(vNormalColor, 1.0);
}
