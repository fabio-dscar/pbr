#version 400 core

layout(location = 0) in vec3 Position;	
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in vec3 Tangent;

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

out vec3 lightVec;
out vec3 halfVec;

uniform cameraBlock {
	mat4 ViewMatrix;
    mat4 ProjMatrix;
	mat4 ViewProjMatrix;
	vec3 ViewPos;
};

// Passes everything in world coordinates to the fragment shader
out FragData {
	vec3 position;
	vec3 normal; 
	vec2 texCoords;
} vsOut;

void main(void) {
	mat4 modelView = ViewMatrix * ModelMatrix;

	vec3 lightPos = vec3(modelView * normalize(vec4(0, 1, 1, 0)));

	mat3 normalMat = transpose(inverse(mat3(modelView)));

	vec3 n = normalize(normalMat * Normal);
	vec3 t = normalize(normalMat * Tangent);
	vec3 b = cross(n, t);

	vec3 pos = vec3(modelView * vec4(Position, 1.0));
	vec3 lightDir = normalize(lightPos);

	vec3 v;	
	v.x	= dot(lightDir, t);	
	v.y	= dot(lightDir, b);	
	v.z	= dot(lightDir, n);	
	lightVec = normalize(v);

	vec3 halfVector = normalize(lightDir - pos);
	v.x = dot(halfVector, t);
	v.y = dot(halfVector, b);
	v.z = dot(halfVector, n);
	halfVec = normalize(v);

	// Everything in view coordinates
	vsOut.position  = pos;
	vsOut.normal    = normalize(normalMat * Normal);
	vsOut.texCoords = TexCoords;

	// Return position in MVP coordinates
	gl_Position = ViewProjMatrix * ModelMatrix * vec4(Position, 1.0);
	//gl_Position = ViewProjMatrix * vec4(vsOut.position, 1.0);
}