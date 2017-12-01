#include <Geometry.h>

using namespace pbr;
using namespace pbr::math;

RRID Geometry::rrid() const {
    return _id;
}

void Geometry::setRRID(RRID id) {
    _id = id;
}

void Geometry::addVertex(const Vertex& vertex) {
    _vertices.push_back(vertex);
}

void Geometry::addIndex(uint32 idx) {
    _indices.push_back(idx);
}

const std::vector<Vertex>& Geometry::vertices() const {
    return _vertices;
}

const std::vector<uint32>& Geometry::indices() const {
    return _indices;
}

void Geometry::setVertices(const std::vector<Vertex>& vertices) {
    _vertices.resize(vertices.size());
    std::copy(vertices.begin(), vertices.end(), _vertices.begin());
}

void Geometry::setIndices(const std::vector<uint32>& indices) {
    _indices.resize(indices.size());
    std::copy(indices.begin(), indices.end(), _indices.begin());
}

BBox3 Geometry::bbox() const {
    Vec3 min(FLOAT_INFINITY);
    Vec3 max(-FLOAT_INFINITY);

    for (const Vertex& v : _vertices) {
        min = math::min(v.position, min);
        max = math::max(v.position, max);
    }

    return BBox3(min, max);
}

BSphere Geometry::bSphere() const {
    const BBox3 box = bbox();
    return box.sphere();
}

void pbr::genSphereGeometry(Geometry& geo, float radius, uint32 widthSegments, uint32 heightSegments) {
    uint32 index = 0;
    Vertex vert;

    std::vector<std::vector<uint32>> grid;
    for (uint32 iy = 0; iy <= heightSegments; iy++) {
        float v = iy / heightSegments;

        std::vector<uint32> auxIdx;
        for (uint32 ix = 0; ix <= widthSegments; ix++) {
            float u = ix / widthSegments;

            // Position
            vert.position.x = -radius * std::cos(u * 2.0f * PI) * std::sin(v * PI);
            vert.position.y = radius * std::cos(v * PI);
            vert.position.z = radius * std::sin(u * 2.0f * PI) * std::sin(v * PI);

            // Normal
            vert.normal = normalize(Vec3(vert.position.x,
                                         vert.position.y,
                                         vert.position.z));

            // Texture Coords
            vert.uv = Vec2(u, 1 - v);

            auxIdx.push_back(index++);

            geo.addVertex(vert);
        }

        grid.push_back(auxIdx);
    }

    for (uint32 iy = 0; iy < heightSegments; iy++) {
        for (uint32 ix = 0; ix < widthSegments; ix++) {

            uint32 a = grid[iy][ix + 1];
            uint32 b = grid[iy][ix];
            uint32 c = grid[iy + 1][ix];
            uint32 d = grid[iy + 1][ix + 1];

            if (iy != 0) {
                geo.addIndex(a);
                geo.addIndex(b);
                geo.addIndex(d);
            }

            if (iy != heightSegments - 1) {
                geo.addIndex(b);
                geo.addIndex(c);
                geo.addIndex(d);
            }
        }
    }
}

void pbr::genBoxGeometry(Geometry& geo, uint32 widthSegments, uint32 heightSegments, uint32 depthSegments) {

}

/*void pbr::fromObjFile(Geometry& geo, const ObjFile& objFile) {
    geo.setIndices(objFile.indices);

    for (const ObjVertex& v : objFile.vertices)
        geo.addVertex({ v.pos, v.normal, v.texCoord });
}*/