
#include "ArcVertexCloud.h"

#include <glbinding/gl/gl.h>

#include "common.h"

using namespace gl;

ArcVertexCloud::ArcVertexCloud()
: ArcImplementation("Attributed Vertex Cloud")
, m_vertices(0)
, m_vao(0)
, m_vertexShader(0)
, m_tessControlShader(0)
, m_tessEvaluationShader(0)
, m_geometryShader(0)
, m_fragmentShader(0)
{
}

ArcVertexCloud::~ArcVertexCloud()
{
    glDeleteBuffers(1, &m_vertices);
    glDeleteVertexArrays(1, &m_vao);
}

void ArcVertexCloud::onInitialize()
{
    glGenBuffers(1, &m_vertices);
    glGenVertexArrays(1, &m_vao);

    initializeVAO();

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    m_tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    m_tessEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    m_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    m_programs.resize(2);
    m_programs[0] = glCreateProgram();
    m_programs[1] = glCreateProgram();

    glAttachShader(m_programs[0], m_vertexShader);
    glAttachShader(m_programs[0], m_tessControlShader);
    glAttachShader(m_programs[0], m_tessEvaluationShader);
    glAttachShader(m_programs[0], m_geometryShader);
    glAttachShader(m_programs[0], m_fragmentShader);

    glAttachShader(m_programs[1], m_vertexShader);
    glAttachShader(m_programs[1], m_tessControlShader);
    glAttachShader(m_programs[1], m_tessEvaluationShader);
    glAttachShader(m_programs[1], m_geometryShader);

    loadShader();
}

void ArcVertexCloud::initializeVAO()
{
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    glBufferData(GL_ARRAY_BUFFER, byteSize(), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 0, verticesCount() * sizeof(float) * 2, m_center.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 2, verticesCount() * sizeof(float) * 2, m_heightRange.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 4, verticesCount() * sizeof(float) * 2, m_angleRange.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 6, verticesCount() * sizeof(float) * 2, m_radiusRange.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 8, verticesCount() * sizeof(float) * 1, m_colorValue.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 9, verticesCount() * sizeof(float) * 1, m_gradientIndex.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 10, verticesCount() * sizeof(float) * 1, m_tessellationCount.data());

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 2));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 4));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 6));
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 8));
    glVertexAttribIPointer(5, 1, GL_INT, sizeof(int), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 9));
    glVertexAttribIPointer(6, 1, GL_INT, sizeof(int), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 10));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool ArcVertexCloud::loadShader()
{
    const auto vertexShaderSource = textFromFile("data/shaders/arcs-avc/standard.vert");
    const auto vertexShaderSource_ptr = vertexShaderSource.c_str();
    if(vertexShaderSource_ptr)
        glShaderSource(m_vertexShader, 1, &vertexShaderSource_ptr, 0);

    glCompileShader(m_vertexShader);

    bool success = checkForCompilationError(m_vertexShader, "vertex shader");


    const auto tessControlShaderSource = textFromFile("data/shaders/arcs-avc/standard.tcs");
    const auto tessControlShaderSource_ptr = tessControlShaderSource.c_str();
    if(tessControlShaderSource_ptr)
        glShaderSource(m_tessControlShader, 1, &tessControlShaderSource_ptr, 0);

    glCompileShader(m_tessControlShader);

    success &= checkForCompilationError(m_tessControlShader, "tessellation control shader");


    const auto tessEvaluationShaderSource = textFromFile("data/shaders/arcs-avc/standard.tes");
    const auto tessEvaluationShaderSource_ptr = tessEvaluationShaderSource.c_str();
    if(tessEvaluationShaderSource_ptr)
        glShaderSource(m_tessEvaluationShader, 1, &tessEvaluationShaderSource_ptr, 0);

    glCompileShader(m_tessEvaluationShader);

    success &= checkForCompilationError(m_tessEvaluationShader, "tessellation evaluation shader");


    const auto geometryShaderSource = textFromFile("data/shaders/arcs-avc/standard.geom");
    const auto geometryShaderSource_ptr = geometryShaderSource.c_str();
    if(geometryShaderSource_ptr)
        glShaderSource(m_geometryShader, 1, &geometryShaderSource_ptr, 0);

    glCompileShader(m_geometryShader);

    success &= checkForCompilationError(m_geometryShader, "geometry shader");


    const auto fragmentShaderSource = textFromFile("data/shaders/visualization.frag");
    const auto fragmentShaderSource_ptr = fragmentShaderSource.c_str();
    if(fragmentShaderSource_ptr)
        glShaderSource(m_fragmentShader, 1, &fragmentShaderSource_ptr, 0);

    glCompileShader(m_fragmentShader);

    success &= checkForCompilationError(m_fragmentShader, "fragment shader");


    if (!success)
    {
        return false;
    }

    glLinkProgram(m_programs[0]);

    success &= checkForLinkerError(m_programs[0], "program");

    glLinkProgram(m_programs[1]);

    success &= checkForLinkerError(m_programs[1], "depth only program");

    if (!success)
    {
        return false;
    }

    glBindFragDataLocation(m_programs[0], 0, "out_color");

    return true;
}

void ArcVertexCloud::setArc(size_t index, const Arc & arc)
{
    m_center[index] = arc.center;
    m_heightRange[index] = arc.heightRange;
    m_angleRange[index] = arc.angleRange;
    m_radiusRange[index] = arc.radiusRange;
    m_colorValue[index] = arc.colorValue;
    m_gradientIndex[index] = arc.gradientIndex;
    m_tessellationCount[index] = arc.tessellationCount;
}

size_t ArcVertexCloud::size() const
{
    return m_center.size();
}

size_t ArcVertexCloud::verticesPerArc() const
{
    return 1;
}

size_t ArcVertexCloud::verticesCount() const
{
    return size() * verticesPerArc();
}

size_t ArcVertexCloud::staticByteSize() const
{
    return 0;
}

size_t ArcVertexCloud::byteSize() const
{
    return verticesPerArc() * size() * vertexByteSize();
}

size_t ArcVertexCloud::vertexByteSize() const
{
    return sizeof(float) * componentCount();
}

size_t ArcVertexCloud::componentCount() const
{
    return 11;
}

void ArcVertexCloud::resize(size_t count)
{
    m_center.resize(count);
    m_heightRange.resize(count);
    m_angleRange.resize(count);
    m_radiusRange.resize(count);
    m_colorValue.resize(count);
    m_gradientIndex.resize(count);
    m_tessellationCount.resize(count);
}

void ArcVertexCloud::onRender()
{
    glBindVertexArray(m_vao);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glPatchParameteri(GL_PATCH_VERTICES, 1);

    // Pre-Z Pass
    //glDepthFunc(GL_LEQUAL);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glUseProgram(m_programs[1]);
    //glDrawArrays(GL_PATCHES, 0, size());

    // Color Pass

    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    //glDepthMask(GL_FALSE);
    glUseProgram(m_programs[0]);
    glDrawArrays(GL_PATCHES, 0, size());

    glUseProgram(0);

    glBindVertexArray(0);
}

const std::vector<gl::GLuint> & ArcVertexCloud::programs() const
{
    return m_programs;
}
