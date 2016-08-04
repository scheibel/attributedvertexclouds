
#include "PrismaRendering.h"

#include <iostream>
#include <chrono>
#include <algorithm>

#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/gl.h>

#include "common.h"

#include "PrismaVertexCloud.h"
//#include "PrismaTriangles.h"
//#include "PrismaTriangleStrip.h"
//#include "PrismaInstancing.h"


using namespace gl;


namespace
{


static const auto gridOffset = 0.2f;

static const auto lightGray = glm::vec3(234) / 275.0f;
static const auto red = glm::vec3(196, 30, 20) / 275.0f;
static const auto orange = glm::vec3(255, 114, 70) / 275.0f;
static const auto yellow = glm::vec3(255, 200, 107) / 275.0f;


} // namespace


PrismaRendering::PrismaRendering()
: Rendering("Prismas")
, m_gradientTexture(0)
{
}

PrismaRendering::~PrismaRendering()
{
    glDeleteTextures(1, &m_gradientTexture);
}

void PrismaRendering::onInitialize()
{
    addImplementation(new PrismaVertexCloud);

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
}

void PrismaRendering::onCreateGeometry()
{
    const auto prismaGridSize = m_gridSize;
    const auto prismaCount = prismaGridSize * prismaGridSize * prismaGridSize;
    const auto worldScale = glm::vec3(1.0f) / glm::vec3(prismaGridSize, prismaGridSize, prismaGridSize);

    for (auto implementation : m_implementations)
    {
        implementation->resize(prismaCount);
    }

    std::array<std::vector<float>, 4> noise;
    for (auto i = size_t(0); i < noise.size(); ++i)
    {
        noise[i] = rawFromFileF("data/noise/noise-"+std::to_string(prismaGridSize)+"-"+std::to_string(i)+".raw");
    }

#pragma omp parallel for
    for (size_t i = 0; i < prismaCount; ++i)
    {
        const auto position = glm::ivec3(i % prismaGridSize, (i / prismaGridSize) % prismaGridSize, i / prismaGridSize / prismaGridSize);
        const auto offset = glm::vec3(
            (position.y + position.z) % 2 ? gridOffset : 0.0f,
            (position.x + position.z) % 2 ? gridOffset : 0.0f,
            (position.x + position.y) % 2 ? gridOffset : 0.0f
        );

        Prisma p;

        p.heightRange.x = -0.5f + (position.y + offset.y) * worldScale.y - 0.5f * noise[0][i] * worldScale.y;
        p.heightRange.y = -0.5f + (position.y + offset.y) * worldScale.y + 0.5f * noise[0][i] * worldScale.y;

        const auto vertexCount = size_t(3) + size_t(glm::ceil(12.0f * noise[1][i]));
        const auto center = glm::vec2(-0.5f, -0.5f) + (glm::vec2(position.x, position.z) + glm::vec2(offset.x, offset.z)) * glm::vec2(worldScale.x, worldScale.z);
        const auto radius = 0.5f * 0.5f * (noise[2][i] + 1.0f);

        p.points.resize(vertexCount);

        for (auto j = size_t(0); j < vertexCount; ++j)
        {
            const auto angle = glm::pi<float>() * 2.0f * float(j) / float(vertexCount);
            const auto normalizedPosition = glm::vec2(
                glm::cos(angle),
                glm::sin(angle)
            );

            p.points[j] = center + glm::vec2(radius, radius) * normalizedPosition * glm::vec2(worldScale.x, worldScale.z);
        }

        p.colorValue = noise[3][i];

        for (auto implementation : m_implementations)
        {
            static_cast<PrismaImplementation*>(implementation)->setPrisma(i, p);
        }
    }
}

void PrismaRendering::onPrepareRendering()
{
    GLuint program = m_current->program();
    const auto gradientSamplerLocation = glGetUniformLocation(program, "gradient");
    glUseProgram(program);
    glUniform1i(gradientSamplerLocation, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, m_gradientTexture);
}

void PrismaRendering::onFinalizeRendering()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, 0);
}
