
#pragma once

#include <vector>

#include <glm/vec2.hpp>

#include <glbinding/gl/types.h>

#include "Cuboid.h"


class CuboidTriangles
{
public:
    CuboidTriangles();
    ~CuboidTriangles();

    bool initialized() const;
    void initialize();

    void render();

    bool loadShader();

    void setCube(size_t index, const Cuboid & cuboid);

    size_t size() const;
    size_t verticesPerCuboid() const;
    size_t verticesCount() const;
    size_t byteSize() const;
    size_t vertexByteSize() const;
    size_t componentCount() const;

    void reserve(size_t count);
    void resize(size_t count);

    const std::vector<gl::GLuint> & programs() const;
public:
    std::vector<glm::vec3> m_vertex;
    std::vector<glm::vec3> m_normal;
    std::vector<float> m_colorValue;
    std::vector<int> m_gradientIndex;

    gl::GLuint m_vertices;
    gl::GLuint m_vao;

    gl::GLuint m_vertexShader;
    gl::GLuint m_fragmentShader;

    std::vector<gl::GLuint> m_programs;

    void initializeVAO();
};
