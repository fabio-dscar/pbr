#version 400 core

// Passes everything in world coordinates to the fragment shader
in FragData {
	vec3 position;
	vec3 normal; 
	vec2 texCoords;
} vsIn;

uniform cameraBlock {
   	mat4 ViewMatrix;
	mat4 ProjMatrix;
	mat4 ViewProjMatrix;
	vec3 ViewPos;
};

out vec4 outColor;

in vec3 lightVec;
in vec3 halfVec;

uniform sampler2D diffTex;
uniform sampler2D bumpMap;

void main(void) {
	vec3 normal = 2.0 * texture(bumpMap, vsIn.texCoords * 8).rgb - 1.0;
	normal = normalize(normal);

	vec3 N = normal;
	vec3 L = normalize(lightVec);
	//vec3 V = normalize(-vsIn.position);

	vec3 H = normalize(halfVec);

	float NdotL = dot(N, L);
	float NdotH = dot(N, H);

	vec3 diff = vec3(0, 0, 0);
	vec3 spec = vec3(0, 0, 0);
	if (NdotL > 0) {
		diff = texture(diffTex, vsIn.texCoords * 8).rgb * NdotL;

		if (NdotH > 0)
			spec = vec3(0.85, 0.85, 0.85) * pow(NdotH, 40.0);
	}

	outColor = vec4(diff + spec, 1.0);

	//outColor = vec4(0.5, 0.5, 0.1, 1.0);
}