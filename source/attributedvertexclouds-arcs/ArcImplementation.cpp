
#include "ArcImplementation.h"

#include <glbinding/gl/gl.h>

#include "common.h"

using namespace gl;

ArcImplementation::ArcImplementation(const std::string & name)
: m_name(name)
, m_initialized(false)
{
}

ArcImplementation::~ArcImplementation()
{
}

const std::string & ArcImplementation::name() const
{
    return m_name;
}

bool ArcImplementation::initialized() const
{
    return m_initialized;
}

void ArcImplementation::initialize()
{
    if (!initialized())
    {
        onInitialize();

        m_initialized = true;
    }
}

void ArcImplementation::render()
{
    initialize();

    onRender();
}

size_t ArcImplementation::fullByteSize() const
{
    return byteSize() + staticByteSize();
}
