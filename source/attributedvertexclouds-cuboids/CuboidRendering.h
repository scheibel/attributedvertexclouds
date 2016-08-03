
#include <chrono>

#include <glbinding/gl/types.h>


class CuboidImplementation;
class Postprocessing;


class CuboidRendering
{
public:
    CuboidRendering();
    ~CuboidRendering();

    void initialize();
    void createGeometry();

    void resize(int w, int h);
    void render();

    void setTechnique(int i);
    void toggleRasterizerDiscard();
    void togglePostprocessing();
    void spaceMeasurement();
    void reloadShaders();
    void startFPSMeasuring();

    void measureGPU(const std::string & name, std::function<void()> callback, bool on) const;
    void measureCPU(const std::string & name, std::function<void()> callback, bool on) const;

protected:
    CuboidImplementation * m_current;
    Postprocessing * m_postprocessing;
    std::array<CuboidImplementation *, 4> m_implementations;

    gl::GLuint m_query;
    gl::GLuint m_gradientTexture;

    int m_width;
    int m_height;

    std::chrono::high_resolution_clock::time_point m_start;

    bool m_usePostprocessing;
    bool m_rasterizerDiscard;

    size_t m_fpsSamples;
    std::chrono::high_resolution_clock::time_point m_fpsMeasurementStart;

    void updateUniforms();
};
