
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
#include "Postprocessing.h"


using namespace gl;


namespace
{


static const auto cuboidGridSize = size_t(48);
static const auto cuboidCount = cuboidGridSize * cuboidGridSize * cuboidGridSize;
static const auto fpsSampleCount = size_t(100);

static const auto worldScale = glm::vec3(1.0f) / glm::vec3(cuboidGridSize, cuboidGridSize, cuboidGridSize);
static const auto gridOffset = 0.2f;

static const auto lightGray = glm::vec3(234) / 275.0f;
static const auto red = glm::vec3(196, 30, 20) / 275.0f;
static const auto orange = glm::vec3(255, 114, 70) / 275.0f;
static const auto yellow = glm::vec3(255, 200, 107) / 275.0f;


} // namespace


Rendering::Rendering()
: m_current(nullptr)
, m_postprocessing(nullptr)
, m_query(0)
, m_gradientTexture(0)
, m_usePostprocessing(false)
, m_measure(false)
, m_rasterizerDiscard(false)
, m_fpsSamples(fpsSampleCount+1)
{
    m_implementations[0] = new CuboidTriangles;
    m_implementations[1] = new CuboidTriangleStrip;
    m_implementations[2] = new CuboidInstancing;
    m_implementations[3] = new CuboidVertexCloud;
    m_postprocessing = new Postprocessing;

    setTechnique(0);
}

Rendering::~Rendering()
{
    // Flag all aquired resources for deletion (hint: driver decides when to actually delete them; see: shared contexts)
    glDeleteQueries(1, &m_query);
    glDeleteTextures(1, &m_gradientTexture);
}

void Rendering::initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);

    createGeometry();

    glGenQueries(1, &m_query);

    glGenTextures(1, &m_gradientTexture);

    std::array<glm::vec3, 4> gradient = {{
        red,
        orange,
        yellow,
        lightGray
    }};

    glBindTexture(GL_TEXTURE_1D, m_gradientTexture);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F, gradient.size(), 0, GL_RGB, GL_FLOAT, gradient.data());
    glBindTexture(GL_TEXTURE_1D, 0);

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

    m_postprocessing->loadShader();
}

void Rendering::createGeometry()
{
    for (auto implementation : m_implementations)
    {
        implementation->resize(cuboidCount);
    }

    std::array<std::vector<float>, 4> noise;
    for (auto i = size_t(0); i < noise.size(); ++i)
    {
        noise[i] = rawFromFileF("data/noise/noise-48-"+std::to_string(i)+".raw");
    }

#pragma omp parallel for
    for (size_t i = 0; i < cuboidCount; ++i)
    {
        const auto position = glm::ivec3(i % cuboidGridSize, (i / cuboidGridSize) % cuboidGridSize, i / cuboidGridSize / cuboidGridSize);
        const auto offset = glm::vec3(
            (position.y + position.z) % 2 ? gridOffset : 0.0f,
            (position.x + position.z) % 2 ? gridOffset : 0.0f,
            (position.x + position.y) % 2 ? gridOffset : 0.0f
        );

        Cuboid c;
        c.center = glm::vec3(-0.5f, -0.5f, -0.5f) + (glm::vec3(position) + offset) * worldScale;
        c.extent = glm::mix(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(noise[0][i], noise[1][i], noise[2][i])) * worldScale;
        c.colorValue = glm::mix(0.0f, 1.0f, noise[3][i]);

        for (auto implementation : m_implementations)
        {
            implementation->setCube(i, c);
        }
    }
}

void Rendering::updateUniforms()
{
    static const auto eye = glm::vec3(1.0f, 1.0f, 1.0f);
    static const auto center = glm::vec3(0.0f, 0.0f, 0.0f);
    static const auto up = glm::vec3(0.0f, 1.0f, 0.0f);

    const auto f = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_start).count()) / 1000.0f;

    const auto view = glm::lookAt(cameraPath(eye, f), center, up);
    const auto viewProjection = glm::perspectiveFov(glm::radians(45.0f), float(m_width), float(m_height), 0.05f, 2.0f) * view;

    GLuint program = m_current->program();
    const auto viewProjectionLocation = glGetUniformLocation(program, "viewProjection");
    const auto gradientSamplerLocation = glGetUniformLocation(program, "gradient");
    glUseProgram(program);
    glUniformMatrix4fv(viewProjectionLocation, 1, GL_FALSE, glm::value_ptr(viewProjection));
    glUniform1i(gradientSamplerLocation, 0);

    glUseProgram(0);
}

void Rendering::resize(int w, int h)
{
    m_width = w;
    m_height = h;

    if (m_postprocessing->initialized())
    {
        m_postprocessing->resize(m_width, m_height);
    }
}

void Rendering::setTechnique(int i)
{
    m_current = m_implementations.at(i);

    switch (i)
    {
    case 0:
        std::cout << "Switch to Triangles implementation" << std::endl;
        break;
    case 1:
        std::cout << "Switch to TriangleStrip implementation" << std::endl;
        break;
    case 2:
        std::cout << "Switch to Instancing implementation" << std::endl;
        break;
    case 3:
        std::cout << "Switch to AttributedVertexCloud implementation" << std::endl;
        break;
    }
}

void Rendering::render()
{
    if (m_fpsSamples == fpsSampleCount)
    {
        const auto end = std::chrono::high_resolution_clock::now();

        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_fpsMeasurementStart).count() / 1000.0f / fpsSampleCount;

        std::cout << "Measured " << (1.0f / elapsed) << "FPS (" << "(~ " << (elapsed * 1000.0f) << "ms per frame)" << std::endl;

        m_fpsSamples = fpsSampleCount + 1;
    }

    if (m_fpsSamples < fpsSampleCount)
    {
        ++m_fpsSamples;
    }

    m_current->initialize();

    updateUniforms();

    glViewport(0, 0, m_width, m_height);

    if (m_usePostprocessing && !m_rasterizerDiscard)
    {
        static const float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

        if (!m_postprocessing->initialized())
        {
            m_postprocessing->initialize();
            m_postprocessing->resize(m_width, m_height);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_postprocessing->fbo());

        glClearBufferfv(GL_COLOR, 0, white);
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, m_gradientTexture);

    if (m_rasterizerDiscard)
    {
        glEnable(GL_RASTERIZER_DISCARD);
    }

    measureGPU("rendering", [this]() {
        m_current->render();
    }, m_measure);

    if (m_rasterizerDiscard)
    {
        glDisable(GL_RASTERIZER_DISCARD);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, 0);

    if (m_usePostprocessing && !m_rasterizerDiscard)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_postprocessing->render();
    }
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

void Rendering::toggleRasterizerDiscard()
{
    m_rasterizerDiscard = !m_rasterizerDiscard;
}

void Rendering::startFPSMeasuring()
{
    m_fpsSamples = 0;
    m_fpsMeasurementStart = std::chrono::high_resolution_clock::now();
}

void Rendering::togglePostprocessing()
{
    m_usePostprocessing = !m_usePostprocessing;
}
