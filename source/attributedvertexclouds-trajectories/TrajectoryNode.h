
#pragma once

#include <glm/vec3.hpp>


class TrajectoryNode
{
public:
    TrajectoryNode();

    glm::vec3 position;
    int type;
    glm::vec3 incoming;
    glm::vec3 outgoing;
    float colorValue;
    float sizeValue;
};
