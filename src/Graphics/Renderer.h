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
        CAMERA_BUFFER_IDX = 0,
        LIGHTS_BUFFER_IDX = 1
    };

    // Buffer for shaders with renderer information
    struct RendererBuffer {
        float gamma;
    };
    
    class PBR_SHARED Renderer {
    public:
        Renderer() { }

        void prepare();
        void render(const Scene& scene, const Camera& camera);

        float gamma() const;
        void setGamma(float gamma);
    private:
        void uploadLightsBuffer(const Scene& scene);
        void uploadCameraBuffer(const Camera& camera);
        void drawShapes(const Scene& scene);

        float _gamma;
        ToneOperator _tone;

        RRID _lightsBuffer;
        RRID _cameraBuffer;
    };

}

#endif
