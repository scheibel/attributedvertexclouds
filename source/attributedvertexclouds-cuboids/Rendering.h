
#include <chrono>

#include <glbinding/gl/types.h>

#include "CuboidVertexCloud.h"
#include "CuboidTriangles.h"


// For more information on how to write C++ please adhere to: 
// http://cginternals.github.io/guidelines/cpp/index.html

enum class CuboidTechnique
{
    Triangles,
    TriangleStrip,
    VertexCloud
};

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
    void toggleMeasurements();
    void reloadShaders();

    void measureGPU(const std::string & name, std::function<void()> callback, bool on) const;
    void measureCPU(const std::string & name, std::function<void()> callback, bool on) const;

protected:
    CuboidTechnique m_current;
    CuboidVertexCloud m_avc;
    CuboidTriangles m_triangles;

    gl::GLuint m_query;

    int m_width;
    int m_height;

    std::chrono::high_resolution_clock::time_point m_start;

    bool m_measure;

    void updateUniforms();
};
