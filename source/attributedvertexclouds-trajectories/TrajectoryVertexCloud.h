
#pragma once

#include <vector>

#include <glm/vec2.hpp>

#include <glbinding/gl/types.h>

#include "TrajectoryNode.h"


class TrajectoryVertexCloud
{
public:
    TrajectoryVertexCloud();
    ~TrajectoryVertexCloud();

    bool initialized() const;

    void initialize();
    void render();

    void onInitialize();
    void onRender();

    bool loadShader();

    void setTrajectoryNode(size_t index, const TrajectoryNode & node);

    size_t size() const;
    size_t verticesCount() const;
    size_t staticByteSize() const;
    size_t byteSize() const;
    size_t vertexByteSize() const;
    size_t componentCount() const;

    void resize(size_t count);

    gl::GLuint program() const;
public:
    bool m_initialized;

    std::vector<glm::vec3> m_position;
    std::vector<int> m_type;
    std::vector<float> m_colorValue;
    std::vector<float> m_sizeValue;

    gl::GLuint m_vertices;
    gl::GLuint m_vao;

    gl::GLuint m_vertexShader;
    gl::GLuint m_tessControlShader;
    gl::GLuint m_tessEvaluationShader;
    gl::GLuint m_geometryShader;
    gl::GLuint m_fragmentShader;

    gl::GLuint m_program;

    void initializeVAO();
    size_t verticesPerNode() const;
};
