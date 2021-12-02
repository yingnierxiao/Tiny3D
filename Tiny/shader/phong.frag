#include "shader/util.glsl"

uniform BindlessSampler2D texBlds[MAX_TEX];
#ifdef BillPass
uniform vec3 uNormal;
#endif

in vec2 vTexcoord;
flat in ivec4 vTexid;
flat in vec3 vColor;
in vec3 vNormal;
in mat3 vTBN;

layout (location = 0) out vec4 FragTex;
layout (location = 1) out vec4 FragMat;
layout (location = 2) out vec4 FragRoughMetal;

void main() {
	vec4 textureColor = texture(texBlds[vTexid.x], vTexcoord);
	if(textureColor.a < 0.3) discard;
#ifndef BillPass
	vec3 normal = vTexid.y > 0 ? GetNormalFromMap(texBlds[vTexid.y], vTexcoord, vTBN) : vNormal;
	normal = normalize(normal) * 0.5 + 0.5;

	FragMat = vec4(vColor, 1.0);
	//FragRoughMetal.r = vTexid.z >= 0 ? texture(texBlds[vTexid.z], vTexcoord).g : DefaultRM.r;
	//FragRoughMetal.g = vTexid.w >= 0 ? texture(texBlds[vTexid.w], vTexcoord).r : DefaultRM.g;
	FragRoughMetal.rg = vTexid.z >= 0 ? texture(texBlds[vTexid.z], vTexcoord).gr : DefaultRM.rg;
#else
	vec3 normal = uNormal;

	FragMat = BoardMat;
	FragRoughMetal = BoardRM;
#endif
	FragTex = textureColor;
	FragRoughMetal.ba = normal.xy;
	FragMat.z = normal.z;
}