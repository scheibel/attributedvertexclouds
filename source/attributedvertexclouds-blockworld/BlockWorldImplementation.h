
#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include "Block.h"


class BlockWorldImplementation
{
public:
    BlockWorldImplementation(const std::string & name);
    virtual ~BlockWorldImplementation();

    const std::string & name() const;

    bool initialized() const;

    void initialize();
    void render();

    size_t fullByteSize() const;

    void setBlockSize(float size);

    // Subclass interface

    virtual void onInitialize() = 0;
    virtual void onRender() = 0;

    virtual bool loadShader() = 0;

    virtual void setBlock(size_t index, const Block & block) = 0;

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
    float m_blockSize;
};
