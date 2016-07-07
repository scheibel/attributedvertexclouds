
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

    m_programs.resize(2);
    m_programs[0] = glCreateProgram();
    m_programs[1] = glCreateProgram();

    glAttachShader(m_programs[0], m_vertexShader);
    glAttachShader(m_programs[0], m_geometryShader);
    glAttachShader(m_programs[0], m_fragmentShader);

    glAttachShader(m_programs[1], m_vertexShader);
    glAttachShader(m_programs[1], m_geometryShader);

    loadShader();
}

void CuboidVertexCloud::initializeVAO()
{
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    glBufferData(GL_ARRAY_BUFFER, byteSize(), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, size() * sizeof(float) * 0, size() * sizeof(float) * 2, center.data());
    glBufferSubData(GL_ARRAY_BUFFER, size() * sizeof(float) * 2, size() * sizeof(float) * 2, extent.data());
    glBufferSubData(GL_ARRAY_BUFFER, size() * sizeof(float) * 4, size() * sizeof(float) * 2, heightRange.data());
    glBufferSubData(GL_ARRAY_BUFFER, size() * sizeof(float) * 6, size() * sizeof(float) * 1, colorValue.data());
    glBufferSubData(GL_ARRAY_BUFFER, size() * sizeof(float) * 7, size() * sizeof(float) * 1, gradientIndex.data());

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(size() * sizeof(float) * 0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(size() * sizeof(float) * 2));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(size() * sizeof(float) * 4));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), reinterpret_cast<void*>(size() * sizeof(float) * 6));
    glVertexAttribIPointer(4, 1, GL_INT, sizeof(int), reinterpret_cast<void*>(size() * sizeof(float) * 7));

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

    glLinkProgram(m_programs[1]);

    success &= checkForLinkerError(m_programs[1], "depth only program");

    if (!success)
    {
        return false;
    }

    glBindFragDataLocation(m_programs[0], 0, "out_color");

    return true;
}

void CuboidVertexCloud::setCube(size_t index, const Cuboid & cuboid)
{
    center[index] = glm::vec2(glm::linearRand(-8.0f, 8.0f), glm::linearRand(-8.0f, 8.0f));
    extent[index] = glm::vec2(glm::linearRand(0.1f, 0.4f), glm::linearRand(0.1f, 0.4f));
    heightRange[index] = glm::vec2(glm::linearRand(-0.5f, 0.5f));
    heightRange[index].y += glm::linearRand(0.1f, 0.4f);
    colorValue[index] = glm::linearRand(0.0f, 1.0f);
    gradientIndex[index] = 0;
}

void CuboidVertexCloud::setCube(size_t index, Cuboid && cuboid)
{
    center[index] = glm::vec2(cuboid.center.x, cuboid.center.z);
    extent[index] = glm::vec2(cuboid.extent.x, cuboid.extent.z);
    heightRange[index] = glm::vec2(cuboid.center.y - cuboid.extent.y / 2.0f, cuboid.extent.y);
    colorValue[index] = cuboid.colorValue;
    gradientIndex[index] = cuboid.gradientIndex;
}

size_t CuboidVertexCloud::size() const
{
    return center.size();
}

size_t CuboidVertexCloud::byteSize() const
{
    return size() * vertexByteSize();
}

size_t CuboidVertexCloud::vertexByteSize() const
{
    return sizeof(float) * componentCount();
}

size_t CuboidVertexCloud::componentCount() const
{
    return 8;
}

void CuboidVertexCloud::reserve(size_t count)
{
    center.reserve(count);
    extent.reserve(count);
    heightRange.reserve(count);
    colorValue.reserve(count);
    gradientIndex.reserve(count);
}

void CuboidVertexCloud::resize(size_t count)
{
    center.resize(count);
    extent.resize(count);
    heightRange.resize(count);
    colorValue.resize(count);
    gradientIndex.resize(count);
}

void CuboidVertexCloud::render()
{
    glBindVertexArray(m_vao);

    // Pre-Z Pass
    //glDepthFunc(GL_LEQUAL);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glUseProgram(m_depthOnlyProgram);
    //glDrawArrays(GL_POINTS, 0, m_avc.size());

    // Color Pass
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glUseProgram(m_programs[0]);
    glDrawArrays(GL_POINTS, 0, size());

    glUseProgram(0);

    glBindVertexArray(0);
}

const std::vector<gl::GLuint> & CuboidVertexCloud::programs() const
{
    return m_programs;
}
