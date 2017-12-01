#ifndef __PBR_SHAPE_H__
#define __PBR_SHAPE_H__

#include <SceneObject.h>
#include <Bounds.h>

namespace pbr {

    class Material;
    class Geometry;

    class Shape : public SceneObject {
    public:
        Shape() { }

        const sref<Material>& material() const;
        const sref<Geometry>& geometry() const;

        virtual BBox3   bbox()    const = 0;
        virtual BSphere bSphere() const = 0;

    private:
        sref<Geometry> _geometry;
        sref<Material> _material;
    };

}

#endif