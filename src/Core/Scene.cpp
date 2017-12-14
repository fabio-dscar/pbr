#include <Scene.h>

#include <Shape.h>

using namespace pbr;

Scene::Scene() : _bbox(Vec3(0)) { }

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

const vec<sref<Camera>>& Scene::cameras() const {
    return _cameras;
}

const vec<sref<Shape>>& Scene::shapes() const {
    return _shapes;
}

const vec<sref<Light>>& Scene::lights() const {
    return _lights;
}