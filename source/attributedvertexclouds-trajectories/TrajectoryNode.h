
#pragma once

#include <glm/vec3.hpp>


class TrajectoryNode
{
public:
    TrajectoryNode();

    glm::vec3 position;
    int type;
    float colorValue;
    float sizeValue;
};
