#ifndef __PBR_PBRMATERIAL_H__
#define __PBR_PBRMATERIAL_H__

#include <Material.h>
#include <PBRMath.h>
#include <Spectrum.h>
#include <RenderInterface.h>
#include <Resources.h>

#include <Texture.h>

using namespace pbr::math;

namespace pbr {

    class PBR_SHARED PBRMaterial : public Material {
    public:
        PBRMaterial();

        void update(const Skybox& skybox);
        void uploadData() const;

        void setDiffuse(RRID diffTex);
        void setNormal(RRID normalTex);
        void setSpecular(const Vec3& spec);

        void setMetallic(RRID metalTex);
        void setMetallic(float metallic);

        void setRoughness(RRID roughTex);
        void setRoughness(float roughness);

        void setIrradianceTex(RRID id);
        void setBrdfTex(RRID id);
        void setGGXTex(RRID id);

    private:
        RGBSpectrum _diffuse;
        float _metallic;
        float _roughness;
        Vec3  _f0;

        RRID _diffuseTex;
        RRID _normalTex;
        RRID _metallicTex;
        RRID _roughTex;

        // PBR maps
        RRID _irradianceTex;
        RRID _brdfTex;
        RRID _ggxTex;
    };

}


#endif