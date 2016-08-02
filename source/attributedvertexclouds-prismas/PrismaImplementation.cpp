
#include "PrismaImplementation.h"

#include <glbinding/gl/gl.h>

#include "common.h"

using namespace gl;

PrismaImplementation::PrismaImplementation(const std::string & name)
: m_name(name)
, m_initialized(false)
{
}

PrismaImplementation::~PrismaImplementation()
{
}

const std::string & PrismaImplementation::name() const
{
    return m_name;
}

bool PrismaImplementation::initialized() const
{
    return m_initialized;
}

void PrismaImplementation::initialize()
{
    if (!initialized())
    {
        onInitialize();

        m_initialized = true;
    }
}

void PrismaImplementation::render()
{
    initialize();

    onRender();
}

size_t PrismaImplementation::fullByteSize() const
{
    return byteSize() + staticByteSize();
}
