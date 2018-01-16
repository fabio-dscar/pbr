#ifndef __PBR_RENDERER_H__
#define __PBR_RENDERER_H__

#include <PBR.h>

namespace pbr {

    class Scene;
    class Camera;

    static PBR_CONSTEXPR uint32 NUM_LIGHTS = 4;
    
    enum ToneOperator {
        SIMPLE,
        REINHART,
        UNCHARTED
    };

    enum BufferIndices : uint32 {
        CAMERA_BUFFER_IDX   = 0,
        LIGHTS_BUFFER_IDX   = 1,
        RENDERER_BUFFER_IDX = 2
    };

    // Buffer for shaders with renderer information
    struct RendererBuffer {
        float gamma;
        float exp;

        // Uncharted tone curve control parameters
        float A, B, C, D, E, F, W;
    };
    
    class PBR_SHARED Renderer {
    public:
        Renderer();

        void prepare();
        void render(const Scene& scene, const Camera& camera);

        float gamma() const;
        void setGamma(float gamma);

        float exposure() const;
        void setExposure(float exp);

        const float* toneParams() const;
        void setToneParams(float toneParams[7]);

        void setSkyboxDraw(bool state);

    private:
        void uploadRendererBuffer();
        void uploadLightsBuffer(const Scene& scene);
        void uploadCameraBuffer(const Camera& camera);
        void drawShapes(const Scene& scene);
        void drawSkybox(const Scene& scene);

        float _gamma;
        float _exposure;
        float _toneParams[7];
        ToneOperator _tone;

        bool _drawSkybox;
        
        RRID _lightsBuffer;
        RRID _cameraBuffer;
        RRID _rendererBuffer;
    };

}

#endif
