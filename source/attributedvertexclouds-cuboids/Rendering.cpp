
#include "Rendering.h"

#include <iostream>
#include <chrono>

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/gl.h>

#include "common.h"


using namespace gl;


Rendering::Rendering()
: m_query(0)
{
}

Rendering::~Rendering()
{
    // Flag all aquired resources for deletion (hint: driver decides when to actually delete them; see: shared contexts)
    glDeleteQueries(1, &m_query);

    glDeleteBuffers(1, &m_vertices);
    glDeleteVertexArrays(1, &m_vao);
}

void Rendering::initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);

    createGeometry();

    glGenQueries(1, &m_query);

    glGenBuffers(1, &m_vertices);
    glGenVertexArrays(1, &m_vao);

    initializeVAO();

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    m_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    m_depthOnlyFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    m_program = glCreateProgram();
    m_depthOnlyProgram = glCreateProgram();

    glAttachShader(m_program, m_vertexShader);
    glAttachShader(m_program, m_geometryShader);
    glAttachShader(m_program, m_fragmentShader);

    glAttachShader(m_depthOnlyProgram, m_vertexShader);
    glAttachShader(m_depthOnlyProgram, m_geometryShader);
    glAttachShader(m_depthOnlyProgram, m_depthOnlyFragmentShader);

    loadShader();
}

void Rendering::createGeometry()
{
    m_avc.resize(1);

    m_avc.center.front() = glm::vec2(0.0f, 0.0f);
    m_avc.extent.front() = glm::vec2(1.0f, 1.0f);
    m_avc.heightRange.front() = glm::vec2(0.0f, 0.04f);
    m_avc.colorValue.front() = 0.5f;
    m_avc.gradientIndex.front() = 0;
}

void Rendering::initializeVAO()
{
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
    glBufferData(GL_ARRAY_BUFFER, m_avc.byteSize(), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, m_avc.size() * sizeof(float) * 0, m_avc.size() * sizeof(float) * 2, m_avc.center.data());
    glBufferSubData(GL_ARRAY_BUFFER, m_avc.size() * sizeof(float) * 2, m_avc.size() * sizeof(float) * 2, m_avc.extent.data());
    glBufferSubData(GL_ARRAY_BUFFER, m_avc.size() * sizeof(float) * 4, m_avc.size() * sizeof(float) * 2, m_avc.heightRange.data());
    glBufferSubData(GL_ARRAY_BUFFER, m_avc.size() * sizeof(float) * 6, m_avc.size() * sizeof(float) * 1, m_avc.colorValue.data());
    glBufferSubData(GL_ARRAY_BUFFER, m_avc.size() * sizeof(float) * 7, m_avc.size() * sizeof(float) * 1, m_avc.gradientIndex.data());

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(m_avc.size() * sizeof(float) * 0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(m_avc.size() * sizeof(float) * 2));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(m_avc.size() * sizeof(float) * 4));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), reinterpret_cast<void*>(m_avc.size() * sizeof(float) * 6));
    glVertexAttribIPointer(4, 1, GL_INT, sizeof(int), reinterpret_cast<void*>(m_avc.size() * sizeof(float) * 7));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Rendering::loadShader()
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


    const auto depthOnlyFragmentShaderSource = textFromFile("data/shaders/depthonly.frag");
    const auto depthOnlyFragmentShaderSource_ptr = depthOnlyFragmentShaderSource.c_str();
    if(depthOnlyFragmentShaderSource_ptr)
        glShaderSource(m_depthOnlyFragmentShader, 1, &depthOnlyFragmentShaderSource_ptr, 0);

    glCompileShader(m_depthOnlyFragmentShader);

    success &= checkForCompilationError(m_depthOnlyFragmentShader, "depth only fragment shader");


    if (!success)
    {
        return false;
    }

    glLinkProgram(m_program);

    success &= checkForLinkerError(m_program, "program");

    glLinkProgram(m_depthOnlyProgram);

    success &= checkForLinkerError(m_depthOnlyProgram, "depth only program");

    if (!success)
    {
        return false;
    }

    glBindFragDataLocation(m_program, 0, "out_color");

    return true;
}

void Rendering::updateUniforms()
{
    static const auto eye = glm::vec3(2.0f, 2.0f, 2.0f);
    static const auto center = glm::vec3(0.0f, 0.0f, 0.0f);
    static const auto up = glm::vec3(0.0f, 1.0f, 0.0f);
    static const auto view = glm::lookAt(eye, center, up);

    const auto viewProjection = glm::perspectiveFov(glm::radians(45.0f), float(m_width), float(m_height), 0.1f, 30.0f) * view;


    const auto viewProjectionLocation = glGetUniformLocation(m_program, "viewProjection");
    glUseProgram(m_program);
    glUniformMatrix4fv(viewProjectionLocation, 1, GL_FALSE, glm::value_ptr(viewProjection));

    const auto viewProjectionLocation2 = glGetUniformLocation(m_depthOnlyProgram, "viewProjection");
    glUseProgram(m_depthOnlyProgram);
    glUniformMatrix4fv(viewProjectionLocation2, 1, GL_FALSE, glm::value_ptr(viewProjection));

    glUseProgram(0);
}

void Rendering::resize(int w, int h)
{
    m_width = w;
    m_height = h;
}

void Rendering::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, m_width, m_height);

    updateUniforms();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(m_vao);
    glUseProgram(m_program);

    glDrawArrays(GL_POINTS, 0, m_avc.size());

    glBindVertexArray(0);
    glUseProgram(0);
}

void Rendering::measure(std::function<void()> callback, bool on) const
{
    if (!on)
    {
        return callback();
    }

    glFinish();
    const auto start = std::chrono::high_resolution_clock::now();

    glBeginQuery(gl::GL_TIME_ELAPSED, m_query);

    callback();

    glEndQuery(gl::GL_TIME_ELAPSED);

    glFinish();

    const auto end = std::chrono::high_resolution_clock::now();

    int available = 0;
    while (!available)
    {
        glGetQueryObjectiv(m_query, gl::GL_QUERY_RESULT_AVAILABLE, &available);
    }

    int value;
    glGetQueryObjectiv(m_query, gl::GL_QUERY_RESULT, &value);

    std::cout << "CPU measured: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << std::endl;
    std::cout << "GPU measured: " << value << "ns" << std::endl;
}
