
#include <chrono>

#include <glbinding/gl/types.h>

#include "CuboidVertexCloud.h"


// For more information on how to write C++ please adhere to: 
// http://cginternals.github.io/guidelines/cpp/index.html

class Rendering
{
public:
    Rendering();
    ~Rendering();

    void initialize();
    bool loadShader();
    void createGeometry();
    void initializeVAO();
    void updateUniforms();

    void resize(int w, int h);
    void render();

    void measure(std::function<void()> callback, bool on) const;

protected:
    CuboidVertexCloud m_avc;

    gl::GLuint m_query;

    gl::GLuint m_vertices;
    gl::GLuint m_vao;

    gl::GLuint m_vertexShader;
    gl::GLuint m_geometryShader;
    gl::GLuint m_fragmentShader;
    gl::GLuint m_depthOnlyFragmentShader;
    gl::GLuint m_program;
    gl::GLuint m_depthOnlyProgram;

    int m_width;
    int m_height;
};
