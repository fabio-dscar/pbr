#ifndef __PBR_SHAPE_H__
#define __PBR_SHAPE_H__

#include <SceneObject.h>
#include <Bounds.h>
#include <Ray.h>
#include <Skybox.h>

using namespace pbr::math;

namespace pbr {

    class Material;
    class Geometry;

    class Shape : public SceneObject {
    public:
        Shape();
        Shape(const Vec3& position);
        Shape(const Mat4& objToWorld);

        virtual void prepare() = 0;
        virtual void draw() = 0;

        const sref<Material>& material() const;
        const sref<Geometry>& geometry() const;

        const Mat3& normalMatrix() const;

        virtual BBox3   bbox()    const = 0;
        virtual BSphere bSphere() const = 0;

        virtual bool intersect(const Ray& ray) const = 0;
        virtual bool intersect(const Ray& ray, RayHitInfo& info) const = 0;

        void updateMatrix() override;

        void setMaterial(const sref<Material>& mat);
        void updateMaterial(const Skybox& skybox);

        RRID _prog;

    protected:
        sref<Geometry> _geometry;
        sref<Material> _material;

        Mat3 _normalMatrix;
    };

}

#endif