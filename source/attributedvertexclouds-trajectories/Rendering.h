
#include <chrono>

#include <glbinding/gl/types.h>


class TrajectoryVertexCloud;


class Rendering
{
public:
    Rendering();
    ~Rendering();

    void initialize();
    void createGeometry();

    void resize(int w, int h);
    void render();

    void togglePerformanceMeasurements();
    void toggleRasterizerDiscard();
    void reloadShaders();
    void startFPSMeasuring();

    void measureGPU(const std::string & name, std::function<void()> callback, bool on) const;
    void measureCPU(const std::string & name, std::function<void()> callback, bool on) const;

protected:
    TrajectoryVertexCloud * m_current;

    gl::GLuint m_query;
    gl::GLuint m_gradientTexture;

    int m_width;
    int m_height;

    std::chrono::high_resolution_clock::time_point m_start;

    bool m_measure;
    bool m_rasterizerDiscard;

    size_t m_fpsSamples;
    std::chrono::high_resolution_clock::time_point m_fpsMeasurementStart;

    void updateUniforms();
};
