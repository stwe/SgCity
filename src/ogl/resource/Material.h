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

#include <cstdint>
#include <string>
#include <glm/vec3.hpp>

//-------------------------------------------------
// Material
//-------------------------------------------------

namespace sg::ogl::resource
{
    /**
     * Represents a Material.
     */
    struct Material
    {
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The material name statement.
         */
        std::string newmtl{};

        /**
         * The ambient color.
         */
        glm::vec3 ka{ glm::vec3(0.0f) };

        /**
         * The diffuse color.
         */
        glm::vec3 kd{ glm::vec3(0.0f) };

        /**
         * The specular color.
         */
        glm::vec3 ks{ glm::vec3(0.0f) };

        /**
         * Can be a number from 0 to 10 which represents various material lighting and shading effects.
         * illum = 1 indicates a flat material with no specular highlights, so the value of Ks is not used.
         * illum = 2 denotes the presence of specular highlights, and so a specification for Ks is required.
         */
        int illum{ 2 };

        /**
         * Specifies the specular exponent (shininess).
         * Ns values normally range from 0 to 1000.
         */
        float ns{ 0.0f };

        /**
         * The transparency of the material.
         * A factor of 1.0 is fully opaque.
         * A factor of 0.0 is fully dissolved (completely transparent).
         */
        float d{ 1.0f };

        /**
         * The ambientmap texture Id.
         */
        uint32_t mapKa{ 0 };

        /**
         * The diffusemap texture Id. Most of time, it will be the same as ambient texture map.
         */
        uint32_t mapKd{ 0 };

        /**
         * The specularmap texture Id.
         */
        uint32_t mapKs{ 0 };

        /**
         * The bumpmap texture Id.
         */
        uint32_t mapBump{ 0 };

        /**
         * The normalmap texture Id.
         */
        uint32_t mapKn{ 0 };

        //-------------------------------------------------
        // For convenience
        //-------------------------------------------------

        [[nodiscard]] bool HasAmbientMap() const { return mapKa != 0; };
        [[nodiscard]] bool HasDiffuseMap() const { return mapKd != 0; };
        [[nodiscard]] bool HasSpecularMap() const { return mapKs != 0; };
        [[nodiscard]] bool HasBumpMap() const { return mapBump != 0; };
        [[nodiscard]] bool HasNormalMap() const { return mapKn != 0; };
    };
}
