#version 330

out vec4 fragColor;

in vec2 vUv;
flat in vec3 vColor;
in vec3 vNormalColor;
flat in float vTextureNr;
flat in float vIntensity;

uniform sampler2D diffuseMap;
uniform sampler2D rMap;
uniform sampler2D cMap;
uniform sampler2D iMap;
uniform sampler2D trafficMap;

vec4 col;

void main()
{
    col = texture(diffuseMap, vUv);

    if (vTextureNr == 0)
        col = texture(diffuseMap, vUv);
    else if (vTextureNr == 1)
        col = texture(rMap, vUv);
    else if (vTextureNr == 2)
        col = texture(cMap, vUv);
    else if (vTextureNr == 3)
        col = texture(iMap, vUv);
    else if (vTextureNr == 4)
        col = texture(trafficMap, vUv);

    float ambientIntensity = 0.4;
    fragColor = max(vIntensity * col, ambientIntensity * col);

    //fragColor = texture(diffuseMap, vUv);
    //fragColor = vec4(vColor, 1.0);
    //fragColor = vec4(vNormalColor, 1.0);
}
