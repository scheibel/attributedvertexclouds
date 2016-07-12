
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
    void togglePerformanceMeasurements(bool rasterizerDiscard);
    void spaceMeasurement();
    void reloadShaders();

    void measureGPU(const std::string & name, std::function<void()> callback, bool on) const;
    void measureCPU(const std::string & name, std::function<void()> callback, bool on) const;

protected:
    CuboidImplementation * m_current;
    std::array<CuboidImplementation *, 4> m_implementations;

    gl::GLuint m_query;

    int m_width;
    int m_height;

    std::chrono::high_resolution_clock::time_point m_start;

    bool m_measure;
    bool m_rasterizerDiscard;

    void updateUniforms();
};
