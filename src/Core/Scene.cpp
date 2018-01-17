#include <Scene.h>

#include <Shape.h>
#include <Skybox.h>

using namespace pbr;

Scene::Scene() : _bbox(Vec3(0)), _skybox(nullptr) { }

bool Scene::intersect(const Ray& ray, Shape** obj) {
    RayHitInfo info;
    float t;
    info.dist = FLOAT_INFINITY;
    info.obj  = nullptr;
    
    for (sref<Shape> shape : _shapes) {
        BBox3 bbox = shape->bbox();
        if (bbox.intersectRay(ray, &t)) {
            if (t < info.dist) {
                info.dist = t;
                info.obj  = shape.get();
            }
        }
    }

    *obj = (Shape*)info.obj;

    return info.obj != nullptr;
}

void Scene::addCamera(const sref<Camera>& camera) {
    _cameras.push_back(camera);
}

void Scene::addShape(const sref<Shape>& shape) {
    _bbox.expand(shape->bbox());
    _shapes.push_back(shape);
}

void Scene::addLight(const sref<Light>& light) {
    _lights.push_back(light);
}

void Scene::setEnvironment(const Skybox& skybox) {
    _skybox = &skybox;

    for (sref<Shape>& shape : _shapes)
        shape->updateMaterial(skybox);
}

const vec<sref<Camera>>& Scene::cameras() const {
    return _cameras;
}

const vec<sref<Shape>>& Scene::shapes() const {
    return _shapes;
}

const vec<sref<Light>>& Scene::lights() const {
    return _lights;
}

const Skybox& Scene::skybox() const {
    return *_skybox;
}

bool Scene::hasSkybox() const {
    return _skybox != nullptr;
}