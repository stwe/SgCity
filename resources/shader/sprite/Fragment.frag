#version 330

out vec4 fragColor;

in vec2 vUv;
in vec3 vIdColor;

//niform sampler2D diffuseMap;
//uniform vec3 col;

void main()
{
    //fragColor = texture(diffuseMap, vUv);
    fragColor = vec4(vIdColor.x, vIdColor.y, vIdColor.z, 1.0);
}
