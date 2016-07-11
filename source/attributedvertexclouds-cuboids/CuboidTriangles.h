
#pragma once

#include <vector>

#include <glm/vec2.hpp>

#include <glbinding/gl/types.h>

#include "Cuboid.h"
#include "CuboidImplementation.h"


class CuboidTriangles : public CuboidImplementation
{
public:
    CuboidTriangles();
    ~CuboidTriangles();

    virtual void onInitialize() override;
    virtual void onRender() override;

    virtual bool loadShader() override;

    virtual void setCube(size_t index, const Cuboid & cuboid) override;

    virtual size_t size() const;
    virtual size_t verticesPerCuboid() const;
    virtual size_t verticesCount() const;
    virtual size_t staticByteSize() const;
    virtual size_t byteSize() const;
    virtual size_t vertexByteSize() const;
    virtual size_t componentCount() const;

    virtual void resize(size_t count);

    virtual const std::vector<gl::GLuint> & programs() const;
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
