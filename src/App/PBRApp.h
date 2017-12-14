#ifndef __PBR_PBRAPP_H__
#define __PBR_PBRAPP_H__

#include <OpenGLApplication.h>

#include <Scene.h>
#include <Renderer.h>

namespace pbr {

    class PBRApp : public OpenGLApplication {
    public:
        PBRApp(const std::string& title, int width, int height);

        void prepare()   override;
        void drawScene() override;
        void update(float dt) override;
        void cleanup()   override;

        void processKeyPress(unsigned char key, int x, int y) override;
        void processMouseClick(int button, int state, int x, int y) override;
    private:
        Scene    _scene;
        Renderer _renderer;

        sref<Camera> _camera;
    };

}

#endif