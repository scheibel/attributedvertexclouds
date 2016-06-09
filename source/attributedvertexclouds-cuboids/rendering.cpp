
#include "rendering.h"

#include <iostream>
#include <chrono>

#include <glbinding/gl32core/gl.h>


using namespace gl32core;


rendering::rendering()
: m_query(0)
{
}

rendering::~rendering()
{
    // Flag all aquired resources for deletion (hint: driver decides when to actually delete them; see: shared contexts)
    glDeleteQueries(1, &m_query);
}

void rendering::initialize()
{
    glGenQueries(1, &m_query);
}

void rendering::resize(int w, int h)
{
    m_width = w;
    m_height = h;
}

void rendering::render()
{
    static float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glViewport(0, 0, m_width, m_height);

    glFinish();
    const auto start = std::chrono::high_resolution_clock::now();

    glBeginQuery(gl::GL_TIME_ELAPSED, m_query);
    glClearBufferfv(GL_COLOR, 0, clearColor);
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

void rendering::execute()
{
    render();
}
