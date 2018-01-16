#include <PBRApp.h>

#include <Resources.h>
#include <RenderInterface.h>

#include <Shape.h>
#include <Sphere.h>
#include <Mesh.h>
#include <Texture.h>
#include <Skybox.h>

#include <Camera.h>
#include <Perspective.h>

#include <Transform.h>

#include <PBRMaterial.h>

#include <GUI.h>

using namespace pbr;

void initializeEngine() {
    // Initialize resource manager
    Resource.initialize();

    // Initialize render hardware interface
    RHI.initialize();
}

PBRApp::PBRApp(const std::string& title, int width, int height) : OpenGLApplication(title, width, height), 
                         _skyToggle(true), _selectedShape(nullptr), _showGUI(true), _skybox(1) {

}

void PBRApp::prepare() {
    initializeEngine();

    ImGui_Init(_width, _height);
    
    // Initialize renderer
    _renderer.prepare();
    _exposure = _renderer.exposure();
    _gamma    = _renderer.gamma();
    memcpy(_toneParams, _renderer.toneParams(), sizeof(float) * 7);

    // Load cubemaps
    vec<std::string> folders = { "Pinetree", "Ruins", "WalkOfFame", "WinterForest" };
    for (const std::string& str : folders)
        _skyboxes.emplace_back(str);

    for (Skybox& sky : _skyboxes)
        sky.initialize();

    // Create camera and add it to the scene
    _camera = make_sref<Perspective>(_width, _height, Vec3(-3, 3, -3), 
                                     Vec3(0, 0, 0), Vec3(0, 1, 0), 0.1f, 500.0f, 60.0f);
    _scene.addCamera(_camera);

    // Add shapes
    sref<Shape> mesh = make_sref<Mesh>("gun.obj");
    mesh->prepare();
    mesh->_prog = -1; //Resource.getShader("lighting")->id();
    //mesh->setScale(0.75, 0.75, 0.75);
    mesh->setScale(5.5f, 5.5f, 5.5f);
    mesh->updateMatrix();
    _scene.addShape(mesh);
    
    Image diffTex;
    TexSampler texSampler;
    diffTex.loadImage("albedo.png");
    RRID diffId = RHI.createTexture(diffTex, texSampler);

    Image normalTex;
    normalTex.loadImage("normals.png");
    RRID normalId = RHI.createTexture(normalTex, texSampler);

    Image metallicTex;
    metallicTex.loadImage("metallic.png");
    RRID metalId = RHI.createTexture(metallicTex, texSampler);

    Image roughTex;
    roughTex.loadImage("roughness.png");
    RRID roughId = RHI.createTexture(roughTex, texSampler);

    sref<PBRMaterial> mat = make_sref<PBRMaterial>();
    mat->setDiffuse(diffId);
    mat->setNormal(normalId);
    mat->setMetallic(metalId);
    //mat->setMetallic(1.0f);
    //mat->setRoughness(0.2f);
    mat->setRoughness(roughId);
    mat->setSpecular(Vec3(0.04f, 0.04f, 0.04f));
    //mat->setSpecular(Vec3(1.0f, 0.71f, 0.29f));
    mesh->setMaterial(mat);

    sref<Shape> quad = make_sref<Mesh>("quad.obj");
    quad->prepare();
    quad->_prog = Resource.getShader("lightingTex")->id();
    quad->setPosition(Vec3(0, -3.0f, 0));
    quad->setScale(50, 1, 50);
    quad->updateMatrix();
    //_scene.addShape(quad);

    /*RRID skyShader = Resource.getShader("skybox")->id();
    RRID skyTex    = Resource.getTexture("sky")->rrid();
    sref<Skybox> skybox = make_sref<Skybox>(skyShader, skyTex);
    skybox->initialize();
    _scene.setEnvironment(*skybox);*/

    changeSkybox(_skybox);
}

void PBRApp::drawScene() {
    _renderer.render(_scene, *_camera);

    if (_showGUI)
        drawInterface();
}

void PBRApp::restoreToneDefaults() {
    _gamma = 2.4f;
    _exposure = 3.0f;
    _toneParams[0] = 0.15f;
    _toneParams[1] = 0.5f;
    _toneParams[2] = 0.1f;
    _toneParams[3] = 0.2f;
    _toneParams[4] = 0.02f;
    _toneParams[5] = 0.3f;
    _toneParams[6] = 11.2f;
}

void PBRApp::update(float dt) {
    if (_mouseBtns[2]) {
        _camera->updateOrientation(_mouseDy * dt * 0.75f, _mouseDx * dt * 0.75f);
        _camera->updateViewMatrix();
    }

    Vector3 moveDir(0);
    if (_keys['w']) {
        moveDir += -_camera->front();
    } else if (_keys['s']) {
        moveDir += _camera->front();
    } 
    
    if (_keys['d']) {
        moveDir += _camera->right();
    } else if (_keys['a']) {
        moveDir += -_camera->right();
    }

    if (moveDir != Vector3(0)) {
        _camera->setPosition(_camera->position() + normalize(moveDir) * dt * 6.0f);
        _camera->updateViewMatrix();
    }

    // Update renderer parameters
    _renderer.setExposure(_exposure);
    _renderer.setGamma(_gamma);
    _renderer.setToneParams(_toneParams);
    _renderer.setSkyboxDraw(_skyToggle);
}

void PBRApp::cleanup()  {

}

void PBRApp::processKeyPress(unsigned char key, int x, int y)  {
    OpenGLApplication::processKeyPress(key, x, y);

    if (key == 'h')
        _showGUI = !_showGUI;
}

void PBRApp::processMouseClick(int button, int state, int x, int y) {
    OpenGLApplication::processMouseClick(button, state, x, y);
    
    // Additional processing, picking, etc
    // Pixel (x, y)
    if (_mouseBtns[1]) {
        Vec2 pixel = Vec2(_clickX, _clickY);

       	// Create ray from pixel
        Vec3 rayNDS = Vec3((2.0f * pixel.x) / _width - 1.0f,
                            1.0f - (2.0f * pixel.y) / _height,
                            1.0f);
        
        Vec4 rayClip = Vec4(rayNDS.x, rayNDS.y, -1.0, 1.0);
        
        Vec4 rayEye = inverse(_camera->projMatrix()) * rayClip;
        rayEye = Vec4(rayEye.x, rayEye.y, -1.0, 0.0);
        
        Vec3 rayWorld = (inverse(_camera->viewMatrix()) * rayEye);
        rayWorld = normalize(rayWorld);
        
        Ray ray = Ray(_camera->position(), rayWorld);
        
        if (_scene.intersect(ray, &_selectedShape)) {
            std::cout << "Intersection found." << std::endl;
        }

    }
}

void PBRApp::drawInterface() {
    ImGui_NewFrame(_mouseX, _mouseY, _mouseBtns);

    // Environment window
    ImGui::Begin("Environment");
    ImGui::Checkbox("Draw Skybox", &_skyToggle);
    if (ImGui::Combo("Current Environment", &_skybox, "Pinetree\0Ruins\0Walk of Fame\0Winter Forest\0"))
        changeSkybox(_skybox);
    ImGui::End();

    // Tone map window
    ImGui::Begin("Uncharted Tone Map");

    ImGui::TextWrapped("Uncharted tone function parameters. They control the shape of the tone curve. For more information check the report.");

    ImGui::SliderFloat("gamma", &_gamma, 0.0f, 4.0f);
    ImGui::SliderFloat("exposure", &_exposure, 0.0f, 5.0f);
 
    ImGui::Separator();

    ImGui::SliderFloat("A", &_toneParams[0], 0.0f, 2.0f);
    ImGui::SliderFloat("B", &_toneParams[1], 0.0f, 2.0f);
    ImGui::SliderFloat("C", &_toneParams[2], 0.0f, 2.0f);
    ImGui::SliderFloat("D", &_toneParams[3], 0.0f, 2.0f);
    ImGui::SliderFloat("E", &_toneParams[4], 0.0f, 2.0f);
    ImGui::SliderFloat("J", &_toneParams[5], 0.0f, 2.0f);
    ImGui::SliderFloat("W", &_toneParams[6], 0.0f, 20.0f);

    if (ImGui::Button("Restore defaults"))
        restoreToneDefaults();

    ImGui::End();

    // Selected object window
    if (_selectedShape != nullptr) {
        ImGui::Begin("Selected Object");

        static float m = 0.5f;
        static float a = 0.5f;

        ImGui::SliderFloat("Metallic", &m, 0.0f, 1.0f);

        float arr[3];
        ImGui::ColorEdit3("Diffuse", &arr[0]);

        ImGui::End();
    }

    ImGui::Render();
}

void PBRApp::changeSkybox(int id) {
    _scene.setEnvironment(_skyboxes[id]);
}