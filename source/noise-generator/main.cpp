
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glkernel/Kernel.h>
#include <glkernel/noise.h>
#include <glkernel/scale.h>

#include <fstream>


namespace
{


const auto gridSize = size_t(32);
const auto componentSize = size_t(8);


}


int main(int /*argc*/, char ** /*argv*/)
{
    // 8 random numbers per
    auto kernel = glkernel::kernel1{ gridSize, gridSize, gridSize };

    for (auto i = size_t(0); i < componentSize; ++i)
    {
        glkernel::noise::gradient(kernel, glkernel::noise::GradientNoiseType::Simplex);
        //glkernel::scale::range(kernel, 0.0f, 1.0f, -1.0f, 1.0f);

        std::fstream file("data/noise/noise-"+std::to_string(gridSize)+"-"+std::to_string(i)+".raw", std::fstream::out | std::fstream::binary);

        file.write(reinterpret_cast<char *>(kernel.data()), sizeof(float) * kernel.size());
        file.close();
    }
}
