
#pragma once

#include <vector>
#include <mutex>

#include <glm/vec2.hpp>

#include <glbinding/gl/types.h>

#include "Prisma.h"
#include "PrismaImplementation.h"


class PrismaTriangleStrip : public PrismaImplementation
{
public:
    PrismaTriangleStrip();
    ~PrismaTriangleStrip();

    virtual void onInitialize() override;
    virtual void onRender() override;

    virtual bool loadShader() override;

    virtual void setPrisma(size_t index, const Prisma & prisma) override;

    virtual size_t size() const override;
    virtual size_t verticesCount() const override;
    virtual size_t staticByteSize() const override;
    virtual size_t byteSize() const override;
    virtual size_t vertexByteSize() const override;
    virtual size_t componentCount() const override;

    virtual void resize(size_t count) override;

    virtual gl::GLuint program() const override;
public:
    std::vector<glm::vec3> m_position;
    std::vector<glm::vec3> m_normal;
    std::vector<float> m_colorValue;

    std::vector<gl::GLint> m_multiStarts;
    std::vector<gl::GLint> m_multiCounts;

    gl::GLuint m_vertices;
    gl::GLuint m_vao;

    gl::GLuint m_vertexShader;
    gl::GLuint m_fragmentShader;

    gl::GLuint m_program;

    void initializeVAO();

protected:
    std::mutex m_mutex;
};
