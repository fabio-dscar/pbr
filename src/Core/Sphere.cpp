#include <Sphere.h>

#include <Geometry.h>
#include <RenderInterface.h>

using namespace pbr;

Sphere::Sphere(const Vec3& pos, float radius) : Shape(), _radius(radius) { }
Sphere::Sphere(const Mat4& objToWorld, float radius) : Shape(objToWorld), _radius(radius) { }

void Sphere::prepare() {
    _geometry = make_sref<Geometry>();

    // Generate geometry for sphere
    genSphereGeometry(*_geometry, _radius, 32, 32);

    // Upload geometry to the GPU
    RHI.uploadGeometry(_geometry);
}

void Sphere::draw() {
    RHI.drawGeometry(_geometry->rrid());
}

BBox3 Sphere::bbox() const {
    const Vec3 min = _position - Vec3(_radius);
    const Vec3 max = _position + Vec3(_radius);

    return BBox3(min, max);
}

BSphere Sphere::bSphere() const {
    return BSphere(_position, _radius);
}

bool Sphere::intersect(const Ray& ray) const {
    // Ray-sphere intersection
    return false;
}

bool Sphere::intersect(const Ray& ray, RayHitInfo& info) const {
    // Ray-sphere intersection
    return false;
}