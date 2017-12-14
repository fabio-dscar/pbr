#ifndef __PBR_SCENE_H__
#define __PBR_SCENE_H__

#include <PBR.h>
#include <Bounds.h>
#include <Ray.h>

using namespace pbr::math;

namespace pbr {

    class Camera;
    class Shape;
    class Light;

    template<class T>
    using vec = std::vector<T>;

    class Scene {
    public:
        Scene();

        // TODO
        //bool intersect(const Ray& ray);

        void addCamera(const sref<Camera>& camera);
        void addShape (const sref<Shape>&  shape);      
        void addLight (const sref<Light>&  light);

        const vec<sref<Camera>>& cameras() const;
        const vec<sref<Shape>>&  shapes()  const;
        const vec<sref<Light>>&  lights()  const;

    private:
        BBox3 _bbox;

        vec<sref<Camera>> _cameras;
        vec<sref<Shape>>  _shapes;
        vec<sref<Light>>  _lights;
    };

}

#endif