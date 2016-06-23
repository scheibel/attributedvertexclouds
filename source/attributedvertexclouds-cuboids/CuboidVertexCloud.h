
#pragma once

#include <vector>

#include <glm/vec2.hpp>


class CuboidVertexCloud
{
public:
    CuboidVertexCloud();

    size_t size() const;
    size_t byteSize() const;
    size_t vertexByteSize() const;
    size_t componentCount() const;

    void reserve(size_t count);
    void resize(size_t count);
public:
    std::vector<glm::vec2> center;
    std::vector<glm::vec2> extent;
    std::vector<glm::vec2> heightRange;
    std::vector<float> colorValue;
    std::vector<int> gradientIndex;
};
