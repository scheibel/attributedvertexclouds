
#include <chrono>

#include <glbinding/gl32core/gl.h>  // this is a OpenGL feature include; it declares all OpenGL 3.2 Core symbols


// For more information on how to write C++ please adhere to: 
// http://cginternals.github.io/guidelines/cpp/index.html

class rendering
{
public:
    rendering();
    ~rendering();

    void initialize();

    void resize(int w, int h);
    void render();
    void execute();

protected:
    gl::GLuint m_query;

    int m_width;
    int m_height;
};
