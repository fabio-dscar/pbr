#version 400 core

// Passes everything in world coordinates to the fragment shader
in FragData {
	vec3 position;
	vec3 normal; 
	vec2 texCoords;
} vsIn;

uniform cameraBlock {
	mat4 ViewProjMatrix;
	vec3 ViewPos;
};

out vec4 outColor;

void main(void) {
	vec3 N = normalize(vsIn.normal);
	vec3 L = normalize(vec3(0, 1, 1));
	vec3 V = normalize(ViewPos - vsIn.position);

	vec3 H = normalize(L + V);

	float NdotL = dot(N, L);
	float NdotH = dot(N, H);

	vec3 diff = vec3(0, 0, 0);
	vec3 spec = vec3(0, 0, 0);
	if (NdotL > 0) {
		diff = vec3(0.85, 0.15, 0.15) * NdotL;
		spec = vec3(1, 1, 1) * pow(NdotH, 15.0);
	}

	outColor = vec4(diff + spec, 1.0);

	//outColor = vec4(0.5, 0.5, 0.1, 1.0);
}