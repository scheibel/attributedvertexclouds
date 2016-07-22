
#include <chrono>

#include <glbinding/gl/types.h>


class ArcImplementation;


class Rendering
{
public:
    Rendering();
    ~Rendering();

    void initialize();
    void createGeometry();

    void resize(int w, int h);
    void render();

    void setTechnique(int i);
    void togglePerformanceMeasurements();
    void toggleRasterizerDiscard();
    void spaceMeasurement();
    void reloadShaders();
    void startFPSMeasuring();

    void measureGPU(const std::string & name, std::function<void()> callback, bool on) const;
    void measureCPU(const std::string & name, std::function<void()> callback, bool on) const;

protected:
    ArcImplementation * m_current;
    std::array<ArcImplementation *, 4> m_implementations;

    gl::GLuint m_query;

    int m_width;
    int m_height;

    std::chrono::high_resolution_clock::time_point m_start;

    bool m_measure;
    bool m_rasterizerDiscard;

    size_t m_fpsSamples;
    std::chrono::high_resolution_clock::time_point m_fpsMeasurementStart;

    void updateUniforms();
};
