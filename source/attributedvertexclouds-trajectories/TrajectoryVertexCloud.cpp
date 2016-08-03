
#include "TrajectoryVertexCloud.h"

#include <glbinding/gl/gl.h>

#include "common.h"

using namespace gl;

TrajectoryVertexCloud::TrajectoryVertexCloud()
: m_initialized(false)
, m_vertices(0)
, m_vao(0)
, m_vertexShader(0)
, m_tessControlShader(0)
, m_tessEvaluationShader(0)
, m_geometryShader(0)
, m_fragmentShader(0)
{
}

TrajectoryVertexCloud::~TrajectoryVertexCloud()
{
    glDeleteBuffers(1, &m_vertices);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_tessControlShader);
    glDeleteShader(m_tessEvaluationShader);
    glDeleteShader(m_geometryShader);
    glDeleteShader(m_fragmentShader);
    glDeleteProgram(m_program);
}

bool TrajectoryVertexCloud::initialized() const
{
    return m_initialized;
}

void TrajectoryVertexCloud::initialize()
{
    if (!initialized())
    {
        onInitialize();

        m_initialized = true;
    }
}

void TrajectoryVertexCloud::render()
{
    initialize();

    onRender();
}

void TrajectoryVertexCloud::onInitialize()
{
    glGenBuffers(1, &m_vertices);
    glGenVertexArrays(1, &m_vao);

    initializeVAO();

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    m_tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    m_tessEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    m_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    m_program = glCreateProgram();

    glAttachShader(m_program, m_vertexShader);
    glAttachShader(m_program, m_tessControlShader);
    glAttachShader(m_program, m_tessEvaluationShader);
    glAttachShader(m_program, m_geometryShader);
    glAttachShader(m_program, m_fragmentShader);

    loadShader();
}

void TrajectoryVertexCloud::initializeVAO()
{
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    glBufferData(GL_ARRAY_BUFFER, byteSize(), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 0, verticesCount() * sizeof(float) * 3, m_position.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 3, verticesCount() * sizeof(float) * 1, m_type.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 4, verticesCount() * sizeof(float) * 1, m_colorValue.data());
    glBufferSubData(GL_ARRAY_BUFFER, verticesCount() * sizeof(float) * 5, verticesCount() * sizeof(float) * 1, m_sizeValue.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 0));
    glVertexAttribIPointer(1, 1, GL_INT, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 3));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 4));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesCount() * sizeof(float) * 5));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool TrajectoryVertexCloud::loadShader()
{
    const auto vertexShaderSource = textFromFile("data/shaders/trajectories-avc/standard.vert");
    const auto vertexShaderSource_ptr = vertexShaderSource.c_str();
    if(vertexShaderSource_ptr)
        glShaderSource(m_vertexShader, 1, &vertexShaderSource_ptr, 0);

    glCompileShader(m_vertexShader);

    bool success = checkForCompilationError(m_vertexShader, "vertex shader");


    const auto tessControlShaderSource = textFromFile("data/shaders/trajectories-avc/standard.tcs");
    const auto tessControlShaderSource_ptr = tessControlShaderSource.c_str();
    if(tessControlShaderSource_ptr)
        glShaderSource(m_tessControlShader, 1, &tessControlShaderSource_ptr, 0);

    glCompileShader(m_tessControlShader);

    success &= checkForCompilationError(m_tessControlShader, "tessellation control shader");


    const auto tessEvaluationShaderSource = textFromFile("data/shaders/trajectories-avc/standard.tes");
    const auto tessEvaluationShaderSource_ptr = tessEvaluationShaderSource.c_str();
    if(tessEvaluationShaderSource_ptr)
        glShaderSource(m_tessEvaluationShader, 1, &tessEvaluationShaderSource_ptr, 0);

    glCompileShader(m_tessEvaluationShader);

    success &= checkForCompilationError(m_tessEvaluationShader, "tessellation evaluation shader");


    const auto geometryShaderSource = textFromFile("data/shaders/trajectories-avc/standard.geom");
    const auto geometryShaderSource_ptr = geometryShaderSource.c_str();
    if(geometryShaderSource_ptr)
        glShaderSource(m_geometryShader, 1, &geometryShaderSource_ptr, 0);

    glCompileShader(m_geometryShader);

    success &= checkForCompilationError(m_geometryShader, "geometry shader");


    const auto fragmentShaderSource = textFromFile("data/shaders/trajectories-avc/standard.frag");
    const auto fragmentShaderSource_ptr = fragmentShaderSource.c_str();
    if(fragmentShaderSource_ptr)
        glShaderSource(m_fragmentShader, 1, &fragmentShaderSource_ptr, 0);

    glCompileShader(m_fragmentShader);

    success &= checkForCompilationError(m_fragmentShader, "fragment shader");


    if (!success)
    {
        return false;
    }

    glLinkProgram(m_program);

    success &= checkForLinkerError(m_program, "program");

    if (!success)
    {
        return false;
    }

    glBindFragDataLocation(m_program, 0, "out_color");

    return true;
}

void TrajectoryVertexCloud::setTrajectoryNode(size_t index, const TrajectoryNode & node)
{
    m_position[index] = node.position;
    m_type[index] = node.type;
    m_colorValue[index] = node.colorValue;
    m_sizeValue[index] = node.sizeValue;
}

size_t TrajectoryVertexCloud::size() const
{
    return m_position.size();
}

size_t TrajectoryVertexCloud::verticesPerNode() const
{
    return 1;
}

size_t TrajectoryVertexCloud::verticesCount() const
{
    return size() * verticesPerNode();
}

size_t TrajectoryVertexCloud::staticByteSize() const
{
    return 0;
}

size_t TrajectoryVertexCloud::byteSize() const
{
    return verticesPerNode() * size() * vertexByteSize();
}

size_t TrajectoryVertexCloud::vertexByteSize() const
{
    return sizeof(float) * componentCount();
}

size_t TrajectoryVertexCloud::componentCount() const
{
    return 6;
}

void TrajectoryVertexCloud::resize(size_t count)
{
    m_position.resize(count);
    m_type.resize(count);
    m_colorValue.resize(count);
    m_sizeValue.resize(count);
}

void TrajectoryVertexCloud::onRender()
{
    glBindVertexArray(m_vao);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glPatchParameteri(GL_PATCH_VERTICES, 1);

    glUseProgram(m_program);
    glDrawArrays(GL_PATCHES, 0, size());

    glUseProgram(0);

    glBindVertexArray(0);
}

gl::GLuint TrajectoryVertexCloud::program() const
{
    return m_program;
}
