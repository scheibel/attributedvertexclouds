
#include <chrono>

#include <glbinding/gl/types.h>


class CuboidImplementation;


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
    void togglePostprocessing();
    void spaceMeasurement();
    void reloadShaders();
    void startFPSMeasuring();

    void measureGPU(const std::string & name, std::function<void()> callback, bool on) const;
    void measureCPU(const std::string & name, std::function<void()> callback, bool on) const;

protected:
    CuboidImplementation * m_current;
    std::array<CuboidImplementation *, 4> m_implementations;

    gl::GLuint m_query;
    gl::GLuint m_gradientTexture;

    gl::GLuint m_fbo;
    gl::GLuint m_colorBuffer;
    gl::GLuint m_depthBuffer;

    gl::GLuint m_postprocessingVertices;
    gl::GLuint m_postprocessingVAO;
    gl::GLuint m_postProcessingProgram;
    gl::GLuint m_postProcessingVertexShader;
    gl::GLuint m_postProcessingFragmentShader;

    int m_width;
    int m_height;

    std::chrono::high_resolution_clock::time_point m_start;

    bool m_usePostprocessing;
    bool m_measure;
    bool m_rasterizerDiscard;

    size_t m_fpsSamples;
    std::chrono::high_resolution_clock::time_point m_fpsMeasurementStart;

    void updateUniforms();
    bool loadShader();
};
