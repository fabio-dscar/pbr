#include <Shape.h>

#include <Geometry.h>
#include <Material.h>

using namespace pbr;

const sref<Geometry>& Shape::geometry() const {
    return _geometry;
}

const sref<Material>& Shape::material() const {
    return _material;
}