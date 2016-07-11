
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
    }
}

void CuboidImplementation::render()
{
    initialize();

    onRender();
}
