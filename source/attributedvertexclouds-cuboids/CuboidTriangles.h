
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

    virtual size_t size() const override;
    virtual size_t verticesCount() const override;
    virtual size_t staticByteSize() const override;
    virtual size_t byteSize() const override;
    virtual size_t vertexByteSize() const override;
    virtual size_t componentCount() const override;

    virtual void resize(size_t count) override;

    virtual const std::vector<gl::GLuint> & programs() const override;
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
    size_t verticesPerCuboid() const;
};
