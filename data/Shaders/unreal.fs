#version 400

/* ==============================================================================
        Stage Inputs
 ============================================================================== */
// Passes everything in world coordinates to the fragment shader
in FragData {
	vec3 position;
	vec3 normal; 
	vec2 texCoords;
} vsIn;

/* ==============================================================================
        Structures
 ============================================================================== */
struct Light {
    vec3   position;   // Light position / direction for dir lights
    float  auxA;
    vec3   emission;   // Non normalized emission (already multiplied by intensity)
    int    type;
    bool   state;      // On/off flag
}; // 64 Bytes

/* ==============================================================================
        Uniforms
 ============================================================================== */
uniform rendererBlock {
	float gamma;
	float exposure;

	// Uncharted tone map parameters (see common.fs)
	float A, B, C, D, E, J, W;
};

uniform cameraBlock {
   	mat4 ViewMatrix;
	mat4 ProjMatrix;
	mat4 ViewProjMatrix;
	vec3 ViewPos;
};

const int NUM_LIGHTS = 4;

uniform lightBlock {
	Light lights[NUM_LIGHTS];
};

// Material parameters
uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D metallicTex;
uniform sampler2D roughTex;
uniform float metallic;
uniform float roughness;
uniform vec3  spec;

// IBL precomputation
uniform samplerCube irradianceTex;
uniform samplerCube ggxTex;
uniform sampler2D   brdfTex;

/* ==============================================================================
        Imports
 ============================================================================== */
vec3 toLinearRGB(vec3 c, float gamma);
vec3 simpleToneMap(vec3 c, float exp);
vec3 unchartedTonemap(vec3 c, float exp);
vec3 unchartedTonemapParam(vec3 c, float exp, float A, float B, float C, float D, float E, float J, float W);
vec3 toInverseGamma(vec3 c, float gamma);
vec3 fresnelSchlickUnreal(float cosTheta, vec3 F0);

/* ==============================================================================
        Stage Outputs
 ============================================================================== */
out vec4 outColor;

vec3 perturbNormal(in sampler2D normalMap) {
	// Fetch normal from map and adjust to linear space
    vec3 normal = texture(normalMap, vsIn.texCoords).xyz * 2.0 - 1.0;

	// Calculate uv derivatives
    vec2 duvdx = dFdx(vsIn.texCoords);
    vec2 duvdy = dFdy(vsIn.texCoords);

	// Calculate position derivatives
	vec3 dpdx = dFdx(vsIn.position);
	vec3 dpdy = dFdy(vsIn.position);

	// Find tangent from derivates and build the TBN basis
    vec3 N = normalize(vsIn.normal);
    vec3 T = normalize(dpdx * duvdy.t - dpdy * duvdx.t);
    vec3 B = -normalize(cross(N, T));

    return normalize(mat3(T, B, N) * normal);
}

const float MAX_GGX_LOD = 4.0;

void main(void) {
    vec3 V = normalize(ViewPos - vsIn.position);
	vec3 N = perturbNormal(normalTex);
    vec3 R = reflect(-V, N); 

	float rough = texture(roughTex, vsIn.texCoords).r;
	float metal = texture(metallicTex, vsIn.texCoords).r;

	// Diffuse component
	vec3 kd         = toLinearRGB(texture(diffuseTex, vsIn.texCoords).rgb, gamma);
	vec3 irradiance = texture(irradianceTex, N).rgb;
    vec3 diffuse    = kd * irradiance; // Appendix, formula X

	// Specular component
	vec3 F0 = mix(spec, kd, metal);
	vec3 F  = fresnelSchlickUnreal(max(dot(N, V), 0.0), spec);
	
	// Fetch precomputed integrals
    vec3 prefGGX = textureLod(ggxTex, R, rough * MAX_GGX_LOD).rgb;  
    vec3 brdf    = texture(brdfTex, vec2(max(dot(N, V), 0.0), rough)).rgb;

	vec3 brdfInt  = F0 * brdf.r + brdf.g; // Appendix, formula Y
    vec3 specular = prefGGX * brdfInt;    // Appendix, formula Z

	// Total ambient lighting
	vec3 retColor = (1.0 - F) * (1.0 - metal) * diffuse + specular;

	// Tonemapping and gamma correction
	retColor = unchartedTonemapParam(retColor, exposure, A, B, C, D, E, J, W);
	retColor = toInverseGamma(retColor, gamma);

	outColor = vec4(retColor, 1.0);
}
