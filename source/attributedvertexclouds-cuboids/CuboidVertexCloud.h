
#pragma once

#include <vector>

#include <glm/vec2.hpp>

#include <glbinding/gl/types.h>

#include "Cuboid.h"


class CuboidVertexCloud
{
public:
    CuboidVertexCloud();
    ~CuboidVertexCloud();

    bool initialized() const;
    void initialize();

    void render();

    bool loadShader();

    void setCube(size_t index, const Cuboid & cuboid);
    void setCube(size_t index, Cuboid && cuboid);

    size_t size() const;
    size_t byteSize() const;
    size_t vertexByteSize() const;
    size_t componentCount() const;

    void reserve(size_t count);
    void resize(size_t count);

    const std::vector<gl::GLuint> & programs() const;
public:
    std::vector<glm::vec2> center;
    std::vector<glm::vec2> extent;
    std::vector<glm::vec2> heightRange;
    std::vector<float> colorValue;
    std::vector<int> gradientIndex;

    gl::GLuint m_vertices;
    gl::GLuint m_vao;

    gl::GLuint m_vertexShader;
    gl::GLuint m_geometryShader;
    gl::GLuint m_fragmentShader;

    std::vector<gl::GLuint> m_programs;

    void initializeVAO();
};
