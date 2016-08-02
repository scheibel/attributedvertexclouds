
#include <vector>
#include <string>

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/common.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#pragma warning(pop)

#include <glbinding/gl/types.h>

// Read raw binary file into a char vector (probably the fastest way).
std::vector<char> rawFromFile(const char * filePath);
std::vector<float> rawFromFileF(const std::string & filePath);

bool rawToFile(const char * filePath, const std::vector<char> & raw);

std::string textFromFile(const char * filePath);

bool createShader(gl::GLenum type, const std::string & name, const std::string & source, gl::GLuint & id);
bool createProgram(const std::string & name, gl::GLuint vertexShader, gl::GLuint fragmentShader, gl::GLuint & id);

bool checkForCompilationError(gl::GLuint shader, const std::string & identifier);
bool checkForLinkerError(gl::GLuint program, const std::string & identifier);

glm::vec3 cameraPath(const glm::vec3 & eye, float f);
