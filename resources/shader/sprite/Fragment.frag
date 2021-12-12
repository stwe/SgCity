#version 330

out vec4 fragColor;

in vec2 vUv;

//niform sampler2D diffuseMap;

void main()
{
    //fragColor = texture(diffuseMap, vUv);
    fragColor = vec4(0.1, 0.8, 0.1, 1.0);
}
