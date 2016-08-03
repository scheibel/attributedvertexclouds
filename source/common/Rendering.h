
#include <chrono>
#include <vector>

#include <glbinding/gl/types.h>


class Implementation;
class Postprocessing;


class Rendering
{
public:
    Rendering();
    virtual ~Rendering();

    void initialize();

    void resize(int w, int h);
    void render();

    void setTechnique(int i);
    void toggleRasterizerDiscard();
    void togglePostprocessing();
    void spaceMeasurement();
    void reloadShaders();
    void startFPSMeasuring();

    void setGridSize(int gridSize);

    void measureGPU(const std::string & name, std::function<void()> callback, bool on) const;
    void measureCPU(const std::string & name, std::function<void()> callback, bool on) const;

protected:
    Implementation * m_current;
    Postprocessing * m_postprocessing;
    std::vector<Implementation *> m_implementations;

    gl::GLuint m_query;

    int m_width;
    int m_height;
    int m_gridSize;

    std::chrono::high_resolution_clock::time_point m_start;

    bool m_usePostprocessing;
    bool m_rasterizerDiscard;

    size_t m_fpsSamples;
    std::chrono::high_resolution_clock::time_point m_fpsMeasurementStart;

    void prepareRendering();
    void finalizeRendering();

    void addImplementation(Implementation * implementation);

    // Subclass interface

    virtual void onInitialize() = 0;
    virtual void onCreateGeometry() = 0;
    virtual void onPrepareRendering() = 0;
    virtual void onFinalizeRendering() = 0;
    virtual size_t primitiveCount() = 0;
};
