#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace sg::ogl::math
{
    class Transform
    {
    public:
        static glm::mat4 CreateModelMatrix(const glm::vec3& t_position, const glm::vec3& t_rotation, const glm::vec3& t_size)
        {
            auto modelMatrix{ glm::mat4(1.0f) };

            modelMatrix = glm::translate(modelMatrix, t_position);
            modelMatrix = glm::rotate(modelMatrix, glm::radians(t_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(t_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(t_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMatrix = glm::scale(modelMatrix, t_size);

            return modelMatrix;
        }

    protected:

    private:
    };
}
