
#include "CuboidVertexCloud.h"


CuboidVertexCloud::CuboidVertexCloud()
{
}

size_t CuboidVertexCloud::size() const
{
    return center.size();
}

size_t CuboidVertexCloud::byteSize() const
{
    return size() * vertexByteSize();
}

size_t CuboidVertexCloud::vertexByteSize() const
{
    return sizeof(float) * componentCount();
}

size_t CuboidVertexCloud::componentCount() const
{
    return 8;
}

void CuboidVertexCloud::reserve(size_t count)
{
    center.reserve(count);
    extent.reserve(count);
    heightRange.reserve(count);
    colorValue.reserve(count);
    gradientIndex.reserve(count);
}

void CuboidVertexCloud::resize(size_t count)
{
    center.resize(count);
    extent.resize(count);
    heightRange.resize(count);
    colorValue.resize(count);
    gradientIndex.resize(count);
}
