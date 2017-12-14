#include <Geometry.h>

#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <path.h>
#include <Hash.hpp>

#include <PBRMath.h>

#undef min
#undef max

using namespace filesystem;

using namespace pbr;
using namespace pbr::math;

namespace std {
    template<> struct hash<ObjVertex> {
        size_t operator()(ObjVertex const& vertex) const {
            return ((hash<Vec3>()(vertex.pos) ^
                (hash<Vec3>()(vertex.normal) << 1)) >> 1) ^
                     (hash<Vec2>()(vertex.texCoord) << 1);
        }
    };
}

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
    Vec3 pMin( FLOAT_INFINITY);
    Vec3 pMax(-FLOAT_INFINITY);

    for (const Vertex& v : _vertices) {
        pMin = math::min(v.position, pMin);
        pMax = math::max(v.position, pMax);
    }

    return BBox3(pMin, pMax);
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
            vert.position.y =  radius * std::cos(v * PI);
            vert.position.z =  radius * std::sin(u * 2.0f * PI) * std::sin(v * PI);

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

bool pbr::loadObj(const std::string& filePath, ObjFile& obj) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    path fp = path(filePath);
    if (!fp.exists())
        return false;

    obj.objName = fp.filename();
    obj.indices.clear();
    obj.vertices.clear();

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str())) {
        throw std::runtime_error(err);
    }

    uint32 count = 0;
    std::unordered_map<ObjVertex, uint32_t> uniqueVertices = { };
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            /*ObjVertex vertex = { };

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (attrib.normals.size() > 0) {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (attrib.texcoords.size() > 0) {
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32>(obj.vertices.size());
                obj.vertices.push_back(vertex);
            }

            obj.indices.push_back(uniqueVertices[vertex]);*/

            ObjVertex vertex = { };

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (attrib.normals.size() > 0) {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (attrib.texcoords.size() > 0) {
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            obj.vertices.push_back(vertex);

            obj.indices.push_back(count);
            count++;
        }
    }

    return true;
}

void pbr::fromObjFile(Geometry& geo, const ObjFile& objFile) {
    geo.setIndices(objFile.indices);

    for (const ObjVertex& v : objFile.vertices)
        geo.addVertex({ v.pos, v.normal, v.texCoord });
}