
#pragma once

#include <vector>

#include <glm/vec2.hpp>

#include <glbinding/gl/types.h>

#include "Arc.h"
#include "ArcImplementation.h"


class ArcVertexCloud : public ArcImplementation
{
public:
    ArcVertexCloud();
    ~ArcVertexCloud();

    virtual void onInitialize() override;
    virtual void onRender() override;

    virtual bool loadShader() override;

    virtual void setArc(size_t index, const Arc & arc) override;

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
    std::vector<glm::vec2> m_center;
    std::vector<glm::vec2> m_heightRange;
    std::vector<glm::vec2> m_angleRange;
    std::vector<glm::vec2> m_radiusRange;
    std::vector<float> m_colorValue;
    std::vector<int> m_gradientIndex;
    std::vector<int> m_tessellationCount;

    gl::GLuint m_vertices;
    gl::GLuint m_vao;

    gl::GLuint m_vertexShader;
    gl::GLuint m_tessControlShader;
    gl::GLuint m_tessEvaluationShader;
    gl::GLuint m_geometryShader;
    gl::GLuint m_fragmentShader;

    std::vector<gl::GLuint> m_programs;

    void initializeVAO();
};
