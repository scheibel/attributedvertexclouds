
#include "Rendering.h"

#include <iostream>
#include <chrono>

#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/gl.h>

#include "common.h"


using namespace gl;


Rendering::Rendering()
: m_query(0)
, m_measure(false)
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
    glDepthFunc(GL_LEQUAL);

    createGeometry();

    glGenQueries(1, &m_query);

    glGenBuffers(1, &m_vertices);
    glGenVertexArrays(1, &m_vao);

    initializeVAO();

    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    m_geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    m_program = glCreateProgram();
    m_depthOnlyProgram = glCreateProgram();

    glAttachShader(m_program, m_vertexShader);
    glAttachShader(m_program, m_geometryShader);
    glAttachShader(m_program, m_fragmentShader);

    glAttachShader(m_depthOnlyProgram, m_vertexShader);
    glAttachShader(m_depthOnlyProgram, m_geometryShader);

    loadShader();

    m_start = std::chrono::high_resolution_clock::now();
}

void Rendering::createGeometry()
{
    static const size_t cuboidCount = 10000;

    m_avc.resize(cuboidCount);

#pragma omp parallel for
    for (size_t i = 0; i < cuboidCount; ++i)
    {
        m_avc.center[i] = glm::vec2(glm::linearRand(-8.0f, 8.0f), glm::linearRand(-8.0f, 8.0f));
        m_avc.extent[i] = glm::vec2(glm::linearRand(0.1f, 0.4f), glm::linearRand(0.1f, 0.4f));
        m_avc.heightRange[i] = glm::vec2(glm::linearRand(-0.5f, 0.5f));
        m_avc.heightRange[i].y += glm::linearRand(0.1f, 0.4f);
        m_avc.colorValue[i] = glm::linearRand(0.0f, 1.0f);
        m_avc.gradientIndex[i] = 0;
    }
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
    static const auto eye = glm::vec3(1.0f, 12.0f, 1.0f);
    static const auto center = glm::vec3(0.0f, 0.0f, 0.0f);
    static const auto up = glm::vec3(0.0f, 1.0f, 0.0f);

    const auto f = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_start).count()) / 1000.0f;

    auto eyeRotation = glm::mat4(1.0f);
    eyeRotation = glm::rotate(eyeRotation, glm::sin(0.8342378f * f), glm::vec3(0.0f, 1.0f, 0.0f));
    eyeRotation = glm::rotate(eyeRotation, glm::cos(-0.5423543f * f), glm::vec3(1.0f, 0.0f, 0.0f));
    eyeRotation = glm::rotate(eyeRotation, glm::sin(0.13234823f * f), glm::vec3(0.0f, 0.0f, 1.0f));

    const auto rotatedEye = eyeRotation * glm::vec4(eye, 1.0f);

    const auto view = glm::lookAt(glm::vec3(rotatedEye), center, up);
    const auto viewProjection = glm::perspectiveFov(glm::radians(45.0f), float(m_width), float(m_height), 1.0f, 30.0f) * view;


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

    measure([this]() {
        // Pre-Z Pass
        //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        //glUseProgram(m_depthOnlyProgram);
        //glDrawArrays(GL_POINTS, 0, m_avc.size());

        // Color Pass
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glUseProgram(m_program);
        glDrawArrays(GL_POINTS, 0, m_avc.size());
    }, m_measure);

    glUseProgram(0);

    glBindVertexArray(0);
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

void Rendering::toggleMeasurements()
{
    m_measure = !m_measure;
}
