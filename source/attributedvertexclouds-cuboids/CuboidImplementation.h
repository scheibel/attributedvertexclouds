
#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include "Cuboid.h"


class CuboidImplementation
{
public:
    CuboidImplementation();
    virtual ~CuboidImplementation();

    bool initialized() const;

    void initialize();
    void render();

    virtual void onInitialize() = 0;
    virtual void onRender() = 0;

    virtual bool loadShader() = 0;

    virtual void setCube(size_t index, const Cuboid & cuboid) = 0;

    virtual size_t size() const = 0;
    virtual size_t verticesPerCuboid() const = 0;
    virtual size_t verticesCount() const = 0;
    virtual size_t byteSize() const = 0;
    virtual size_t vertexByteSize() const = 0;
    virtual size_t componentCount() const = 0;

    virtual void resize(size_t count) = 0;

    virtual const std::vector<gl::GLuint> & programs() const = 0;
public:
    bool m_initialized;
};
