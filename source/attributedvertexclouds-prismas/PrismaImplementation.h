
#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include "Prisma.h"


class PrismaImplementation
{
public:
    PrismaImplementation(const std::string & name);
    virtual ~PrismaImplementation();

    const std::string & name() const;

    bool initialized() const;

    void initialize();
    void render();

    size_t fullByteSize() const;

    // Subclass interface

    virtual void onInitialize() = 0;
    virtual void onRender() = 0;

    virtual bool loadShader() = 0;

    virtual void setPrisma(size_t index, const Prisma & prisma) = 0;

    virtual size_t size() const = 0;
    virtual size_t verticesCount() const = 0;
    virtual size_t staticByteSize() const = 0;
    virtual size_t byteSize() const = 0;
    virtual size_t vertexByteSize() const = 0;
    virtual size_t componentCount() const = 0;

    virtual void resize(size_t count) = 0;

    virtual gl::GLuint program() const = 0;
public:
    std::string m_name;
    bool m_initialized;
};
