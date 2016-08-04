
#include <glbinding/gl/types.h>

#include "Rendering.h"


class PrismaRendering : public Rendering
{
public:
    PrismaRendering();
    virtual ~PrismaRendering();

protected:
    gl::GLuint m_gradientTexture;

    virtual void onInitialize() override;
    virtual void onCreateGeometry() override;
    virtual void onPrepareRendering() override;
    virtual void onFinalizeRendering() override;
};
