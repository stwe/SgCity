#pragma once

#include <glm/gtc/matrix_transform.hpp>

/*
    public static Matrix4f createModelMatrix(Vector2f position, Vector2f size) {
        Matrix4f modelMatrix = new Matrix4f();
        modelMatrix
                .identity()
                .translate(new Vector3f(position, 0.0f))
                .scale(new Vector3f(size, 1.0f));

        return modelMatrix;
    }
*/

namespace sg::ogl::math
{
    struct Transform
    {
        glm::vec3 position{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 scale{ glm::vec3(1.0f, 1.0f, 1.0f) };

        explicit operator glm::mat4() const
        {
            auto modelMatrix{ glm::mat4(1.0f) };

            modelMatrix = translate(modelMatrix, position);
            modelMatrix = glm::scale(modelMatrix, scale);

            return modelMatrix;
        }
    };
}
