
#include "Rendering.h"

#include <iostream>
#include <chrono>
#include <algorithm>

#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/gl.h>

#include "common.h"

#include "CuboidVertexCloud.h"
#include "CuboidTriangles.h"
#include "CuboidTriangleStrip.h"
#include "CuboidInstancing.h"


using namespace gl;


namespace
{


static const size_t cuboidCount = 50000;


} // namespace


Rendering::Rendering()
: m_current(nullptr)
, m_query(0)
, m_measure(false)
{
    m_implementations[0] = new CuboidTriangles;
    m_implementations[1] = new CuboidTriangleStrip;
    m_implementations[2] = new CuboidInstancing;
    m_implementations[3] = new CuboidVertexCloud;

    setTechnique(0);
}

Rendering::~Rendering()
{
    // Flag all aquired resources for deletion (hint: driver decides when to actually delete them; see: shared contexts)
    glDeleteQueries(1, &m_query);
}

void Rendering::initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);

    createGeometry();

    glGenQueries(1, &m_query);

    m_start = std::chrono::high_resolution_clock::now();
}

void Rendering::reloadShaders()
{
    for (auto implementation : m_implementations)
    {
        if (implementation->initialized())
        {
            implementation->loadShader();
        }
    }
}

void Rendering::createGeometry()
{
    for (auto implementation : m_implementations)
    {
        implementation->resize(cuboidCount);
    }

#pragma omp parallel for
    for (size_t i = 0; i < cuboidCount; ++i)
    {
        Cuboid c;
        c.center = glm::vec3(glm::linearRand(-8.0f, 8.0f), glm::linearRand(-0.5f, 0.5f), glm::linearRand(-8.0f, 8.0f));
        c.extent = glm::vec3(glm::linearRand(0.1f, 0.4f), glm::linearRand(0.1f, 0.4f), glm::linearRand(0.1f, 0.4f));
        c.colorValue = glm::linearRand(0.0f, 1.0f);
        c.gradientIndex = 0;

        for (auto implementation : m_implementations)
        {
            implementation->setCube(i, c);
        }
    }
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
    //const auto rotatedEye = glm::vec3(12.0f, 0.0f, 0.0f);

    const auto view = glm::lookAt(glm::vec3(rotatedEye), center, up);
    const auto viewProjection = glm::perspectiveFov(glm::radians(45.0f), float(m_width), float(m_height), 1.0f, 30.0f) * view;

    for (auto implementation : m_implementations)
    {
        if (implementation->initialized())
        {
            for (GLuint program : implementation->programs())
            {
                const auto viewProjectionLocation = glGetUniformLocation(program, "viewProjection");
                glUseProgram(program);
                glUniformMatrix4fv(viewProjectionLocation, 1, GL_FALSE, glm::value_ptr(viewProjection));
            }
        }
    }

    glUseProgram(0);
}

void Rendering::resize(int w, int h)
{
    m_width = w;
    m_height = h;
}

void Rendering::setTechnique(int i)
{
    m_current = m_implementations.at(i);
}

void Rendering::render()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, m_width, m_height);

    m_current->initialize();

    updateUniforms();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    measureGPU("rendering", [this]() {
        m_current->render();
    }, m_measure);
}

void Rendering::spaceMeasurement()
{
    const auto reference = std::accumulate(m_implementations.begin(), m_implementations.end(),
            std::accumulate(m_implementations.begin(), m_implementations.end(), 0, [](size_t currentSize, const CuboidImplementation * technique) {
                return std::max(currentSize, technique->fullByteSize());
            }), [](size_t currentSize, const CuboidImplementation * technique) {
        return std::min(currentSize, technique->fullByteSize());
    });

    const auto printSpaceMeasurement = [&reference](const std::string & techniqueName, size_t byteSize)
    {
        std::cout << techniqueName << std::endl << (byteSize / 1024) << "kB (" << (static_cast<float>(byteSize) / reference) << "x)" << std::endl;
    };

    std::cout << "Cuboid count: " << cuboidCount << std::endl;
    std::cout << std::endl;

    for (const auto implementation : m_implementations)
    {
        printSpaceMeasurement(implementation->name(), implementation->fullByteSize());
    }
}

void Rendering::measureCPU(const std::string & name, std::function<void()> callback, bool on) const
{
    if (!on)
    {
        return callback();
    }

    const auto start = std::chrono::high_resolution_clock::now();

    callback();

    const auto end = std::chrono::high_resolution_clock::now();

    std::cout << name << ": " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << "ns" << std::endl;
}

void Rendering::measureGPU(const std::string & name, std::function<void()> callback, bool on) const
{
    if (!on)
    {
        return callback();
    }

    glBeginQuery(gl::GL_TIME_ELAPSED, m_query);

    callback();

    glEndQuery(gl::GL_TIME_ELAPSED);

    int available = 0;
    while (!available)
    {
        glGetQueryObjectiv(m_query, gl::GL_QUERY_RESULT_AVAILABLE, &available);
    }

    int value;
    glGetQueryObjectiv(m_query, gl::GL_QUERY_RESULT, &value);

    std::cout << name << ": " << value << "ns" << std::endl;
}

void Rendering::togglePerformanceMeasurements()
{
    m_measure = !m_measure;
}
