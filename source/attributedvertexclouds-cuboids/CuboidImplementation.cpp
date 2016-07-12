
#include "CuboidImplementation.h"

#include <glbinding/gl/gl.h>

#include "common.h"

using namespace gl;

CuboidImplementation::CuboidImplementation(const std::string & name)
: m_name(name)
, m_initialized(false)
{
}

CuboidImplementation::~CuboidImplementation()
{
}

const std::string & CuboidImplementation::name() const
{
    return m_name;
}

bool CuboidImplementation::initialized() const
{
    return m_initialized;
}

void CuboidImplementation::initialize()
{
    if (!initialized())
    {
        onInitialize();

        m_initialized = true;
    }
}

void CuboidImplementation::render()
{
    initialize();

    onRender();
}

size_t CuboidImplementation::fullByteSize() const
{
    return byteSize() + staticByteSize();
}
