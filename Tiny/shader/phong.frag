#version 450
#extension GL_ARB_bindless_texture : enable 

layout(bindless_sampler) uniform sampler2D texBlds[256];

in vec2 vTexcoord;
flat in vec4 vTexid;
flat in vec3 vColor;
in vec3 vNormal;
in mat3 vTBN;

layout (location = 0) out vec4 FragTex;
layout (location = 1) out vec4 FragMat;
layout (location = 2) out vec4 FragNormalGrass;
layout (location = 3) out vec4 FragRoughMetal;

void main() {
	vec4 textureColor = texture2D(texBlds[int(vTexid.x)], vTexcoord);
	if(textureColor.a < 0.25) discard;

	vec3 normal = vNormal;
	if(vTexid.y >= 0.0) {
		vec3 texNorm = texture2D(texBlds[int(vTexid.y)], vTexcoord).rgb;
		texNorm = 2.0 * normalize(texNorm) - vec3(1.0);
		normal = vTBN * texNorm;
	}

	FragTex = textureColor;
	FragMat = vec4(vColor, 1.0);
	FragNormalGrass = vec4(normalize(normal) * 0.5 + 0.5, 0.0);

	FragRoughMetal = vec4(0.0, 0.0, 0.0, 1.0);
	if(vTexid.z >= 0.0) 
		FragRoughMetal.r = texture2D(texBlds[int(vTexid.z)], vTexcoord).r;
	if(vTexid.w >= 0.0) 
		FragRoughMetal.g = texture2D(texBlds[int(vTexid.w)], vTexcoord).r;
}