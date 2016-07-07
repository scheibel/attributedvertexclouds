
#include "CuboidVertexCloud.h"

#include <glbinding/gl/gl.h>

#include "common.h"

using namespace gl;

CuboidVertexCloud::CuboidVertexCloud()
: m_vertices(0)
, m_vao(0)
, m_vertexShader(0)
, m_geometryShader(0)
, m_fragmentShader(0)
{
}

CuboidVertexCloud::~CuboidVertexCloud()
{
    glDeleteBuffers(1, &m_vertices);
    glDeleteVertexArrays(1, &m_vao);
}

bool CuboidVertexCloud::initialized() const
{
    return m_vertices > 0;
}

void CuboidVertexCloud::initialize()
{
    glGenBuffers(1, &m_vertices);
    glGenVertexArrays(1, &m_vao);

    initializeVAO();

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    m_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    m_programs.resize(1);
    m_programs[0] = glCreateProgram();

    glAttachShader(m_programs[0], m_vertexShader);
    glAttachShader(m_programs[0], m_geometryShader);
    glAttachShader(m_programs[0], m_fragmentShader);

    loadShader();
}

void CuboidVertexCloud::initializeVAO()
{
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    glBufferData(GL_ARRAY_BUFFER, byteSize(), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 0, verticesCount() * sizeof(float) * 2, m_center.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 2, verticesCount() * sizeof(float) * 2, m_extent.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 4, verticesCount() * sizeof(float) * 2, m_heightRange.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 6, verticesCount() * sizeof(float) * 1, m_colorValue.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 7, verticesCount() * sizeof(float) * 1, m_gradientIndex.data());

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 2));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 4));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 6));
    glVertexAttribIPointer(4, 1, GL_INT, sizeof(int), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 7));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool CuboidVertexCloud::loadShader()
{
    const auto vertexShaderSource = textFromFile("data/shaders/cuboids-avc/standard.vert");
    const auto vertexShaderSource_ptr = vertexShaderSource.c_str();
    if(vertexShaderSource_ptr)
        glShaderSource(m_vertexShader, 1, &vertexShaderSource_ptr, 0);

    glCompileShader(m_vertexShader);

    bool success = checkForCompilationError(m_vertexShader, "vertex shader");


    const auto geometryShaderSource = textFromFile("data/shaders/cuboids-avc/standard.geom");
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

    if (!success)
    {
        return false;
    }

    glBindFragDataLocation(m_programs[0], 0, "out_color");

    return true;
}

void CuboidVertexCloud::setCube(size_t index, const Cuboid & cuboid)
{
    m_center[index] = glm::vec2(cuboid.center.x, cuboid.center.z);
    m_extent[index] = glm::vec2(cuboid.extent.x, cuboid.extent.z);
    m_heightRange[index] = glm::vec2(cuboid.center.y - cuboid.extent.y / 2.0f, cuboid.extent.y);
    m_colorValue[index] = cuboid.colorValue;
    m_gradientIndex[index] = cuboid.gradientIndex;
}

size_t CuboidVertexCloud::size() const
{
    return m_center.size();
}

size_t CuboidVertexCloud::verticesPerCuboid() const
{
    return 1;
}

size_t CuboidVertexCloud::verticesCount() const
{
    return size() * verticesPerCuboid();
}

size_t CuboidVertexCloud::byteSize() const
{
    return verticesPerCuboid() * size() * vertexByteSize();
}

size_t CuboidVertexCloud::vertexByteSize() const
{
    return sizeof(float) * componentCount();
}

size_t CuboidVertexCloud::componentCount() const
{
    return 8;
}

void CuboidVertexCloud::resize(size_t count)
{
    m_center.resize(count);
    m_extent.resize(count);
    m_heightRange.resize(count);
    m_colorValue.resize(count);
    m_gradientIndex.resize(count);
}

void CuboidVertexCloud::render()
{
    glBindVertexArray(m_vao);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glUseProgram(m_programs[0]);
    glDrawArrays(GL_POINTS, 0, size());

    glUseProgram(0);

    glBindVertexArray(0);
}

const std::vector<gl::GLuint> & CuboidVertexCloud::programs() const
{
    return m_programs;
}
