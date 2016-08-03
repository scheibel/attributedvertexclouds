
#include <iostream>

// C++ library for creating windows with OpenGL contexts and receiving 
// input and events http://www.glfw.org/ 
#include <GLFW/glfw3.h> 

// C++ binding for the OpenGL API. 
// https://github.com/cginternals/glbinding
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <glbinding/callbacks.h>

#include "PrismaRendering.h"


// From http://en.cppreference.com/w/cpp/language/namespace:
// "Unnamed namespace definition. Its members have potential scope 
// from their point of declaration to the end of the translation
// unit, and have internal linkage."
namespace
{

auto rendering = PrismaRendering();

const auto canvasWidth = 1440; // in pixel
const auto canvasHeight = 900; // in pixel

// The review mode is used by the tutors to semi-automatically unzip,
// configure, compile, and review  your submissions. The macro is
// defined via the CMake configuration and should only be used within
// the main.cpp (this) file.

// "The size callback ... which is called when the window is resized."
// http://www.glfw.org/docs/latest/group__window.html#gaa40cd24840daa8c62f36cafc847c72b6
void resizeCallback(GLFWwindow * /*window*/, int width, int height)
{
    rendering.resize(width, height);
}

void keyCallback(GLFWwindow * /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
    {
        std::cout << "Reload shaders" << std::endl;
        rendering.reloadShaders();
    }

    if (key == GLFW_KEY_F6 && action == GLFW_RELEASE)
    {
        std::cout << "Start FPS measuring" << std::endl;
        rendering.startFPSMeasuring();
    }

    if (key == GLFW_KEY_R && action == GLFW_RELEASE)
    {
        rendering.toggleRasterizerDiscard();
    }

    if (key == GLFW_KEY_P && action == GLFW_RELEASE)
    {
        rendering.spaceMeasurement();
    }

    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_4 && action == GLFW_RELEASE)
    {
        rendering.setTechnique(key - GLFW_KEY_1);
    }
}


// "In case a GLFW function fails, an error is reported to the GLFW 
// error callback. You can receive these reports with an error
// callback." http://www.glfw.org/docs/latest/quick.html#quick_capture_error
void errorCallback(int errnum, const char * errmsg)
{
    std::cerr << errnum << ": " << errmsg << std::endl;
}


}


int main(int /*argc*/, char ** /*argv*/)
{
    if (!glfwInit())
    {
        return 1;
    }

    std::cout << "Choose Techniques" << std::endl;
    std::cout << " [1] Triangles" << std::endl;
    std::cout << " [2] Triangle Strip" << std::endl;
    std::cout << " [3] Instancing" << std::endl;
    std::cout << " [4] Attributed Vertex Cloud" << std::endl;
    std::cout << std::endl;
    std::cout << "Measuring" << std::endl;
    std::cout << " [p] Start/Stop Performance Measurements" << std::endl;
    std::cout << " [m] Print Memory Measurements" << std::endl;
    std::cout << std::endl;
    std::cout << "Debugging" << std::endl;
    std::cout << " [r] Enable/Disable rasterizer" << std::endl;
    std::cout << " [F5]: Shader Reload" << std::endl;

    glfwSetErrorCallback(errorCallback);

    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(canvasWidth, canvasHeight, "", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();

        return 2;
    }

    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);

    glfwMakeContextCurrent(window);

    glbinding::Binding::initialize(false);

    glbinding::setAfterCallback([](const glbinding::FunctionCall & functionCall) {
        gl::GLenum error = glbinding::Binding::GetError.directCall();

        if (error != gl::GL_NO_ERROR)
        {
            throw error;
        }
    });

    glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After, { "glGetError" });

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    rendering.resize(width, height);
    rendering.initialize();

    while (!glfwWindowShouldClose(window)) // main loop
    {
        glfwPollEvents();

        rendering.render();

        glfwSwapBuffers(window);
    }

    glfwMakeContextCurrent(nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
