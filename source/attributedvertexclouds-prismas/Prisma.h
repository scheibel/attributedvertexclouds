
#pragma once

#include <vector>

#include <glm/vec2.hpp>

class Prisma
{
public:
    Prisma();

    std::vector<glm::vec2> points;
    glm::vec2 heightRange;
    float colorValue;
    int gradientIndex;
};
