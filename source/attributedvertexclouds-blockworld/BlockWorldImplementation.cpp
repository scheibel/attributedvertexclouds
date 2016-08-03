
#include "BlockWorldImplementation.h"

#include <glbinding/gl/gl.h>

#include "common.h"

using namespace gl;

BlockWorldImplementation::BlockWorldImplementation(const std::string & name)
: m_name(name)
, m_initialized(false)
, m_blockSize(1.0f)
{
}

BlockWorldImplementation::~BlockWorldImplementation()
{
}

const std::string & BlockWorldImplementation::name() const
{
    return m_name;
}

bool BlockWorldImplementation::initialized() const
{
    return m_initialized;
}

void BlockWorldImplementation::initialize()
{
    if (!initialized())
    {
        onInitialize();

        m_initialized = true;
    }
}

void BlockWorldImplementation::render()
{
    initialize();

    onRender();
}

size_t BlockWorldImplementation::fullByteSize() const
{
    return byteSize() + staticByteSize();
}

void BlockWorldImplementation::setBlockSize(float size)
{
    m_blockSize = size;
}
