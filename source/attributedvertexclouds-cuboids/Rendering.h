
#include <chrono>

#include <glbinding/gl/types.h>

#include "CuboidVertexCloud.h"
#include "CuboidTriangles.h"
#include "CuboidTriangleStrip.h"


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
    CuboidTriangles m_triangles;
    CuboidTriangleStrip m_triangleStrip;
    CuboidVertexCloud m_avc;

    gl::GLuint m_query;

    int m_width;
    int m_height;

    std::chrono::high_resolution_clock::time_point m_start;

    bool m_measure;

    void updateUniforms();
};
