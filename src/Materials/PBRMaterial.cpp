#include <PBRMaterial.h>

using namespace pbr;

PBRMaterial::PBRMaterial() : _metallic(1.0f), _roughness(0.0f), _f0(0.04f) {
    _prog = Resource.getShader("unreal")->id();

    _brdfTex = Resource.getTexture("brdf")->rrid();

    _metallicTex = -1;
    _roughTex    = -1;
}

void PBRMaterial::update(const Skybox& skybox) {
    _irradianceTex = skybox.irradianceTex();
    _ggxTex = skybox.ggxTex();
}

void PBRMaterial::uploadData() const {
    //RHI.useProgram(_prog);
    
    RHI.setFloat("metallic", _metallic);
    RHI.setFloat("roughness", _roughness);
    RHI.setVector3("spec", Vec3(_f0.r, _f0.g, _f0.b));

    // Set diffuse texture
    RHI.bindTexture(1, _diffuseTex);
    RHI.setSampler("diffuseTex", 1);

    RHI.bindTexture(2, _normalTex);
    RHI.setSampler("normalTex", 2);

    if (_metallicTex != -1) {
        RHI.bindTexture(3, _metallicTex);
        RHI.setSampler("metallicTex", 3);
    }

    if (_roughTex != -1) {
        RHI.bindTexture(4, _roughTex);
        RHI.setSampler("roughTex", 4);
    }

    RHI.bindTexture(6, _irradianceTex);
    RHI.setSampler("irradianceTex", 6);

    RHI.bindTexture(7, _ggxTex);
    RHI.setSampler("ggxTex", 7);
    
    RHI.bindTexture(8, _brdfTex);
    RHI.setSampler("brdfTex", 8);

    //RHI.useProgram(0);
}

void PBRMaterial::setIrradianceTex(RRID id) {
    _irradianceTex = id;
}

void PBRMaterial::setBrdfTex(RRID id) {
    _brdfTex = id;
}

void PBRMaterial::setGGXTex(RRID id) {
    _ggxTex = id;
}

void PBRMaterial::setDiffuse(RRID diffTex) {
    _diffuseTex = diffTex;
}

void PBRMaterial::setNormal(RRID normalTex) {
    _normalTex = normalTex;
}

void PBRMaterial::setSpecular(const Color& spec) {
    _f0 = spec;
}

void PBRMaterial::setMetallic(RRID metalTex) {
    _metallicTex = metalTex;
}

void PBRMaterial::setMetallic(float metallic) {
    _metallic = metallic;
}

void PBRMaterial::setRoughness(RRID roughTex) {
    _roughTex = roughTex;
}

void PBRMaterial::setRoughness(float roughness) {
    _roughness = roughness;
}

float PBRMaterial::metallic() const {
    return _metallic;
}

float PBRMaterial::roughness() const {
    return _roughness;
}

Color PBRMaterial::specular() const {
    return _f0;
}

Color PBRMaterial::diffuse() const {
    return _diffuse;
}

RRID PBRMaterial::diffuseTex() const {
    return _diffuseTex;
}

RRID PBRMaterial::metallicTex() const {
    return _metallicTex;
}

RRID PBRMaterial::roughTex() const {
    return _roughTex;
}