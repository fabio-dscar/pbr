#pragma warning(disable : 4018)

#include <RenderInterface.h>

#include <Geometry.h>
#include <Utils.h>

using namespace pbr::math;

const GLenum OGLShaderTypes[] = {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_GEOMETRY_SHADER,
    GL_COMPUTE_SHADER
};

const GLenum OGLBufferTargets[] = {
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
    GL_UNIFORM_BUFFER
};

const GLenum OGLAttrTypes[] = {
    GL_BYTE,
    GL_SHORT,
    GL_UNSIGNED_INT,
    GL_FLOAT
};

const GLenum OGLBufferUsage[] = {
    GL_STATIC_DRAW,
    GL_STREAM_DRAW,
    GL_DYNAMIC_DRAW
};

using namespace pbr;

RenderInterface::RenderInterface() {

}

RenderInterface::~RenderInterface() {   

}

RenderInterface& RenderInterface::get() {
    static RenderInterface _inst;
    return _inst;
}

void RenderInterface::initialize() {
    _programs.push_back({ 0 });
    _currProgram = 0;

    // Load standard engine shaders
    ShaderSource vsLighting(VERTEX_SHADER,   "lighting.vs");
    ShaderSource fsLighting(FRAGMENT_SHADER, "lighting.fs");

    Shader prog("lighting");
    prog.addShader(vsLighting);
    prog.addShader(fsLighting);
    prog.link();

    RHI.useProgram(prog.id());
    prog.registerUniformBlock("cameraBlock");
    RHI.setBufferBlock("cameraBlock", 0);
    RHI.useProgram(0);

    prog.registerUniform("ModelMatrix");
    prog.registerUniform("NormalMatrix");
}

RRID RenderInterface::uploadGeometry(const sref<Geometry>& geo) {
    auto verts   = geo->vertices();
    auto indices = geo->indices();

    // Create vertex array for the geometry
    RRID resId = createVertexArray();

    RHIVertArray& vertArray = _vertArrays[resId];
    glBindVertexArray(vertArray.id);

    // Create VBOs for vertex data and indices
    RRID vboIds[2];
    vboIds[0] = createBuffer(BUFFER_VERTEX, BufferUsage::STATIC, sizeof(Vertex) * verts.size(), &verts[0]);

    BufferLayoutEntry entries[] = { { 0, 3, ATTRIB_FLOAT, sizeof(Vertex), offsetof(Vertex, position) },
                                    { 1, 3, ATTRIB_FLOAT, sizeof(Vertex), offsetof(Vertex, normal) },
                                    { 2, 2, ATTRIB_FLOAT, sizeof(Vertex), offsetof(Vertex, uv) } };

    BufferLayout layout = { 3, &entries[0] };
    setBufferLayout(vboIds[0], layout);

    vboIds[1] = createBuffer(BUFFER_INDEX, BufferUsage::STATIC, sizeof(uint32) * indices.size(), &indices[0]);

    // Associate created VBOs with the VAO
    vertArray.buffers.push_back(vboIds[0]);
    vertArray.buffers.push_back(vboIds[1]);

    vertArray.numVertices = (GLsizei)verts.size();
    vertArray.numIndices  = (GLsizei)indices.size();

    glBindVertexArray(0);

    // Associate RRID of the VAO with the geometry
    geo->setRRID(resId);

    return resId;
}

void RenderInterface::drawGeometry(RRID id) {
    if (id < 0 || id >= _vertArrays.size())
        return; // Error

    RHIVertArray& vao = _vertArrays[id];
    if (vao.id == 0)
        return; // Error

    glBindVertexArray(vao.id);

    glDrawArrays(GL_TRIANGLES, 0, vao.numVertices);

    /*if (vao.numIndices != -1)
        glDrawElements(GL_TRIANGLES, (GLsizei)vao.numIndices, GL_UNSIGNED_INT, (GLvoid*)0);
    else
        glDrawArrays(GL_TRIANGLES, 0, vao.numVertices);
        */

    glBindVertexArray(0);
}

RRID RenderInterface::createVertexArray() {
    RHIVertArray vertArray;

    glGenVertexArrays(1, &vertArray.id);

    RRID resId = _vertArrays.size();
    _vertArrays.push_back(vertArray);

    return resId;
}

bool RenderInterface::deleteVertexArray(RRID id) {
    if (id < 0)
        return false; // Error

    RHIVertArray vao = _vertArrays[id];
    if (vao.id != 0) {
        glDeleteVertexArrays(1, &vao.id);
        vao.id = 0;
        vao.buffers.clear();
        vao.numIndices = 0;
        return true;
    }

    return false;
}

RRID RenderInterface::createBuffer(BufferType type, BufferUsage usage, size_t size, void* data) {
    RHIBuffer buffer;
    buffer.target = OGLBufferTargets[type];

    glGenBuffers(1, &buffer.id);
    glBindBuffer(buffer.target, buffer.id);
    glBufferData(buffer.target, size, data, OGLBufferUsage[usage]);
    glBindBuffer(buffer.target, 0);

    RRID resId = _buffers.size();
    _buffers.push_back(buffer);

    return resId;
}

void RenderInterface::bindBufferBase(RRID id, uint32 index) {
    if (id < 0 || id >= _buffers.size())
        return; // Error

    RHIBuffer buffer = _buffers[id];

    glBindBuffer(buffer.target, buffer.id);
    glBindBufferBase(buffer.target, index, buffer.id);
    glBindBuffer(buffer.target, 0);
}

void RenderInterface::setBufferLayout(RRID id, uint32 idx, AttribType type, uint32 numElems, uint32 stride, size_t offset) {
    if (id < 0 || id >= _buffers.size())
        return; // Error

    RHIBuffer buffer = _buffers[id];

    if (buffer.id == 0 || buffer.target != BUFFER_VERTEX)
        return; // Error

    glBindBuffer(buffer.target, buffer.id);
    glEnableVertexAttribArray(idx);
    glVertexAttribPointer(idx, numElems, OGLAttrTypes[type], GL_FALSE, (GLsizei)stride, (const void*)offset);
    glBindBuffer(buffer.target, 0);
}

void RenderInterface::setBufferLayout(RRID id, const BufferLayout& layout) {
    if (id < 0 || id >= _buffers.size())
        return; // Error

    RHIBuffer buffer = _buffers[id];
    if (buffer.id == 0 || buffer.target != OGLBufferTargets[BUFFER_VERTEX])
        return; // Error

    glBindBuffer(buffer.target, buffer.id);

    for (uint32 i = 0; i < layout.numEntries; ++i) {
        const BufferLayoutEntry& entry = layout.entries[i];

        glEnableVertexAttribArray(entry.index);
        glVertexAttribPointer(entry.index, entry.numElems, OGLAttrTypes[entry.type], GL_FALSE, (GLsizei)entry.stride, (const void*)entry.offset);
    }

    glBindBuffer(buffer.target, 0);
}

bool RenderInterface::updateBuffer(RRID id, size_t size, void* data) {
    if (id < 0 || id >= _buffers.size())
        return false; // Error

    RHIBuffer buffer = _buffers[id];
    if (buffer.id == 0)
        return false; // Error

    glBindBuffer(buffer.target, buffer.id);
    GLvoid* p = glMapBuffer(buffer.target, GL_WRITE_ONLY);
    memcpy(p, data, size);
    glUnmapBuffer(buffer.target);
    glBindBuffer(buffer.target, 0);

    return true;
}

bool RenderInterface::deleteBuffer(RRID id) {
    if (id < 0 || id >= _buffers.size())
        return false; // Error

    RHIBuffer buffer = _buffers[id];
    if (buffer.id != 0) {
        glDeleteBuffers(1, &buffer.id);
        buffer.id = 0;
        return true;
    }

    return false;
}

uint32 RenderInterface::compileShader(const ShaderSource& source) {
    // Create shader id
    GLuint id = glCreateShader(OGLShaderTypes[source.type()]);
    if (id == 0)
        std::cerr << "Could not create shader: " + source.name();

    // Set shader source and compile
    const char* c_str = source.source().c_str();
    glShaderSource(id, 1, &c_str, 0);
    glCompileShader(id);

    // Check if shader compiled
    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_TRUE)
        return id;

    // Check shader log for the error and print it
    GLint logLen;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);

    char* log = new char[logLen];
    glGetShaderInfoLog(id, logLen, &logLen, log);

    std::string message(log);
    delete[] log;

    std::cerr << "Shader " << source.name() << " compilation log:\n" << message;

    // Cleanup the shader
    glDeleteShader(id);

    return id;
}

bool RenderInterface::deleteShader(const ShaderSource& source) {
    uint32 id = source.id();
    if (id != 0) {
        glDeleteShader(id);
        return true;
    }

    return false;
}

RRID RenderInterface::linkProgram(const Shader& shader) {
    // Create program
    GLuint id = glCreateProgram();
    if (id == 0) {
        std::string message = getProgramError(shader);
        Utils::throwError("Could not create program " + shader.name());
    }

    // Attach shaders
    for (GLuint sid : shader.shaders()) {
        glAttachShader(id, sid);

        // Check attachment error
        checkOpenGLError("Could not attach shader (" + std::to_string(sid) +
                         ") to program (" + std::to_string(id) + ") " + shader.name() + ".");
    }

    glLinkProgram(id);

    GLint res;
    glGetProgramiv(id, GL_LINK_STATUS, &res);
    if (res != GL_TRUE) {
        // Check program log for the error and print it
        std::string message = getProgramError(shader);
        Utils::throwError(message);

        // Detach shaders
        for (GLuint sid : shader.shaders())
            glDetachShader(id, sid);

        // Delete the program
        glDeleteProgram(id);

        return false;
    }

    // Register uniform locations
    
    // Register uniform block locations

    // Detach shaders after successful linking
    for (GLuint sid : shader.shaders())
        glDetachShader(id, sid);

    RRID rrid = _programs.size();
    _programs.push_back({ id });

    return rrid;
}

std::string RenderInterface::getProgramError(const Shader& shader) {
    GLint logLen;
    glGetProgramiv(shader.id(), GL_INFO_LOG_LENGTH, &logLen);

    char* log = new char[logLen];
    glGetProgramInfoLog(shader.id(), logLen, &logLen, log);

    std::string strLog(log);
    delete[] log;

    return strLog;
}

void RenderInterface::useProgram(RRID id) {
    glUseProgram(_programs[id].id);
    _currProgram = id;
}

void RenderInterface::setFloat(const std::string& name, float val) {
    GLuint id = _programs[_currProgram].id;
    GLint loc = glGetUniformLocation(id, name.c_str());
    glUniform1f(loc, val);
}

void RenderInterface::setVector3(const std::string& name, const Vec3& vec) {
    GLuint id = _programs[_currProgram].id;
    GLint loc = glGetUniformLocation(id, name.c_str());
    glUniform3fv(loc, 1, (const GLfloat*)&vec);
}

void RenderInterface::setVector4(const std::string& name, const Vec4& vec) {
    GLuint id = _programs[_currProgram].id;
    GLint loc = glGetUniformLocation(id, name.c_str());
    glUniform4fv(loc, 1, (const GLfloat*)&vec);
}

void RenderInterface::setMatrix3(const std::string& name, const Mat3& mat) {
    GLuint id = _programs[_currProgram].id;
    GLint loc = glGetUniformLocation(id, name.c_str());
    glUniformMatrix3fv(loc, 1, GL_FALSE, (const GLfloat*)&mat);
}

void RenderInterface::setMatrix4(const std::string& name, const Mat4& mat) {
    GLuint id = _programs[_currProgram].id;
    GLint loc = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)&mat);
}

void RenderInterface::setFloat(int32 loc, float val) {
    glUniform1f(loc, val);
}

void RenderInterface::setVector3(int32 loc, const Vec3& vec) {
    glUniform3fv(loc, 1, (const GLfloat*)&vec);
}

void RenderInterface::setVector4(int32 loc, const Vec4& vec) {
    glUniform4fv(loc, 1, (const GLfloat*)&vec);
}

void RenderInterface::setMatrix3(int32 loc, const Mat3& mat) {
    glUniformMatrix3fv(loc, 1, GL_FALSE, (const GLfloat*)&mat);
}

void RenderInterface::setMatrix4(int32 loc, const Mat4& mat) {
    glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)&mat);
}

void RenderInterface::setBufferBlock(const std::string& name, uint32 binding) {
    GLuint id = _programs[_currProgram].id;
    GLint idx = glGetUniformBlockIndex(id, name.c_str());
    glUniformBlockBinding(id, idx, binding);
}

int32 RenderInterface::uniformLocation(RRID id, const std::string& name) {
    GLuint pid = _programs[id].id;
    return glGetUniformLocation(pid, name.c_str());
}
 
uint32 RenderInterface::uniformBlockLocation(RRID id, const std::string& name) {
    GLuint pid = _programs[id].id;
    return glGetUniformBlockIndex(pid, name.c_str());
}

void RenderInterface::checkOpenGLError(const std::string& error) {
    if (isOpenGLError()) {
        std::cerr << error << std::endl;
        std::cin.get();
        exit(EXIT_FAILURE);
    }
}

bool RenderInterface::isOpenGLError() {
    bool isError = false;
    GLenum errCode;
    const GLubyte *errString;
    while ((errCode = glGetError()) != GL_NO_ERROR) {
        isError = true;
        errString = gluErrorString(errCode);
        std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
    }
    return isError;
}