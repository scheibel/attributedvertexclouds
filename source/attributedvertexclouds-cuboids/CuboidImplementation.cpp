
#include "CuboidImplementation.h"

#include <glbinding/gl/gl.h>

#include "common.h"

using namespace gl;

CuboidImplementation::CuboidImplementation()
: m_initialized(false)
{
}

CuboidImplementation::~CuboidImplementation()
{
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
