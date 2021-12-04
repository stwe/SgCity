#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace sg::ogl::math
{
    class Transform
    {
    public:
        static glm::mat4 CreateModelMatrix(const glm::vec2& t_position, const glm::vec2& t_size)
        {
            auto modelMatrix{ glm::mat4(1.0f) };

            modelMatrix = glm::translate(modelMatrix, glm::vec3(t_position, 0.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(t_size, 1.0f));

            return modelMatrix;
        }

    protected:

    private:
    };
}
