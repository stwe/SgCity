#version 330

out vec4 fragColor;

in vec2 vUv;

uniform float hasDiffuseMap;
uniform sampler2D diffuseMap;
uniform vec3 diffuseColor;

void main()
{
    fragColor = vec4(diffuseColor, 1.0);
    if (hasDiffuseMap > 0.5)
    {
        fragColor = texture(diffuseMap, vUv);

        // discard if transparent
        if (fragColor.a < 0.5)
        {
            discard;
        }
    }
}
