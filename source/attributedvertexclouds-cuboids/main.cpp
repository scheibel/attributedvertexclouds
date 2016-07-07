
#include <iostream>

// C++ library for creating windows with OpenGL contexts and receiving 
// input and events http://www.glfw.org/ 
#include <GLFW/glfw3.h> 

// C++ binding for the OpenGL API. 
// https://github.com/cginternals/glbinding
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <glbinding/callbacks.h>

#include "Rendering.h"


// From http://en.cppreference.com/w/cpp/language/namespace:
// "Unnamed namespace definition. Its members have potential scope 
// from their point of declaration to the end of the translation
// unit, and have internal linkage."
namespace
{

auto exercise = Rendering();

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
    exercise.resize(width, height);
}

void keyCallback(GLFWwindow * /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_F5 && action == GLFW_RELEASE)
    {
        std::cout << "Reload shaders" << std::endl;
        exercise.loadShader();
    }

    if (key == GLFW_KEY_M && action == GLFW_RELEASE)
    {
        exercise.toggleMeasurements();
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

    std::cout << " [F5]: Reload shaders." << std::endl;

    glfwSetErrorCallback(errorCallback);

    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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
    exercise.resize(width, height);
    exercise.initialize();

    while (!glfwWindowShouldClose(window)) // main loop
    {
        glfwPollEvents();

        exercise.render();

        glfwSwapBuffers(window);
    }

    glfwMakeContextCurrent(nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
