#ifndef __PBR_RI_H__
#define __PBR_RI_H__

#include <GL/glew.h>

#include <PBR.h>
#include <PBRMath.h>
#include <Shader.h>

// Macro to syntax sugar the singleton getter
#define RHI RenderInterface::get()

using namespace pbr::math;

namespace pbr {

    class Geometry;

    template<class T>
    using vec = std::vector<T>;

    struct RHIGeometry {
        RRID buffer;
        sref<Geometry> geo;
    };

    struct RHIVertArray {
        GLuint      id;
        GLsizei     numIndices;
        GLsizei     numVertices;
        vec<GLuint> buffers;
    };
    
    struct RHIProgram {
        GLuint id;
    };

    struct RHIBuffer {
        GLuint id;
        GLenum target;
    };

    enum BufferType {
        BUFFER_VERTEX = 0,
        BUFFER_INDEX  = 1,
        BUFFER_SHARED = 2
    };

    enum BufferUsage {
        STATIC  = 0,
        STREAM  = 1,
        DYNAMIC = 2
    };

    enum AttribType : uint32 {
        ATTRIB_BYTE  = 0,
        ATTRIB_SHORT = 1,
        ATTRIB_UINT  = 2,
        ATTRIB_FLOAT = 3
    };

    struct BufferLayoutEntry {
        uint32     index;
        uint32     numElems;
        AttribType type;
        size_t     stride;
        size_t     offset;
    };

    struct BufferLayout {
        uint32 numEntries;
        BufferLayoutEntry* entries;
    };

    class RenderInterface {
    public:
        ~RenderInterface();

        static RenderInterface& get();

        void initialize();

        /* ===================================================================================
                Shaders
        =====================================================================================*/
        uint32 compileShader(const ShaderSource& source);
        bool   deleteShader (const ShaderSource& source);
        RRID   linkProgram  (const Shader& shader);

        std::string getProgramError(const Shader& shader);

        void useProgram(RRID id);

        void setFloat  (const std::string& name, float val);
        void setVector3(const std::string& name, const Vec3& vec);
        void setVector4(const std::string& name, const Vec4& vec);
        void setMatrix3(const std::string& name, const Mat3& mat);
        void setMatrix4(const std::string& name, const Mat4& mat);

        void setFloat  (int32 loc, float val);
        void setVector3(int32 loc, const Vec3& vec);
        void setVector4(int32 loc, const Vec4& vec);
        void setMatrix3(int32 loc, const Mat3& mat);
        void setMatrix4(int32 loc, const Mat4& mat);

        void setBufferBlock(const std::string& name, uint32 binding);

        int32  uniformLocation(RRID id, const std::string& name);
        uint32 uniformBlockLocation(RRID id, const std::string& name);

        /* ===================================================================================
                Geometry
        =====================================================================================*/
        void drawGeometry(RRID id);
        RRID uploadGeometry(const sref<Geometry>& geo);

        /* ===================================================================================
                 Buffers
        =====================================================================================*/
        RRID createVertexArray();
        bool deleteVertexArray(RRID id);

        RRID createBuffer(BufferType type, BufferUsage usage, size_t size, void* data);
        void bindBufferBase(RRID buffer, uint32 index);
        void setBufferLayout(RRID id, uint32 idx, AttribType type, uint32 numElems, uint32 stride, size_t offset);
        void setBufferLayout(RRID id, const BufferLayout& layout);
        bool updateBuffer(RRID id, size_t size, void* data);
        bool deleteBuffer(RRID id);

        bool isOpenGLError();
        void checkOpenGLError(const std::string& error);
    private:
        RenderInterface();

        RRID _currProgram;

        //vec<RHIGeometry>  _geometry;
        vec<RHIVertArray> _vertArrays;
        vec<RHIBuffer>    _buffers;
        vec<RHIProgram>   _programs;
    };

}

#endif