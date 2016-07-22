
#include "common.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/vec4.hpp>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#pragma warning(pop)

#include <glbinding/gl32core/gl.h>  // this is a OpenGL feature include; it declares all OpenGL 3.2 Core symbols

using namespace gl;

// Read raw binary file into a char vector (probably the fastest way).

std::vector<char> rawFromFile(const char * filePath)
{
    auto stream = std::ifstream(filePath, std::ios::in | std::ios::binary | std::ios::ate);

    if (!stream)
    {
        std::cerr << "Reading from file '" << filePath << "' failed." << std::endl;
        return std::vector<char>();
    }

    stream.seekg(0, std::ios::end);

    const auto size = stream.tellg();
    auto raw = std::vector<char>(size);

    stream.seekg(0, std::ios::beg);
    stream.read(raw.data(), size);

    return raw;
}

std::string textFromFile(const char * filePath)
{
    const auto text = rawFromFile(filePath);
    return std::string(text.begin(), text.end());
}

bool checkForCompilationError(GLuint shader, const std::string & identifier)
{
    auto success = static_cast<GLint>(GL_FALSE);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success != static_cast<GLint>(GL_FALSE))
        return true;

    auto length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    std::vector<char> log(length);

    glGetShaderInfoLog(shader, length, &length, log.data());

    std::cerr
        << "Compiler error in " << identifier << ":" << std::endl
        << std::string(log.data(), length) << std::endl;

    return false;
}

bool rawToFile(const char * filePath, const std::vector<char> & raw)
{
    auto stream = std::ofstream(filePath, std::ios::out | std::ios::binary);

    if (!stream)
    {
        std::cerr << "Writing to file '" << filePath << "' failed." << std::endl;
        return false;
    }

    stream.write(raw.data(), raw.size());

    return true;
}

bool checkForLinkerError(GLuint program, const std::string & identifier)
{
    auto success = static_cast<GLint>(GL_FALSE);
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success != static_cast<GLint>(GL_FALSE))
        return true;

    auto length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    std::vector<char> log(length);

    glGetProgramInfoLog(program, length, &length, log.data());

    std::cerr
        << "Linker error in " << identifier << ":" << std::endl
        << std::string(log.data(), length) << std::endl;

    return false;

}
