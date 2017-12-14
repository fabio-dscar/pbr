#include <PBRApp.h>

#include <Resources.h>
#include <RenderInterface.h>

#include <Shape.h>
#include <Sphere.h>
#include <Mesh.h>

#include <Camera.h>
#include <Perspective.h>

using namespace pbr;

void initializeEngine() {
    // Initialize resource manager
    Resource.initialize();

    // Initialize render hardware interface
    RHI.initialize();
}

PBRApp::PBRApp(const std::string& title, int width, int height) : OpenGLApplication(title, width, height) {

}

void PBRApp::prepare() {
    initializeEngine();
    
    // Initialize renderer
    _renderer.prepare();
    
    // Create camera and add it to the scene
    _camera = make_sref<Perspective>(_width, _height, Vec3(0, 0, -4), 
                                     Vec3(0, 0, 0), Vec3(0, 1, 0), 0.1f, 500.0f, 60.0f);
    _scene.addCamera(_camera);

    // Add shapes
    sref<Shape> mesh = make_sref<Mesh>("model.obj");
    mesh->prepare();
    mesh->_prog = 1;

    _scene.addShape(mesh);
}

void PBRApp::drawScene() {
    _renderer.render(_scene, *_camera);
}

void PBRApp::update(float dt) {
    Vector3 moveDir(0);

    if (_keys['w']) {
        moveDir += _camera->front();
    } else if (_keys['s']) {
        moveDir += -_camera->front();
    } 
    
    if (_keys['d']) {
        moveDir += -_camera->right();
    } else if (_keys['a']) {
        moveDir += _camera->right();
    }

    if (moveDir != Vector3(0)) {
        _camera->setPosition(_camera->position() + normalize(moveDir) * dt * 3.5f);
        _camera->updateMatrix();
    }
    //if (_mouseBtns[GLUT_LEFT_BUTTON])
    //    _camera->updateOrientation(_mouseDy * dt * 0.75f, _mouseDx * dt * 0.75f);

}

void PBRApp::cleanup()  {

}

void PBRApp::processKeyPress(unsigned char key, int x, int y)  {
    OpenGLApplication::processKeyPress(key, x, y);
}

void PBRApp::processMouseClick(int button, int state, int x, int y) {
    OpenGLApplication::processMouseClick(button, state, x, y);

    // Additional processing, picking, etc
    // Pixel (x, y)
    if (_mouseBtns[0]) {
        Vec2 pixel = Vec2(_clickX, _clickY);
    }
}