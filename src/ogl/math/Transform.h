// This file is part of the SgCity project.
//
// Copyright (c) 2022. stwe <https://github.com/stwe/SgCity>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

#include <glm/gtc/matrix_transform.hpp>

//-------------------------------------------------
// Transform
//-------------------------------------------------

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
