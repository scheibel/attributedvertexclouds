
#include <chrono>

#include <glbinding/gl/types.h>

#include "Rendering.h"


class TrajectoryRendering : public Rendering
{
public:
    TrajectoryRendering();
    virtual ~TrajectoryRendering();

protected:
    gl::GLuint m_gradientTexture;

    virtual void onInitialize() override;
    virtual void onCreateGeometry() override;
    virtual void onPrepareRendering() override;
    virtual void onFinalizeRendering() override;
    virtual size_t primitiveCount() override;
};
