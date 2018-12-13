#version 330

uniform samplerCube textureEnv;
uniform sampler2D reflectBuffer;
uniform vec3 light;
uniform vec2 waterBias;

in vec3 vNormal;
in vec3 vViewNormal;
in vec3 vEye2Water;
in vec3 vWater;
in vec4 vProjPos;

layout (location = 0) out vec4 FragTex;
layout (location = 1) out vec4 FragColor;
layout (location = 2) out vec4 FragNormal;

void main() {
	vec3 normal = normalize(vNormal);
	vec3 eye2Water = normalize(vEye2Water);
	
	vec2 bias = normalize(vViewNormal).xz * waterBias;
	vec2 refCoord = (vProjPos.xy/vProjPos.w) * 0.5 + 0.5;
	vec4 reflectTex = texture2D(reflectBuffer, refCoord + bias);
	vec3 reflectedColor = reflectTex.rgb * texture(textureEnv, reflect(eye2Water, normal)).rgb;
	vec3 refractedColor = texture(textureEnv, refract(eye2Water, normal, 0.750395)).rgb;
	
	vec3 lightDirectionReflected = reflect(normalize(vWater - light), normal);
	float specular = pow(max(-dot(eye2Water, lightDirectionReflected), 0.0), 40.0);
	
	float ndote = -dot(normal, eye2Water);
	float fresnel = clamp(1.0 - ndote, 0.0, 1.0);
    fresnel = pow(fresnel, 3.0) * 0.65;

	vec3 waterColor = mix(reflectedColor, refractedColor, fresnel); 
	//+ vec3(specular);

	vec4 surfaceColor = vec4(waterColor, 0.5);
	vec3 color = vec3(1.0, 0.0, 0.0);
		
	FragTex = surfaceColor;
	//FragTex = reflectTex;
	FragColor = vec4(color, 0.1);
	FragNormal = vec4(normal * 0.5 + 0.5, 1.0);
}