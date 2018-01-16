#ifndef __PBR_PBRAPP_H__
#define __PBR_PBRAPP_H__

#include <OpenGLApplication.h>

#include <Scene.h>
#include <Renderer.h>
#include <Skybox.h>

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
        void drawInterface();
        void restoreToneDefaults();
        void changeSkybox(int id);

        Scene    _scene;
        Renderer _renderer;

        sref<Camera> _camera;

        float _rotAngleX;
        float _rotAngleY;

        float _gamma;
        float _exposure;
        float _toneParams[7];

        bool _showGUI;
        bool _skyToggle;

        Shape* _selectedShape;

        int _skybox;
        vec<Skybox> _skyboxes;
    };

}

#endif