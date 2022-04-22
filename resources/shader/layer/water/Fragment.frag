#version 330

in vec4 vClipSpace;
in vec2 vUv;
in vec3 vToCameraVector;
in vec3 vFromLightVector;

out vec4 fragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform float moveFactor;
uniform vec3 lightColor;

const float waveStrength = 0.08;
const float shineDamper = 10.0;
const float reflectivity = 0.1;

void main()
{
    vec2 ndc = (vClipSpace.xy / vClipSpace.w) / 2.0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

    float near = 0.01;
    float far = 5000.0;
    float depth = texture(depthTexture, refractTexCoords).r;
    float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    float waterDepth = floorDistance - waterDistance;

    vec2 distortedTexCoords = texture(dudvTexture, vec2(vUv.x + moveFactor, vUv.y)).rg * 0.1;
    distortedTexCoords = vUv + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
    vec2 totalDistortion = (texture(dudvTexture, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth, 0.0, 1.0);

    refractTexCoords += totalDistortion;
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

    reflectTexCoords += totalDistortion;
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

    vec4 reflectionCol = texture(reflectionTexture, reflectTexCoords);
    vec4 refractionCol = texture(refractionTexture, refractTexCoords);

    vec4 normalCol = texture(normalTexture, distortedTexCoords);
    vec3 normal = vec3(normalCol.r * 2.0 - 1.0, normalCol.b * 3.0, normalCol.g * 2.0 - 1.0);
    normal = normalize(normal);

    vec3 viewVector = normalize(vToCameraVector);
    float refractiveFactor = dot(viewVector, normal);
    refractiveFactor = pow(refractiveFactor, 0.5);

    vec3 reflectedLight = reflect(normalize(vFromLightVector), normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = lightColor * specular * reflectivity * clamp(waterDepth, 0.0, 1.0);

    fragColor = mix(reflectionCol, refractionCol, refractiveFactor);
    fragColor = mix(fragColor, vec4(0.0, 0.3, 0.8, 1.0), 0.6) + vec4(specularHighlights, 0.0);
    fragColor.a = clamp(waterDepth, 0.0, 1.0);
}
