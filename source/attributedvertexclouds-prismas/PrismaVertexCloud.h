
#pragma once

#include <vector>
#include <mutex>

#include <glm/vec2.hpp>

#include <glbinding/gl/types.h>

#include "Prisma.h"
#include "PrismaImplementation.h"


class PrismaVertexCloud : public PrismaImplementation
{
public:
    PrismaVertexCloud();
    ~PrismaVertexCloud();

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

    virtual const std::vector<gl::GLuint> & programs() const override;
public:
    std::vector<glm::vec2> m_center;
    std::vector<glm::vec2> m_heightRange;
    std::vector<float> m_colorValue;
    std::vector<float> m_gradientIndex;

    std::vector<glm::vec2> m_positions;
    std::vector<int> m_prismaIndices;

    gl::GLuint m_vertices;
    gl::GLuint m_centerHeightRangeBuffer;
    gl::GLuint m_colorValueGradientIndexBuffer;
    gl::GLuint m_centerHeightRangeTexture;
    gl::GLuint m_colorValueGradientIndexTexture;

    gl::GLuint m_vao;

    gl::GLuint m_vertexShader;
    gl::GLuint m_geometryShader;
    gl::GLuint m_fragmentShader;

    std::vector<gl::GLuint> m_programs;

    void initializeVAO();

protected:
    std::mutex m_mutex;
};
